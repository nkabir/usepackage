
/* usepackage.c */

/* Jonathan AH Hogg */


/*** uses: ***/

#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <string.h>
#include <sys/utsname.h>
#include "linked_list.h"
#include "packages.h"


/*** prototypes: ***/

int is_csh_user(void);
void add_package(package_t* package);
void use_package(char* name);
void use_group(group_t* group);
group_t* get_group(char* name);
void print_env(void);
list_node* get_into_env(variable_t* var);
linked_list* make_pathlist(char* path_string);
variable_t* update_var(variable_t* evar, variable_t* vvar);
void print_path(linked_list* pathlist);
linked_list* merge_paths(linked_list* elist, linked_list* vlist);


/*** globals: ***/

int debugging = 0;
int silent = 0;
int csh_user;
struct utsname the_host_info;
linked_list* the_packages;
linked_list* the_groups;
linked_list* the_environment;
char* the_home;
char* main_package_filename = MAIN_PACKAGE_FILE;


/*** main program: ***/

void main(int argc, char *argv[])
{
   int first,i;
   char* f;
   group_t* group;
   int sh_override = -1;

   for (i=1; i<argc && *argv[i] == '-'; i++)
   {
      for (f=argv[i]+1; *f; f++)
	 switch (*f)
	 {
	    case 'v':
	       debugging = 1;
	       silent = 0;
	       break;
	    case 's':
	       debugging = 0;
	       silent = 1;
	       break;
	    case 'c':
	       sh_override = 1;
	       break;
	    case 'b':
	       sh_override = 0;
	       break;
	    case 'f':
	       main_package_filename = argv[++i];
	       break;
	    default:
	       fprintf(stderr, "usepackage: unrecognised flag '%c'\n", *f);
	       exit(1);
	 }
   }

   if (i >= argc)
   {
      fprintf(stderr, "usage: usepackage [-vscb] [-f <file>] <package> [<package>...]\n\n");
      fprintf(stderr, "       -v : verbose\n");
      fprintf(stderr, "       -s : silence match warnings\n");
      fprintf(stderr, "       -c : force csh style output\n");
      fprintf(stderr, "       -b : force sh style output\n");
      fprintf(stderr, "       -f : use <file> as main packages file\n");
      exit(1);
   }

   DEBUG("# usepackage\n");
   DEBUG("# Version: $Revision$\n");
   DEBUG("# Copyright (c) Jonathan Hogg, 1995\n");

   uname(&the_host_info);
   DEBUG("# host: %s\n", the_host_info.nodename);
   DEBUG("# operating system: %s %s\n", the_host_info.sysname,
         the_host_info.release);
   DEBUG("# architecture: %s\n", the_host_info.machine);

   csh_user = is_csh_user();
   if (sh_override != -1) csh_user = sh_override;

   the_environment = new_list();
   if (get_packages(&the_packages, &the_groups))
   {
      fprintf(stderr, "usepackage: couldn't load package information.\n");
      exit(2);
   }

   for (first = i, i = argc - 1 ; i >= first ; i--)
   {
      if (group = get_group(argv[i]))
         use_group(group);
      else
         use_package(argv[i]);
   }

   print_env();

   exit(0);
}


/*** functions: ***/

int is_csh_user(void)
{
   struct passwd *pwent;
   char *shell;

   pwent = getpwuid(getuid());
   shell = strrchr(pwent->pw_shell, '/');
   the_home = strdup(pwent->pw_dir);
   if (!shell) return(0);
   shell++;

   DEBUG("# shell: %s\n", shell);
   DEBUG("# home: %s\n", the_home);

   return ((!strcmp(shell, "csh")) || (!strcmp(shell, "tcsh")));
}

void use_package(char* name)
{
   package_t* package;
   list_node* node;
   int got_one = 0;

   DEBUG("# using package %s...\n", name);
      
   for (node = head(the_packages) ; node ; node = next(node))
   {
      package = (package_t*) get_value(node);

      if (package_matches(package, name, the_host_info.machine,
			  the_host_info.sysname, the_host_info.release,
                          the_host_info.nodename))
      {
	 add_package(package);
         got_one = 1;
      }
   }

   if ((!silent) && (!got_one))
      fprintf(stderr,
	      "usepackage: no match for package `%s' on this host.\n",
	      name);
}

void add_package(package_t* package)
{
   variable_t* evar;
   variable_t* vvar;
   list_node* enode;
   list_node* vnode;
   int got_one;
   
   for (vnode = head(package->variables) ; vnode ; vnode = next(vnode))
   {
      vvar = get_value(vnode);
      got_one = 0;
      for (enode = head(the_environment) ; enode ; enode = next(enode))
      {
         evar = get_value(enode);
         if (!strcmp(vvar->name, evar->name))
         {
            set_value(enode, update_var(evar, vvar));
            got_one = 1;
            break;
         }
      }
      if (!got_one)
      {
         enode = get_into_env(vvar);
         evar = get_value(enode);
	 set_value(enode, update_var(evar, vvar));
      }
   }
}

void use_group(group_t* group)
{
   list_node* node;

   DEBUG("# (expanding group `%s')\n", group->name);

   for (node = list_tail(group->packages) ; node ; node = previous(node))
      use_package((char*) get_value(node));
}

group_t* get_group(char* name)
{
   list_node* node;
   group_t* group;

   for (node = head(the_groups) ; node ; node = next(node))
   {
      group = (group_t*) get_value(node);
      if (!strcasecmp(name, group->name))
         return(group);
   }

   return(NULL);
}

void print_env(void)
{
   list_node* node;
   variable_t* var;

   DEBUG("# dumping environment...\n");

   for (node = head(the_environment) ; node ; node = next(node))
   {
      var = (variable_t*) get_value(node);
      if (csh_user)
         printf("setenv %s ", var->name);
      else
         printf("%s=", var->name);

      switch (var->type)
      {
         case VAR_LIT_SET:
            printf("\"%s\"", var->literal);
            break;

         case VAR_PATH_ADD:
         case VAR_PATH_SET:
            print_path(var->pathlist);
            break;

         default:
            break;
      }

      if (!csh_user)
         printf(" ;\nexport %s ;\n", var->name);
      else
         printf(" ;\n");
   }
}

void print_path(linked_list* pathlist)
{
   list_node* node;

   for (node = head(pathlist) ; node ; node = next(node))
   {
      if (next(node))
	 printf("%s:", (char*) get_value(node));
      else
	 printf("%s", (char*) get_value(node));
   }
}

list_node* get_into_env(variable_t* var)
{
   variable_t* env_var;

   env_var = new(variable_t);
   env_var->name = var->name;
   env_var->type = var->type;

   switch(var->type)
   {
      case VAR_LIT_SET:
         env_var->literal = NULL;
         break;

      case VAR_PATH_SET:
         env_var->pathlist = NULL;
         break;

      case VAR_PATH_ADD:
         env_var->pathlist = make_pathlist(getenv(var->name));
         break;

      default:
         break;
   }

   add_to_tail(the_environment, env_var);
   return(list_tail(the_environment));
}

linked_list* make_pathlist(char* path_string)
{
   linked_list* list;
   int start,i,end;
   char* path;

   list = new_list();

   if (!path_string)
      return(list);

   start = 0;
   i = 0;
   end = strlen(path_string);

   if (!end)
      return(list);

   while (i <= end)
   {
      if ((path_string[i] == ':') || (path_string[i] == '\0'))
      {
         if (i > start)
         {
            path = (char*) malloc((i - start) + 1);
            strncpy(path, path_string + start, i - start);
            path[i - start] = '\0';
         }
         add_to_tail(list, path);
         start = i + 1;
      }

      i++;
   }

   return(list);
}

variable_t* update_var(variable_t* evar, variable_t* vvar)
{
   switch (vvar->type)
   {
      case VAR_LIT_SET:
         evar->literal = vvar->literal;
         evar->type = VAR_LIT_SET;
         break;

      case VAR_PATH_SET:
         evar->pathlist = vvar->pathlist;
         evar->type = VAR_PATH_SET;
         break;

      case VAR_PATH_ADD:
         switch (evar->type)
         {
            case VAR_LIT_SET:
	       evar->pathlist = merge_paths(make_pathlist(evar->literal),
                                            vvar->pathlist);
               break;

            case VAR_PATH_SET:
            case VAR_PATH_ADD:
	       evar->pathlist = merge_paths(evar->pathlist, vvar->pathlist);
	       break;

	    default:
	       break;
         }
         evar->type = VAR_PATH_ADD;
         break;

      default:
         break;
   }

   return(evar);
}

linked_list* merge_paths(linked_list* elist, linked_list* vlist)
{
   list_node* enode;
   list_node* vnode;
   char* vpath;

   for (vnode = list_tail(vlist) ; vnode ; vnode = previous(vnode))
   {
      vpath = (char*) get_value(vnode);

      for (enode = head(elist) ; enode ; enode = next(enode))
         if (!strcmp(vpath, (char*) get_value(enode)))
         {
            remove_node(elist, enode, 0);
            break;
         }

      add_to_head(elist, get_value(vnode));
   }

   return(elist);
}


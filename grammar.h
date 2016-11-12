/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_GRAMMAR_H_INCLUDED
# define YY_YY_GRAMMAR_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    COLON = 258,
    SEMICOLON = 259,
    COMMA = 260,
    EQUALS = 261,
    PLUSEQUALS = 262,
    EQUALSPLUS = 263,
    QEQUALS = 264,
    QPLUSEQUALS = 265,
    LITERAL = 266,
    NAME = 267,
    PATH = 268,
    LEFTPAREN = 269,
    RIGHTPAREN = 270,
    PREFIX = 271,
    WILDCARD = 272,
    ASSIGN = 273,
    WITH = 274,
    BEGIN_ANNOTATE = 275,
    END_ANNOTATE = 276,
    SCRIPT = 277,
    ALIAS = 278
  };
#endif
/* Tokens.  */
#define COLON 258
#define SEMICOLON 259
#define COMMA 260
#define EQUALS 261
#define PLUSEQUALS 262
#define EQUALSPLUS 263
#define QEQUALS 264
#define QPLUSEQUALS 265
#define LITERAL 266
#define NAME 267
#define PATH 268
#define LEFTPAREN 269
#define RIGHTPAREN 270
#define PREFIX 271
#define WILDCARD 272
#define ASSIGN 273
#define WITH 274
#define BEGIN_ANNOTATE 275
#define END_ANNOTATE 276
#define SCRIPT 277
#define ALIAS 278

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 64 "grammar.y" /* yacc.c:1909  */
 
   linked_list* list;
   char* string;
   package_t* package;
   variable_t* variable;
   match_t* match;
   group_t* group;
   annotation_t* annotation;
   script_t* script;
   alias_t* alias;

#line 112 "grammar.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_GRAMMAR_H_INCLUDED  */

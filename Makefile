
# Makefile

GROUP = contrib
DEST = /usr/local/contrib

INSTALL_EXEC = install -m 771 -g $(GROUP)
INSTALL_SCRIPT = install -m 775 -g $(GROUP)
INSTALL_FILE = install -m 664 -g $(GROUP)
INSTALL_DIR = install -m 775 -d -g $(GROUP)
M4 = m4
STRIP = strip
BISON = bison -d
FLEX = flex -Cf -i
MV = mv
RM = rm -f
CC = gcc -O2 -DMAIN_PACKAGE_FILE=\"$(DEST)/lib/usepackage/packages.master\"
LINK = gcc


all: README usepackage use.bsh use.csh use.ksh use.zsh

install: all
	$(INSTALL_DIR) $(DEST)/lib/usepackage
	$(INSTALL_FILE) README $(DEST)/README
	$(INSTALL_SCRIPT) use.bsh $(DEST)/lib/usepackage/use.bsh
	$(INSTALL_SCRIPT) use.csh $(DEST)/lib/usepackage/use.csh
	$(INSTALL_SCRIPT) use.ksh $(DEST)/lib/usepackage/use.ksh
	$(INSTALL_SCRIPT) use.zsh $(DEST)/lib/usepackage/use.zsh
	$(STRIP) usepackage
	$(INSTALL_EXEC) usepackage $(DEST)/bin/usepackage
	for package in packages.* ;\
	do \
		$(INSTALL_FILE) $$package $(DEST)/lib/usepackage/$$package ;\
	done

OBJECTS = usepackage.o grammar.o scanner.o linked_list.o utils.o match.o

usepackage: $(OBJECTS)
	$(LINK) -o usepackage $(OBJECTS)

grammar.c grammar.h: grammar.y
	$(BISON) grammar.y
	$(MV) grammar.tab.c grammar.c
	$(MV) grammar.tab.h grammar.h

scanner.c: scanner.l
	$(FLEX) scanner.l
	$(MV) lex.yy.c scanner.c

clean:
	$(RM)	README usepackage use.bsh use.csh use.ksh use.zsh \
		*.o scanner.c grammar.c grammar.h

%.o: %.c
	$(CC) -c $*.c

%: %.in
	$(M4) -DINSTALL_DIR=$(DEST) $*.in > $*


linked_list.o: linked_list.h
match.o: packages.h linked_list.h
usepackage.o: packages.h linked_list.h utils.h version.h
grammar.o: packages.h linked_list.h utils.h
scanner.o: packages.h grammar.h


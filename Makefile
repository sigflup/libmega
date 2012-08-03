
INSTALL_PREFIX=/usr

SHELL=/bin/sh
CFLAGS= -m68000
CC = gcc
LD = ld
OBJCOPY= objcopy
TOOLS_PREFIX = m68k-elf-

HEADS = bottom.h lib.h libmega.h mega.h table.h
OBJS = divsi3.o udivsi3.o mulsi3.o modsi3.o lib.o bottom.o table.o

libmega.a: ${OBJS}
	rm -rf libmega.a
	${TOOLS_PREFIX}ar -q -v libmega.a ${OBJS}
	${TOOLS_PREFIX}ranlib libmega.a

table.o: table.c table.h
	${TOOLS_PREFIX}${CC} ${CFLAGS} -c table.c

lib.o: lib.c lib.h
	${TOOLS_PREFIX}${CC} ${CFLAGS} -c lib.c

bottom.o: bottom.c bottom.h
	${TOOLS_PREFIX}${CC} ${CFLAGS} -c bottom.c

divsi3.o: divsi3.s
	${TOOLS_PREFIX}${CC} ${CFLAGS} -c divsi3.s

udivsi3.o: udivsi3.s
	${TOOLS_PREFIX}${CC} ${CFLAGS} -c udivsi3.s

modsi3.o: modsi3.s
	${TOOLS_PREFIX}${CC} ${CFLAGS} -c modsi3.s


mulsi3.o: mulsi3.s
	${TOOLS_PREFIX}${CC} ${CFLAGS} -c mulsi3.s

install: ${HEADS} libmega.a
	rm -rf ${INSTALL_PREFIX}/include/libmega
	mkdir ${INSTALL_PREFIX}/include/libmega
	cp -f libmega.a ${INSTALL_PREFIX}/lib
	cp -f bottom.h ${INSTALL_PREFIX}/include/libmega
	cp -f lib.h ${INSTALL_PREFIX}/include/libmega
	cp -f libmega.h ${INSTALL_PREFIX}/include/libmega
	cp -f mega.h ${INSTALL_PREFIX}/include/libmega
	cp -f table.h ${INSTALL_PREFIX}/include/libmega
clean:
	rm -f ${OBJS} libmega.a 

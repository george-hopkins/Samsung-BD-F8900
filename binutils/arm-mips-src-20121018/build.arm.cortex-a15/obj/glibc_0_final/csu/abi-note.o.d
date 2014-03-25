$(common-objpfx)csu/abi-note.o: \
 ../ports/sysdeps/arm/eabi/abi-note.S ../include/libc-symbols.h \
 $(common-objpfx)config.h \
 ../sysdeps/wordsize-32/symbol-hacks.h ../csu/abi-note.S \
 $(common-objpfx)csu/abi-tag.h

../include/libc-symbols.h:

$(common-objpfx)config.h:

../sysdeps/wordsize-32/symbol-hacks.h:

../csu/abi-note.S:

$(common-objpfx)csu/abi-tag.h:

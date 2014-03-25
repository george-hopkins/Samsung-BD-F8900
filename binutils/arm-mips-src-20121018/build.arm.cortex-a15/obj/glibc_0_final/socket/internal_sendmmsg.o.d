$(common-objpfx)socket/internal_sendmmsg.o: \
 ../ports/sysdeps/unix/sysv/linux/arm/eabi/internal_sendmmsg.S \
 ../include/libc-symbols.h \
 $(common-objpfx)config.h \
 ../sysdeps/wordsize-32/symbol-hacks.h

../include/libc-symbols.h:

$(common-objpfx)config.h:

../sysdeps/wordsize-32/symbol-hacks.h:

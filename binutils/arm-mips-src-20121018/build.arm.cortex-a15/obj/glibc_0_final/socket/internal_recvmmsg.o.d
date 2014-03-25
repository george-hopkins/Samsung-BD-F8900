$(common-objpfx)socket/internal_recvmmsg.o: \
 ../ports/sysdeps/unix/sysv/linux/arm/eabi/internal_recvmmsg.S \
 ../include/libc-symbols.h \
 $(common-objpfx)config.h \
 ../sysdeps/wordsize-32/symbol-hacks.h

../include/libc-symbols.h:

$(common-objpfx)config.h:

../sysdeps/wordsize-32/symbol-hacks.h:

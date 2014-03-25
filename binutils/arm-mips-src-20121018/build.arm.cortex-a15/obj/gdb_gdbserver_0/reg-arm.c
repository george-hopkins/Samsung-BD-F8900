/* *INDENT-OFF* */ /* THIS FILE IS GENERATED */

/* A register protocol for GDB, the GNU debugger.
   Copyright 2001, 2002 Free Software Foundation, Inc.

   This file is part of GDB.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* This file was created with the aid of ``regdat.sh'' and ``/home6/daewon2/Release/glibc/SP/SP_Linux/LP_Linux_Prj/REL/VDLinux.3.x.2012/toolchain/src/arm-mips-src-20121018/build.arm.cortex-a15/sources/gdb_gdbserver_0/gdb/gdbserver/../regformats/reg-arm.dat''.  */

#include "server.h"
#include "regdef.h"

struct reg regs_arm[] = {
  { "r0", 0, 32 },
  { "r1", 32, 32 },
  { "r2", 64, 32 },
  { "r3", 96, 32 },
  { "r4", 128, 32 },
  { "r5", 160, 32 },
  { "r6", 192, 32 },
  { "r7", 224, 32 },
  { "r8", 256, 32 },
  { "r9", 288, 32 },
  { "r10", 320, 32 },
  { "r11", 352, 32 },
  { "r12", 384, 32 },
  { "sp", 416, 32 },
  { "lr", 448, 32 },
  { "pc", 480, 32 },
  { "f0", 512, 96 },
  { "f1", 608, 96 },
  { "f2", 704, 96 },
  { "f3", 800, 96 },
  { "f4", 896, 96 },
  { "f5", 992, 96 },
  { "f6", 1088, 96 },
  { "f7", 1184, 96 },
  { "fps", 1280, 32 },
  { "cpsr", 1312, 32 },
};

const char *expedite_regs_arm[] = { "r11", "sp", "pc", 0 };
const char *xmltarget_arm = "@<target>\
<architecture>arm</architecture>\
</target>";

void
init_registers_arm ()
{
    set_register_cache (regs_arm,
			sizeof (regs_arm) / sizeof (regs_arm[0]));
    gdbserver_expedite_regs = expedite_regs_arm;
    gdbserver_xmltarget = xmltarget_arm;
}

TARGET = mschange
OBJS = mschange.o exports.o imports.o

# BUILD_PRX = 1
# PRX_EXPORTS = exports.exp
INCDIR = 
CFLAGS = -O2 -G0 -Wall -DENABLE_FILELOG
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)
LIBDIR =
LIBS =

USE_KERNEL_LIBC = 1
USE_KERNEL_LIBS = 1
PSP_FW_VERSION = 500

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build_prx.mak 
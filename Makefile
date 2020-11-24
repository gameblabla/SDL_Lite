SRCDIR		= ./src ./src/cpuinfo ./src/events ./src/file ./src/filesystem/unix ./src/haptic -
SRCDIR		+= ./src/joystick ./src/joystick/linux ./src/loadso/dlopen ./src/power ./src/power/linux ./src/stdlib
SRCDIR		+= ./src/thread ./src/thread/pthread ./src/timer ./src/timer/unix ./src/video
SRCDIR      += ./src/audio ./src/cdrom ./src/haptic/dummy

ifeq ($(STATIC_ENABLED), 1)
TARGET = libSDL.a
else
TARGET = libSDL.so
endif

ifeq ($(ALSA), 1)
SRCDIR 		+= ./src/audio/alsa
CFLAGS		+= -DSDL_AUDIO_DRIVER_ALSA
ifeq ($(STATIC_ENABLED), 0)
else
CFLAGS		+= -DSDL_AUDIO_DRIVER_ALSA_DYNAMIC
endif
else ifeq ($(PULSE), 1)
SRCDIR		+= ./src/audio/pulse
CFLAGS		+= -DSDL_AUDIO_DRIVER_PULSEAUDIO
ifeq ($(STATIC_ENABLED), 0)
else
CFLAGS		+= -DSDL_AUDIO_DRIVER_PULSEAUDIO_DYNAMIC
endif
else ifeq ($(OSS), 1)
SRCDIR		+= ./src/audio/oss
CFLAGS		+= -DSDL_AUDIO_DRIVER_OSS
endif

ifeq ($(CDROM), 1)
SRCDIR 		+= ./src/cdrom/linux
CFLAGS		+= -DSDL_CDROM_LINUX
else
CFLAGS		+= -DSDL_CDROM_DISABLED -DSDL_CDROM_DISABLED
SRCDIR 		+= ./src/cdrom/dummy
endif

ifeq ($(FBDEV), 1)
CFLAGS		+= -DSDL_VIDEO_DRIVER_FBCON
SRCDIR 		+= ./src/video/fbcon
else ifeq ($(KMSDRM), 1)
CFLAGS		+= -DSDL_VIDEO_DRIVER_KMSDRM
SRCDIR		+= ./src/video/kmsdrm
endif

ifeq ($(NEON_A64), 1)
SRCDIR 		+= ./src/video/arm64
#CFLAGS		+= -DSDL_ARM_NEON_BLITTERS=1
endif

ifeq ($(NEON), 1)
SRCDIR 		+= ./src/video/arm
CFLAGS		+= -DSDL_ARM_NEON_BLITTERS=1
endif

CFLAGS		+= -D_GNU_SOURCE -DHAVE_LIBC -D_REENTRANT -I.
CFLAGS		+= -Iinclude -std=gnu99 $(shell $(PKG_CONFIG) --cflags libdrm)

VPATH		= $(SRCDIR)
SRC_S		= $(foreach dir, $(SRCDIR), $(wildcard $(dir)/*.S))
SRC_C		= $(foreach dir, $(SRCDIR), $(wildcard $(dir)/*.c))
OBJ_S		= $(notdir $(patsubst %.S, %.o, $(SRC_S)))
OBJ_C		= $(notdir $(patsubst %.c, %.o, $(SRC_C)))
OBJS		= $(OBJ_S) $(OBJ_C)

all: $(TARGET)

# Rules to make executable
$(TARGET): $(OBJS)  
ifeq ($(STATIC_ENABLED), 1)
	$(AR) rcs $(TARGET) $^
else
	$(CC) -shared $(CFLAGS) $^ -o $@ -ldl -ludev -ldrm $(LDFLAGS)
endif

$(OBJ_C) : %.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<
	
$(OBJ_S) : %.o : %.S
	$(CC) $(CFLAGS) -c -o $@ $<

install:
	cp $(TARGET) $(DESTDIR)/$(TARGET)
	cp sdl.pc $(DESTDIR)$(PREFIX)/lib/pkgconfig
	cp sdl-config $(DESTDIR)$(PREFIX)/bin/sdl-config
	
install-headers:
	mkdir -p $(DESTDIR)$(PREFIX)/include/SDL
	cp include/*.h $(DESTDIR)$(PREFIX)/include/SDL

install-lib:
	cp $(TARGET) $(DESTDIR)$(PREFIX)/lib/

install: $(TARGET) install-headers install-lib

clean:
	rm -f $(TARGET) *.o *.a *.so

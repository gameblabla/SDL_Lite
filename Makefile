# Makefile to build and install the SDL library

top_builddir    = .
srcdir          = .
objects         = build
depend          = build-deps
prefix          = /usr
exec_prefix     = ${prefix}
bindir          = ${exec_prefix}/bin
libdir          = ${exec_prefix}/lib
includedir      = ${prefix}/include
datarootdir     = ${prefix}/share
datadir         = ${datarootdir}
mandir          = ${datarootdir}/man
auxdir          = build-scripts
distpath        = $(srcdir)/..
distdir         = SDL-1.2.16
distfile        = $(distdir).tar.gz


SHELL           = /bin/sh
CC              = gcc
INCLUDE         = -I./include
CFLAGS          = -g -O2 
EXTRA_CFLAGS    = -I./include -D_GNU_SOURCE=1 -DHAVE_LINUX_VERSION_H  -D_REENTRANT -I/usr/include/libdrm -D_REENTRANT -fvisibility=hidden -Wall
LDFLAGS         = 
EXTRA_LDFLAGS   =  -lm  -lasound -lm -ldl -lpthread -lpulse-simple -lpulse -ldrm -ludev -lpthread -ldl -lrt
LIBTOOL         = $(SHELL) $(top_builddir)/libtool
INSTALL         = /bin/install -c
AS              = @AS@
AR              = ar
RANLIB          = ranlib

TARGET          = libSDL.la
SOURCES         =  ./src/*.c ./src/audio/*.c ./src/cdrom/*.c ./src/cpuinfo/*.c ./src/events/*.c ./src/file/*.c ./src/power/*.c ./src/stdlib/*.c ./src/thread/*.c ./src/timer/*.c ./src/video/*.c ./src/joystick/*.c ./src/filesystem/unix/*.c ./src/power/linux/*.c ./src/audio/oss/*.c ./src/audio/alsa/*.c ./src/audio/pulse/*.c ./src/audio/dummy/*.c ./src/video/kmsdrm/*.c ./src/video/dummy/*.c ./src/thread/pthread/SDL_systhread.c ./src/thread/pthread/SDL_syssem.c ./src/thread/pthread/SDL_sysmutex.c ./src/thread/pthread/SDL_syscond.c ./src/loadso/dlopen/*.c ./src/joystick/linux/*.c ./src/cdrom/linux/*.c ./src/timer/unix/*.c
OBJECTS         = $(objects)/SDL.lo $(objects)/SDL_error.lo $(objects)/SDL_fatal.lo $(objects)/SDL_log.lo $(objects)/SDL_audio.lo $(objects)/SDL_audiocvt.lo $(objects)/SDL_audiodev.lo $(objects)/SDL_mixer.lo $(objects)/SDL_mixer_arm.lo $(objects)/SDL_wave.lo $(objects)/SDL_cdrom.lo $(objects)/SDL_cpuinfo.lo $(objects)/SDL_active.lo $(objects)/SDL_events.lo $(objects)/SDL_expose.lo $(objects)/SDL_keyboard.lo $(objects)/SDL_mouse.lo $(objects)/SDL_quit.lo $(objects)/SDL_resize.lo $(objects)/SDL_rwops.lo $(objects)/SDL_power.lo $(objects)/SDL_getenv.lo $(objects)/SDL_iconv.lo $(objects)/SDL_malloc.lo $(objects)/SDL_qsort.lo $(objects)/SDL_stdlib.lo $(objects)/SDL_string.lo $(objects)/SDL_thread.lo $(objects)/SDL_timer.lo $(objects)/SDL_RLEaccel.lo $(objects)/SDL_blit.lo $(objects)/SDL_blit_0.lo $(objects)/SDL_blit_1.lo $(objects)/SDL_blit_A.lo $(objects)/SDL_blit_N.lo $(objects)/SDL_bmp.lo $(objects)/SDL_cursor.lo $(objects)/SDL_gamma.lo $(objects)/SDL_pixels.lo $(objects)/SDL_stretch.lo $(objects)/SDL_surface.lo $(objects)/SDL_video.lo $(objects)/SDL_yuv.lo $(objects)/SDL_yuv_sw.lo $(objects)/SDL_joystick.lo $(objects)/SDL_sysfilesystem.lo $(objects)/SDL_syspower.lo $(objects)/SDL_dmaaudio.lo $(objects)/SDL_dspaudio.lo $(objects)/SDL_alsa_audio.lo $(objects)/SDL_pulseaudio.lo $(objects)/SDL_dummyaudio.lo $(objects)/SDL_kmsdrmcolordef.lo $(objects)/SDL_kmsdrmevents.lo $(objects)/SDL_kmsdrmmisc.lo $(objects)/SDL_kmsdrmvideo.lo $(objects)/SDL_nullevents.lo $(objects)/SDL_nullmouse.lo $(objects)/SDL_nullvideo.lo $(objects)/SDL_systhread.lo $(objects)/SDL_syssem.lo $(objects)/SDL_sysmutex.lo $(objects)/SDL_syscond.lo $(objects)/SDL_sysloadso.lo $(objects)/SDL_sysjoystick.lo $(objects)/SDL_syscdrom.lo $(objects)/SDL_systimer.lo

SDLMAIN_TARGET  = libSDLmain.la
SDLMAIN_SOURCES = ./src/main/dummy/*.c
SDLMAIN_OBJECTS = $(objects)/SDL_dummy_main.lo
SDLMAIN_LDFLAGS = -static

DIST = \
	acinclude \
	build-scripts \
	configure.ac \
	include \
	Makefile.in \
	sdl-config.in \
	sdl.m4 \
	sdl.pc.in \
	src

HDRS = \
	begin_code.h \
	close_code.h \
	SDL.h \
	SDL_active.h \
	SDL_audio.h \
	SDL_byteorder.h \
	SDL_cdrom.h \
	SDL_config.h \
	SDL_config_minimal.h \
	SDL_copying.h \
	SDL_cpuinfo.h \
	SDL_endian.h \
	SDL_error.h \
	SDL_events.h \
	SDL_filesystem.h \
	SDL_getenv.h \
	SDL_haptic.h \
	SDL_joystick.h \
	SDL_keyboard.h \
	SDL_keysym.h \
	SDL_loadso.h \
	SDL_log.h \
	SDL_main.h \
	SDL_mouse.h \
	SDL_mutex.h \
	SDL_name.h \
	SDL_opengles_ext.h \
	SDL_opengles.h \
	SDL_opengl.h \
	SDL_pixels.h \
	SDL_platform.h \
	SDL_power.h \
	SDL_quit.h \
	SDL_revision.h \
	SDL_rwops.h \
	SDL_stdinc.h \
	SDL_syswm.h \
	SDL_thread.h \
	SDL_timer.h \
	SDL_types.h \
	SDL_version.h \
	SDL_video.h

LT_AGE      = 11
LT_CURRENT  = 11
LT_RELEASE  = 1.2
LT_REVISION = 5
LT_LDFLAGS  = -no-undefined -rpath $(libdir) -release $(LT_RELEASE) -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)

ifeq ($(VERBOSE), 0)
	Q := @
endif

all: $(srcdir)/configure Makefile $(objects) $(objects)/$(TARGET) $(objects)/$(SDLMAIN_TARGET)

$(srcdir)/configure: $(srcdir)/configure.ac
	@echo "Warning, configure is out of date, please re-run autogen.sh"

Makefile: $(srcdir)/Makefile.in
	$(Q)$(SHELL) config.status $@

$(objects):
	$(Q)$(SHELL) $(auxdir)/mkinstalldirs $@

.PHONY: all depend install install-bin install-hdrs install-lib install-data install-man uninstall uninstall-bin uninstall-hdrs uninstall-lib uninstall-data uninstall-man clean distclean dist
depend:
	@SOURCES="$(SOURCES) $(SDLMAIN_SOURCES)" INCLUDE="$(INCLUDE)" output="$(depend)" \
	$(Q)$(SHELL) $(auxdir)/makedep.sh

include $(depend)

$(objects)/$(TARGET): $(OBJECTS)
	$(Q)$(LIBTOOL) --mode=link $(CC) -o $@ $^ $(LDFLAGS) $(EXTRA_LDFLAGS) $(LT_LDFLAGS)

$(objects)/$(SDLMAIN_TARGET): $(SDLMAIN_OBJECTS)
	$(Q)$(LIBTOOL) --mode=link $(CC) -o $@ $^ $(LDFLAGS) $(EXTRA_LDFLAGS) $(LT_LDFLAGS) $(SDLMAIN_LDFLAGS)

install: all install-bin install-hdrs install-lib install-data
install-bin:
	$(Q)$(SHELL) $(auxdir)/mkinstalldirs $(DESTDIR)$(bindir)
	$(Q)$(INSTALL) -m 755 sdl-config $(DESTDIR)$(bindir)/sdl-config
install-hdrs:
	$(Q)$(SHELL) $(auxdir)/mkinstalldirs $(DESTDIR)$(includedir)/SDL
	for file in $(HDRS); do \
	    $(Q)$(INSTALL) -m 644 $(srcdir)/include/$$file $(DESTDIR)$(includedir)/SDL/$$file; \
	done
	$(Q)$(INSTALL) -m 644 include/SDL_config.h $(DESTDIR)$(includedir)/SDL/SDL_config.h
install-lib: $(objects) $(objects)/$(TARGET) $(objects)/$(SDLMAIN_TARGET)
	$(Q)$(SHELL) $(auxdir)/mkinstalldirs $(DESTDIR)$(libdir)
	$(Q)$(LIBTOOL) --mode=install $(INSTALL) $(objects)/$(TARGET) $(DESTDIR)$(libdir)/$(TARGET)
	$(Q)$(LIBTOOL) --mode=install $(INSTALL) $(objects)/$(SDLMAIN_TARGET) $(DESTDIR)$(libdir)/$(SDLMAIN_TARGET)
install-data:
	$(Q)$(SHELL) $(auxdir)/mkinstalldirs $(DESTDIR)$(datadir)/aclocal
	$(Q)$(INSTALL) -m 644 $(srcdir)/sdl.m4 $(DESTDIR)$(datadir)/aclocal/sdl.m4
	$(Q)$(SHELL) $(auxdir)/mkinstalldirs $(DESTDIR)$(libdir)/pkgconfig
	$(Q)$(INSTALL) -m 644 sdl.pc $(DESTDIR)$(libdir)/pkgconfig

uninstall: uninstall-bin uninstall-hdrs uninstall-lib uninstall-data
uninstall-bin:
	rm -f $(DESTDIR)$(bindir)/sdl-config
uninstall-hdrs:
	for file in $(HDRS); do \
	    rm -f $(DESTDIR)$(includedir)/SDL/$$file; \
	done
	rm -f $(DESTDIR)$(includedir)/SDL/SDL_config.h
	-rmdir $(DESTDIR)$(includedir)/SDL
uninstall-lib:
	$(LIBTOOL) --mode=uninstall rm -f $(DESTDIR)$(libdir)/$(TARGET)
	$(LIBTOOL) --mode=uninstall rm -f $(DESTDIR)$(libdir)/$(SDLMAIN_TARGET)
uninstall-data:
	rm -f $(DESTDIR)$(datadir)/aclocal/sdl.m4
	rm -f $(DESTDIR)$(libdir)/pkgconfig/sdl.pc

clean:
	rm -rf $(objects)
	if test -f test/Makefile; then (cd test; $(MAKE) $@); fi

distclean: clean
	rm -f Makefile include/SDL_config.h sdl-config
	rm -f config.status config.cache config.log libtool $(depend)
	rm -rf $(srcdir)/autom4te*
	rm -rf $(srcdir)/test/autom4te*
	find $(srcdir) \( \
	    -name '*~' -o \
	    -name '*.bak' -o \
	    -name '*.old' -o \
	    -name '*.rej' -o \
	    -name '*.orig' -o \
	    -name '.#*' \) \
	    -exec rm -f {} \;
	cp $(srcdir)/include/SDL_config.h.default $(srcdir)/include/SDL_config.h
	if test -f test/Makefile; then (cd test; $(MAKE) $@); fi

dist $(distfile):
	$(SHELL) $(auxdir)/mkinstalldirs $(distdir)
	tar cf - $(DIST) | (cd $(distdir); tar xf -)
	cp $(distdir)/include/SDL_config.h.default $(distdir)/include/SDL_config.h
	rm -rf `find $(distdir) -name .svn`
	rm -rf $(distdir)/test/autom4te*
	find $(distdir) \( \
	    -name '*~' -o \
	    -name '*.bak' -o \
	    -name '*.old' -o \
	    -name '*.rej' -o \
	    -name '*.orig' -o \
	    -name '.#*' \) \
	    -exec rm -f {} \;
	if test -f $(distdir)/test/Makefile; then (cd $(distdir)/test && make distclean); fi
	tar cvf - $(distdir) | gzip --best >$(distfile)
	rm -rf $(distdir)

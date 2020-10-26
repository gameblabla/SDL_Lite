/*******************************************************************************
 * Library       : SDLite 1.2.x
 * Purpose       : Low-level access to a framebuffer, audio output and HID.
 * Module        : Core
 * Project       : Redux for Embedded System
 * Description   : Stripped-down and optimized libraries for RISC processors
 * License       : GNU General Public License v3.0
 *******************************************************************************
 *
 * TinyRetroLabs and SDLite 1.2.x:
 * Copyright (c) 2019-2020 Marcus Andrade <boogermann@tinyretrolabs.org>
 *
 * Simple DirectMedia Layer and SDL:
 * Copyright (c) 1997-2012 Sam Lantinga <slouken@libsdl.org>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.
 * If not, see <https://www.gnu.org/licenses/gpl-3.0.html>.
 *
 ******************************************************************************/

#ifndef SDL_config_minimal_h_
#define SDL_config_minimal_h_

#include "SDL_platform.h"


#define SDL_HAS_64BIT_TYPE 1
/* Endianness */
#define SDL_BYTEORDER 1234

/* This is the minimal configuration that can be used to build SDL */
#define HAVE_LIBC 1
#if HAVE_LIBC

/* Useful headers */
#define HAVE_ALLOCA_H 1
#define HAVE_SYS_TYPES_H 1
#define HAVE_STDIO_H 1
#define STDC_HEADERS 1
#define HAVE_STDLIB_H 1
#define HAVE_STDARG_H 1
#define HAVE_MALLOC_H 1
#define HAVE_MEMORY_H 1
#define HAVE_STRING_H 1
#define HAVE_STRINGS_H 1
#define HAVE_INTTYPES_H 1
#define HAVE_STDINT_H 1
#define HAVE_CTYPE_H 1
#define HAVE_MATH_H 1
#define HAVE_ICONV_H 1
#define HAVE_SIGNAL_H 1
/* #undef HAVE_ALTIVEC_H */

/* C library functions */
#define HAVE_MALLOC 1
#define HAVE_CALLOC 1
#define HAVE_REALLOC 1
#define HAVE_FREE 1
#define HAVE_ALLOCA 1
#ifndef _WIN32 /* Don't use C runtime versions of these on Windows */
#define HAVE_GETENV 1
#define HAVE_PUTENV 1
#define HAVE_UNSETENV 1
#endif
#define HAVE_QSORT 1
#define HAVE_ABS 1
#define HAVE_BCOPY 1
#define HAVE_MEMSET 1
#define HAVE_MEMCPY 1
#define HAVE_MEMMOVE 1
#define HAVE_MEMCMP 1
#define HAVE_STRLEN 1
/* #undef HAVE_STRLCPY */
/* #undef HAVE_STRLCAT */
#define HAVE_STRDUP 1
/* #undef HAVE__STRREV */
/* #undef HAVE__STRUPR */
/* #undef HAVE__STRLWR */
/* #undef HAVE_INDEX */
/* #undef HAVE_RINDEX */
#define HAVE_STRCHR 1
#define HAVE_STRRCHR 1
#define HAVE_STRSTR 1
/* #undef HAVE_ITOA */
/* #undef HAVE__LTOA */
/* #undef HAVE__UITOA */
/* #undef HAVE__ULTOA */
#define HAVE_STRTOL 1
#define HAVE_STRTOUL 1
/* #undef HAVE__I64TOA */
/* #undef HAVE__UI64TOA */
#define HAVE_STRTOLL 1
#define HAVE_STRTOULL 1
#define HAVE_STRTOD 1
#define HAVE_ATOI 1
#define HAVE_ATOF 1
#define HAVE_STRCMP 1
#define HAVE_STRNCMP 1
/* #undef HAVE__STRICMP */
#define HAVE_STRCASECMP 1
/* #undef HAVE__STRNICMP */
#define HAVE_STRNCASECMP 1
#define HAVE_SSCANF 1
#define HAVE_SNPRINTF 1
#define HAVE_VSNPRINTF 1
#define HAVE_ICONV 1
#define HAVE_SIGACTION 1
#define HAVE_SA_SIGACTION 1
#define HAVE_SETJMP 1
#define HAVE_NANOSLEEP 1
#define HAVE_CLOCK_GETTIME 1
#define HAVE_GETPAGESIZE 1
#define HAVE_MPROTECT 1
#define HAVE_SEM_TIMEDWAIT 1

#else
/* We may need some replacement for stdarg.h here */
#include <stdarg.h>
#endif /* HAVE_LIBC */

#if !defined(int8_t)
typedef signed char int8_t;
#endif

#if !defined(uint8_t)
typedef unsigned char uint8_t;
#endif

#if !defined(int16_t)
typedef signed short int16_t;
#endif

#if !defined(uint16_t)
typedef unsigned short uint16_t;
#endif

#if !defined(int32_t)
typedef signed int int32_t;
#endif

#if !defined(uint32_t)
typedef unsigned int uint32_t;
#endif

#if !defined(uint32_t)
typedef unsigned int uint32_t;
#endif

#if !defined(uintptr_t)
typedef unsigned long uintptr_t;
#endif

/* Enable the dummy audio driver (src/audio/dummy/\*.c) */
//#define SDL_AUDIO_DRIVER_DUMMY    1

/* Enable the stub cdrom driver (src/cdrom/dummy/\*.c) */
//#define SDL_CDROM_DISABLED    1

/* Enable the stub joystick driver (src/joystick/dummy/\*.c) */
//#define SDL_JOYSTICK_DISABLED    1

/* Enable the stub shared object loader (src/loadso/dummy/\*.c) */
//#define SDL_LOADSO_DISABLED    1

/* Enable the stub thread support (src/thread/generic/\*.c) */
//#define SDL_THREADS_DISABLED    1

/* Enable the stub timer support (src/timer/dummy/\*.c) */
//#define SDL_TIMERS_DISABLED    1

/* Enable the dummy video driver (src/video/dummy/\*.c) */
//#define SDL_VIDEO_DRIVER_DUMMY    1

/*
#define SDL_AUDIO_DRIVER_ALSA 1
#define SDL_AUDIO_DRIVER_ALSA_DYNAMIC 1
#define SDL_AUDIO_DRIVER_PULSE 1
#define SDL_AUDIO_DRIVER_PULSE_DYNAMIC 1
#define SDL_AUDIO_DRIVER_OSS 1
*/
#define SDL_INPUT_LINUXEV 1
#define SDL_JOYSTICK_LINUX 1
#define SDL_LOADSO_DLOPEN 1
#define SDL_THREAD_PTHREAD 1
#define SDL_THREAD_PTHREAD_RECURSIVE_MUTEX 1
#define SDL_TIMER_UNIX 1

/*#define SDL_VIDEO_DRIVER_FBCON 1*/
/*#define SDL_VIDEO_DRIVER_KMSDRM 1*/

#define SDL_VIDEO_DISABLE_SCREENSAVER 1

/* Enable assembly routines */
#define SDL_ASSEMBLY_ROUTINES 1

/*#define SDL_ARM_NEON_BLITTERS 1*/

#endif /* SDL_config_minimal_h_ */

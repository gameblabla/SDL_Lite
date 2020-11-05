/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2012 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    Sam Lantinga
    slouken@libsdl.org
*/

#include "SDL_config.h"

#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include <go2/display.h>

#include "SDL_endian.h"
#include "SDL_timer.h"
#include "SDL_thread.h"
#include "SDL_video.h"
#include "SDL_mouse.h"
#include "../SDL_sysvideo.h"
#include "../SDL_pixels_c.h"
#include "../../events/SDL_events_c.h"

#include "SDL_kmsdrmvideo.h"
#include "SDL_kmsdrmevents_c.h"
#include "SDL_kmsdrmcolordef.h"

static go2_display_t  *display = NULL;
static go2_presenter_t*presenter = NULL;
static go2_surface_t  *fbsurface = NULL;

#define      HW_LCD_WIDTH           480
#define      HW_LCD_HEIGHT          320

/* Initialization/Query functions */
static int KMSDRM_VideoInit(_THIS, SDL_PixelFormat *vformat);
static SDL_Rect **KMSDRM_ListModes(_THIS, SDL_PixelFormat *format, Uint32 flags);
static SDL_Surface *KMSDRM_SetVideoMode(_THIS, SDL_Surface *current, int width, int height, int bpp, Uint32 flags);
static void KMSDRM_VideoQuit(_THIS);

/* etc. */
static void KMSDRM_UpdateRects(_THIS, int numrects, SDL_Rect *rects);

/* NSP driver bootstrap functions */

static int KMSDRM_Available(void)
{
	return(1);
}

static void KMSDRM_DeleteDevice(SDL_VideoDevice *device)
{
	SDL_free(device->hidden);
	SDL_free(device);
}

static void KMSDRM_FreeHWSurface(_THIS, SDL_Surface *surface)
{
	return;
}


static int KMSDRM_VideoInit(_THIS, SDL_PixelFormat *vformat)
{
	this->info.current_w = HW_LCD_WIDTH;
	this->info.current_h = HW_LCD_HEIGHT;

	vformat->BitsPerPixel = 16;
	vformat->BytesPerPixel = 2;
	vformat->Rmask = KMSDRM_RMASK16;
	vformat->Gmask = KMSDRM_GMASK16;
	vformat->Bmask = KMSDRM_BMASK16;
	
	KMSDRM_InitInput(this);
	
	return(0);
}

static SDL_Rect **KMSDRM_ListModes(_THIS, SDL_PixelFormat *format, Uint32 flags)
{
	return (SDL_Rect **) -1;
}

static SDL_Surface *KMSDRM_SetVideoMode(_THIS, SDL_Surface *current,
				     int width, int height, int bpp, Uint32 flags)
{
	int format_bpp = DRM_FORMAT_RGB565;
	const drm_color_def *color_def = get_drm_color_def(bpp, flags);
	if ( !color_def ) {
		SDL_SetError("Bad pixel format (%dbpp).\n", bpp);
		return NULL;
	}
	
	/* Set up the new mode framebuffer */
	switch (bpp)
	{
		default:
			format_bpp = DRM_FORMAT_RGB565;
		break;
		case 24:
			format_bpp = DRM_FORMAT_RGB888;
		break;
		case 32:
			format_bpp = DRM_FORMAT_ARGB8888;
		break;
	}

	if (display)
	{
		go2_display_destroy(display);
		display = NULL;
	}
		
	if (presenter)
	{
		go2_presenter_destroy(presenter);
		presenter = NULL;
	}
		
	if (fbsurface)
	{
		go2_surface_destroy(fbsurface);
		fbsurface = NULL;
	}
	
    display = go2_display_create();
    presenter = go2_presenter_create(display, DRM_FORMAT_RGB565, 0xff080808);
	fbsurface = go2_surface_create(display, width, height, format_bpp);
	
	/* Allocate the new pixel format for the screen */
	if ( ! SDL_ReallocFormat(current, bpp, color_def->r_mask, color_def->g_mask, color_def->b_mask, color_def->a_mask) ) {
		SDL_SetError("Couldn't allocate new pixel format for requested mode");
		return(NULL);
	}

	/* Set up the new mode framebuffer */
	current->flags = SDL_SWSURFACE;
	
	this->hidden->w = this->info.current_w = current->w = width;
	this->hidden->h = this->info.current_h = current->h = height;
	current->pitch = (bpp / 8) * current->w;
	current->pixels = (uint16_t*)go2_surface_map(fbsurface);

	/* We're done */
	return(current);
}

/*static int KMSDRM_FlipHWSurface(_THIS, SDL_Surface *surface)
{
	if (!fbsurface) return 0;
	
	surface->pixels = framebuffer;

	go2_presenter_post( presenter,
	fbsurface,
	0, 0, this->hidden->w, this->hidden->h,
	0, 0, 320, 480,
	GO2_ROTATION_DEGREES_270);
	
	return 1;
}*/

static void KMSDRM_UpdateRects(_THIS, int numrects, SDL_Rect *rects)
{
	/* This is needed given that it can still try to call this function even if the buffer is not initialized */
	if (!fbsurface) return;

	go2_presenter_post( presenter,
	fbsurface,
	0, 0, this->hidden->w, this->hidden->h,
	0, 0, HW_LCD_HEIGHT, HW_LCD_WIDTH,
	GO2_ROTATION_DEGREES_270);
	
	SDL_VideoSurface->pixels = (uint16_t*)go2_surface_map(fbsurface);
}


/* Note:  If we are terminated, this could be called in the middle of
   another SDL video routine -- notably UpdateRects.
*/
static void KMSDRM_VideoQuit(_THIS)
{
	if (display)
	{
		go2_display_destroy(display);
		display = NULL;
	}
		
	if (presenter)
	{
		go2_presenter_destroy(presenter);
		presenter = NULL;
	}
		
	if (fbsurface)
	{
		go2_surface_destroy(fbsurface);
		fbsurface = NULL;
	}
}


static SDL_VideoDevice *KMSDRM_CreateDevice(int devindex)
{
	SDL_VideoDevice *device;

	/* Initialize all variables that we clean on shutdown */
	device = (SDL_VideoDevice *)SDL_malloc(sizeof(SDL_VideoDevice));
	if ( device ) {
		SDL_memset(device, 0, (sizeof *device));
		device->hidden = (struct SDL_PrivateVideoData *)
				SDL_malloc((sizeof *device->hidden));
	}
	if ( (device == NULL) || (device->hidden == NULL) ) {
		SDL_OutOfMemory();
		if ( device ) {
			SDL_free(device);
		}
		return(0);
	}
	SDL_memset(device->hidden, 0, (sizeof *device->hidden));

	/* Set the function pointers */
	device->VideoInit = KMSDRM_VideoInit;
	device->ListModes = KMSDRM_ListModes;
	device->SetVideoMode = KMSDRM_SetVideoMode;
	device->UpdateRects = KMSDRM_UpdateRects;
	device->VideoQuit = KMSDRM_VideoQuit;
	device->InitOSKeymap = KMSDRM_InitOSKeymap;
	device->PumpEvents = KMSDRM_PumpEvents;
	
	//device->FlipHWSurface = KMSDRM_FlipHWSurface;
	//device->FreeHWSurface = KMSDRM_FreeHWSurface; // TODO:: Obvious

	device->free = KMSDRM_DeleteDevice;

	return device;
}

VideoBootStrap KMSDRM_bootstrap = {
	"libgo2", "libgo2 Driver video driver",
	KMSDRM_Available, KMSDRM_CreateDevice
};

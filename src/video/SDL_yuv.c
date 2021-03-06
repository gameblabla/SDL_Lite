// SPDX-License-Identifier: GPL-3.0-or-later
/*******************************************************************************
 * Library       : SDLite 1.2.x
 * Purpose       : Low-level access to a framebuffer, audio output and HID.
 * Module        : Core
 * Project       : Redux for Embedded System
 * Description   : Stripped-down and optimized libraries for RISC processors
 * License       : GNU General Public License v3.0
 *******************************************************************************
 *
 * Rætro and SDLite 1.2.x:
 * Copyright (c) 2019-2020 Marcus Andrade <marcus@raetro.org>
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
#include "SDL_config.h"

/** This is the implementation of the YUV video surface support */
#include "SDL_video.h"
#include "SDL_sysvideo.h"
#include "SDL_yuvfuncs.h"
#include "SDL_yuv_sw_c.h"

SDL_Overlay *SDL_CreateYUVOverlay(int w, int h, Uint32 format, SDL_Surface *display) {
	SDL_VideoDevice *video = current_video;
	SDL_VideoDevice *this = current_video;
	const char *yuv_hwaccel;
	SDL_Overlay *overlay;

	if((display->flags & SDL_OPENGL) == SDL_OPENGL) {
		SDL_SetError("YUV overlays are not supported in OpenGL mode");
		return NULL;
	}

	/* Display directly on video surface, if possible */
	if(SDL_getenv("SDL_VIDEO_YUV_DIRECT")) {
		if((display == SDL_PublicSurface) && ((SDL_VideoSurface->format->BytesPerPixel == 2) || (SDL_VideoSurface->format->BytesPerPixel == 4))) {
			display = SDL_VideoSurface;
		}
	}
	overlay = NULL;
	yuv_hwaccel = SDL_getenv("SDL_VIDEO_YUV_HWACCEL");
	if(((display == SDL_VideoSurface) && video->CreateYUVOverlay) && (!yuv_hwaccel || (SDL_atoi(yuv_hwaccel) > 0))) {
		overlay = video->CreateYUVOverlay(this, w, h, format, display);
	}
	/* If hardware YUV overlay failed ... */
	if(overlay == NULL) {
		overlay = SDL_CreateYUV_SW(this, w, h, format, display);
	}
	return overlay;
}

int SDL_LockYUVOverlay(SDL_Overlay *overlay) {
	if(overlay == NULL) {
		SDL_SetError("Passed NULL overlay");
		return -1;
	}
	return overlay->hwfuncs->Lock(current_video, overlay);
}

void SDL_UnlockYUVOverlay(SDL_Overlay *overlay) {
	if(overlay == NULL) {
		return;
	}
	overlay->hwfuncs->Unlock(current_video, overlay);
}

int SDL_DisplayYUVOverlay(SDL_Overlay *overlay, SDL_Rect *dstrect) {
	SDL_Rect src, dst;
	int srcx, srcy, srcw, srch;
	int dstx, dsty, dstw, dsth;

	if(overlay == NULL || dstrect == NULL) {
		SDL_SetError("Passed NULL overlay or dstrect");
		return -1;
	}

	/* Clip the rectangle to the screen area */
	srcx = 0;
	srcy = 0;
	srcw = overlay->w;
	srch = overlay->h;
	dstx = dstrect->x;
	dsty = dstrect->y;
	dstw = dstrect->w;
	dsth = dstrect->h;
	if(dstx < 0) {
		srcw += (dstx * overlay->w) / dstrect->w;
		dstw += dstx;
		srcx -= (dstx * overlay->w) / dstrect->w;
		dstx = 0;
	}
	if((dstx + dstw) > current_video->screen->w) {
		int extra = (dstx + dstw - current_video->screen->w);
		srcw -= (extra * overlay->w) / dstrect->w;
		dstw -= extra;
	}
	if(dsty < 0) {
		srch += (dsty * overlay->h) / dstrect->h;
		dsth += dsty;
		srcy -= (dsty * overlay->h) / dstrect->h;
		dsty = 0;
	}
	if((dsty + dsth) > current_video->screen->h) {
		int extra = (dsty + dsth - current_video->screen->h);
		srch -= (extra * overlay->h) / dstrect->h;
		dsth -= extra;
	}
	if(srcw <= 0 || srch <= 0 || srch <= 0 || dsth <= 0) {
		return 0;
	}
	/* Ugh, I can't wait for SDL_Rect to be int values */
	src.x = srcx;
	src.y = srcy;
	src.w = srcw;
	src.h = srch;
	dst.x = dstx;
	dst.y = dsty;
	dst.w = dstw;
	dst.h = dsth;
	return overlay->hwfuncs->Display(current_video, overlay, &src, &dst);
}

void SDL_FreeYUVOverlay(SDL_Overlay *overlay) {
	if(overlay == NULL) {
		return;
	}
	if(overlay->hwfuncs) {
		overlay->hwfuncs->FreeHW(current_video, overlay);
	}
	SDL_free(overlay);
}

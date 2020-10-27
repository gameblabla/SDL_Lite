/*
 *###################################################################################
 * Project       : SDLite 1.2.x
 * Module        : Core
 * Purpose       : Provide Low-level access to framebuffer, audio output and HID.
 * License       : GNU General Public License v3.0
 *###################################################################################
 *
 * Backport Power Management from SDL 2
 * This file is part of the SDLite 1.2.x - SDK for Embedded Systems.
 * Copyright (c) 2019-2020 Marcus Andrade
 *
 * Simple DirectMedia Layer
 * Copyright (C) 1997-2020 Sam Lantinga <slouken@libsdl.org>
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 *
 *###################################################################################
 */
#include "SDL_config.h"

#include "SDL_power.h"
#include "SDL_syspower.h"

/*
 * Returns SDL_TRUE if we have a definitive answer.
 * SDL_FALSE to try next implementation.
 */
typedef SDL_bool
(*SDL_GetPowerInfo_Impl)(SDL_PowerState *state, int *seconds, int *percent);

#ifndef SDL_POWER_DISABLED

#ifdef SDL_POWER_HARDWIRED
/* This is for things that _never_ have a battery */
static SDL_bool
SDL_GetPowerInfo_Hardwired(SDL_PowerState * state, int *seconds, int *percent) {
	*seconds = -1;
	*percent = -1;
	*state = SDL_POWERSTATE_NO_BATTERY;
	return SDL_TRUE;
}
#endif

static SDL_GetPowerInfo_Impl implementations[] = {
#ifdef SDL_POWER_LINUX /* in order of preference. More than could work. */
	SDL_GetPowerInfo_Linux_sys_class_power_supply,
	SDL_GetPowerInfo_Linux_proc_acpi,
	SDL_GetPowerInfo_Linux_proc_apm,
#endif
#ifdef SDL_POWER_HARDWIRED
	SDL_GetPowerInfo_Hardwired,
#endif
};
#endif

SDL_PowerState SDL_GetPowerInfo(int *seconds, int *percent) {
#ifndef SDL_POWER_DISABLED
	const int total = sizeof(implementations) / sizeof(implementations[0]);
	SDL_PowerState retval = SDL_POWERSTATE_UNKNOWN;
	int i;
#endif

	int _seconds, _percent;
	/* Make these never NULL for platform-specific implementations. */
	if(seconds == NULL) {
		seconds = &_seconds;
	}
	if(percent == NULL) {
		percent = &_percent;
	}

#ifndef SDL_POWER_DISABLED
	for (i = 0; i < total; i++) {
		if(implementations[i](&retval, seconds, percent)) {
			return retval;
		}
	}
#endif
	/* nothing was definitive. */
	*seconds = -1;
	*percent = -1;
	return SDL_POWERSTATE_UNKNOWN;
}


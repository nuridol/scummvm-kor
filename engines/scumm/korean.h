/* ScummVM - Scumm Interpreter
 * Copyright (C) 2002-2004 The ScummVM Kor. Project
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * $Header: /cvsroot/scummkor/scummvm-kor/scumm/korean.h,v 1.4 2004/08/08 04:49:53 wonst719 Exp $
 */
#ifndef SCUMM_KOREAN_H
#define SCUMM_KOREAN_H

#ifdef SCUMMVMKOR
#include <string.h>

//#include "common/scummsys.h"

#if defined( __DS__ )
#include <SDL.h>
typedef SDL_Color K_Color;
typedef SDL_Surface K_Surface;
#else
typedef uint16 K_Color;
typedef uint16 K_Surface;
#endif

namespace Scumm {

#ifndef ABS
#define ABS(x) ((x) >= 0 ? (x) : -(x))
#endif
	
#ifndef MIN
#define	MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif
	
#ifndef MAX
#define	MAX(a,b) (((a) > (b)) ? (a) : (b))
#endif
	
#define MAX_KOR 32
	
#define INIT_KOR_DELAYS for(int numb=0;numb<MAX_KOR;numb++) if(_strKSet1[numb].delay!=-1) _strKSet1[numb].delay = 0
	
	struct StringKor {
		uint16 xpos, ypos;
		uint8 color;
		char buffer[1024];
		int16 delay;
		bool remainflag;
		bool remainend;
		bool remainstart;
	};
	
	struct StringKorSmush {
		long xpos, ypos, xright;
		uint8 color;
		char buffer[1024];
		uint8 remain;
	};
	
	extern bool _koreanMode;
	extern bool _koreanOnly;
	extern bool _highRes;
	
	extern StringKor _strKSet1[MAX_KOR];
	extern StringKor _strKDesc[MAX_KOR];
	extern StringKorSmush _strKSmush[MAX_KOR];
	
	extern K_Color *_kPalette;
	extern void putEmergencyFont(K_Surface *screen, int xpos, int ypos, int scrw, int scrh, uint16 color, const char *buffer);
	
	extern void loadKoreanFiles(const char *id);
	extern void unloadKoreanFiles();
	
	extern char *convertToKorean(const char *buf, bool descFlag);
	extern char *convertToKoreanValid(const char *buf, bool descFlag);
	extern void addKString(char *buf, uint16 xpos, uint16 ypos, short delay, uint8 col);
	extern void addKDesc(char *buf, uint16 xpos, uint16 ypos, short delay, uint8 col);
	extern void addKSmush(char *buf, long xpos, long ypos, uint8 col);
	
} // End of namespace Scumm

#endif

#endif

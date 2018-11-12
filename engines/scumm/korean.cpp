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
 * $Header: /cvsroot/scummkor/scummvm-kor/scumm/korean.cpp,v 1.7 2004/12/23 12:26:00 wonst719 Exp $
 */

#ifdef SCUMMVMKOR

#include "scumm/scumm.h"
#include "common/scummsys.h"
#if defined( __DS__ )
#include <SDL.h>
#endif

#include "scumm/ks_check.h"
#include "scumm/korean.h"

#include "common/file.h"

//using namespace Scumm;

int strncasecmp2(const char *s1, const char *s2, int n)
{
	char t1, t2;
	char u1, u2;
	
	while(n--) {
		if (!*s2) return (*s1 != 0);
		else if (!*s1) return -1;
		u1= *s1++;
		u2= *s2++;
		t1 = (('A' <= u1 && u1 <= 'Z') ? (u1 | 040) : u1);
		t2 = (('A' <= u2 && u2 <= 'Z') ? (u2 | 040) : u2);
		if (t1 != t2) return (t1 > t2) ? 1 : -1;
	}
	return 0;
}

namespace Scumm {
	
	bool _koreanMode = 0;
	bool _koreanOnly = 0;
	bool _highRes = 0;
	
	char _gameID[16];
	
	char _parseBuffer[1024]; // 메모리를 정적으로
	
	StringKor _strKSet1[MAX_KOR];
	StringKor _strKDesc[MAX_KOR];
	StringKorSmush _strKSmush[MAX_KOR];
	
	char **_KBuffer = 0;	// 한글 대사 파일이 저장될 곳
	int _numKLines = 0;		// 대사의 총 라인 수
	
	int desc_num;			// description의 현재 출력 번호
	
	int _korFontWidth = 0;	// 한글 폰트 너비
	int _korFontHeight = 0;	// 한글 폰트 높이
	int _engFontWidth = 0;	// 영문 폰트 너비
	int _engFontHeight = 0;	// 영문 폰트 높이
	
	K_Color *_kPalette;
	
	byte *_korFontPtr = 0;
	byte *_engFontPtr = 0;
	
	char *convertToKorean(const char *buf, bool descFlag)
	{
		static int currentLine;
		
		char *tbuf, *sbuf;
		bool found = 0;
		
		sbuf = tbuf = _parseBuffer;
		
		/*
		 bool mFlg;
		 mFlg = 0;
		 for(int mi = 0; mi < xxi; mi++) {
		 if(!strncasecmp(buf, strBuf[mi], strlen(buf))){
		 mFlg = 1;
		 break;
		 }
		 }
		 if(!mFlg) {
		 strcpy(strBuf[xxi], buf);
		 xxi++;
		 fprintf(korOutput, "%s\n", buf);
		 }	// 임시 파일에 저장
		 */
		
		if(buf[0] == 0x07 && descFlag)
			*buf++;
		
		/*
		 if (buf[0] == 'Y' && buf[1] == 'e' && buf[2] == 's') {
		 printf("convertToKoreanValid buf = <%s>\n", buf);
		 }
		 */
		
		int i;
		for(i = currentLine; i < _numKLines; i += 2) {
			if(!strncasecmp2(buf, _KBuffer[i], strlen(_KBuffer[i])) && strlen(_KBuffer[i]) == strlen(buf)) {
				strcpy(tbuf, _KBuffer[i + 1]);
				currentLine = i;
				found = 1;
				break;
			}
		}
		if(found == 0) {
			for(i = 0; i < currentLine; i += 2) { // 한번 더 찾아줌
				if(!strncasecmp2(buf, _KBuffer[i], strlen(_KBuffer[i])) && strlen(_KBuffer[i]) == strlen(buf)) {
					strcpy(tbuf, _KBuffer[i + 1]);
					currentLine = i;
					found = 1;
					break;
				}
			}
		}
		
		if(found == 0 && descFlag) {
			if(_highRes)
				strcpy(tbuf, buf);
			else
				*tbuf = 0;
		} else if(found == 0)
			if(_koreanOnly) {
				strcpy(tbuf, buf);	// 못 찾으면 영어 문장으로 대체
				while(*tbuf){
					if(!strncasecmp2(tbuf, "<1>", strlen("<1>"))) {
						*tbuf = '\\';
						*(tbuf + 1) = '\\';
						*(tbuf + 2) = 'n';
					}
					if(!strncasecmp2(buf, "<3>", strlen("<3>"))) {
						*tbuf = '\\';
						*(tbuf + 1) = '\\';
						*(tbuf + 2) = 'n';
					}
					if(!strncasecmp2(tbuf, "<c>", strlen("<c>"))) {
						*tbuf = '\\';
						*(tbuf +1) = '\\';
						*(tbuf +2) = 'c';
					}
					tbuf++;
				}
				tbuf = sbuf;
			} else
				*tbuf = 0;	// 영어 문장 안나올 때만 대체
		
		return sbuf;
	}
	
	char *convertToKoreanValid(const char *buf, bool descFlag)
	{
		static int currentLine;
		
		char *tbuf, *sbuf;
		bool found = 0;
		
		sbuf = tbuf = _parseBuffer;
		
		if(buf[0] == 0x07 && descFlag)
			*buf++;
		
		/*
		 if (buf[0] == 'Y' && buf[1] == 'e' && buf[2] == 's') {
		 printf("convertToKoreanValid buf = <%s>\n", buf);
		 }
		 */
		
		int i;
		for(i = currentLine; i < _numKLines; i += 2) {
			if(!strncasecmp2(buf, _KBuffer[i], strlen(_KBuffer[i])) && strlen(_KBuffer[i]) == strlen(buf)) {
				strcpy(tbuf, _KBuffer[i + 1]);
				currentLine = i;
				found = 1;
				break;
			}
		}
		if(found == 0) {
			for(i = 0; i < currentLine/*_numKLines*/; i += 2) { //한번 더 찾아 줌
				if(!strncasecmp2(buf, _KBuffer[i], strlen(_KBuffer[i])) && strlen(_KBuffer[i]) == strlen(buf)) {
					strcpy(tbuf, _KBuffer[i + 1]);
					currentLine = i;
					found = 1;
					break;
				}
			}
		}
		
		// 못 찾으면 null return
		if (found == 0)
			return 0;
		
		return sbuf;
	}
	
#ifdef UPDATE_HACK_REMOVED
	
	void addKString(char *buf, uint16 xpos, uint16 ypos, short delay, uint8 col)
	{
		int num;
		for(num = 0; num < MAX_KOR - 1; num++)
			if(!(_strKSet1[num].buffer[0] && 0))
				break;
		//if(num == MAX_KOR - 1)
		//	break;
		
		StringKor *s = &_strKSet1[num];
		s->xpos = xpos;
		s->ypos = ypos;
		s->delay = delay;
		s->color = col;
		strcpy(s->buffer, buf);
		s->remainflag = 1;
		s->remainstart = 1;
		s->remainend = 0;
	}
	
	void addKDesc(char *buf, uint16 xpos, uint16 ypos, short delay, uint8 col)
	{
		desc_num = 0;
		StringKor *s = &_strKDesc[desc_num];
		s->xpos = xpos;
		s->ypos = ypos;
		s->delay = delay;
		s->color = col;
		strcpy(s->buffer, buf);
		s->remainflag = 1;
		s->remainstart = 1;
		s->remainend = 0;
	}
	
	void addKSmush(char *buf, long xpos, long ypos, uint8 col)
	{
		int num;
		for(num = 0; num < MAX_KOR - 1; num++)
			if(!(_strKSmush[num].buffer[0] && _strKSmush[num].remain))
				break;
		//if(num == MAX_KOR - 1)
		//	break;
		
		StringKorSmush *s = &_strKSmush[num];
		s->xpos = MAX(xpos, 2);
		s->ypos = MAX(ypos, 2);
		//s->xright = 320 - xpos;
		s->color = col;
		s->remain = 1;
		strcpy(s->buffer, buf);
	}
	
#endif
	
	static void FGETS(char *buf, int maxlen, Common::File &fp)
	{
		int i, n;
		char c;
		
		for (i = 0; i < maxlen; i++) {
			n = fp.read(&c, 1);
			if (n == 0) {
				buf[i] = 0;
				break;
			}
			buf[i] = c;
			if (c == '\n') {
				if (i < maxlen-1)
					buf[i] = 0;
				break;
			}
		}
	}
#ifndef fgets
#define fgets FGETS
#endif
	
	void loadKoreanStrings()
	{
		Common::File fp;
		char fname[128];
		
		strcpy(fname, "sub/");
		strcat(fname, _gameID);
		strcat(fname, ".dat");
		
		if(!fp.open(fname)) {
			// warning("WARNING: Cannot load Korean V1 subtitle!, %s\n", fname);

			// retry without sub
			strcpy(fname, "");
			strcat(fname, _gameID);
			strcat(fname, ".dat");
				
			if(!fp.open(fname)) {
				warning("WARNING: Cannot load Korean V1 subtitle!, %s\n", fname);
				return;
			}
		}
		
		int k;
		char *buf = new char[1024];
		int len = 0;
		
		for(k = 0; !fp.eos(); k++) {
			FGETS(buf, 1023, fp);
			//printf("%d: <%s>\n", k+1, buf);
		}
		// 파일의 라인 수를 계산
		_numKLines = k;
		
		// memory size = file size
		k = fp.size();
		fp.seek(0, SEEK_SET);
		
		_KBuffer = new char *[_numKLines];
		//printf("_KBuffer size = %d\n", (int)k);
		_KBuffer[0] = new char[k];
		//printf("_KBuffer[0] = 0x%x\n", (int)_KBuffer[0]);
		
		for(int i = 0; i < _numKLines; i++) {
			FGETS(buf, 1023, fp);
			buf[1023] = 0;
			len = strlen(buf);
			//_KBuffer[i] = new char[strlen(buf) + 1]; //\0
			if (i > 0)
				_KBuffer[i] = _KBuffer[i-1] + strlen(_KBuffer[i-1])+1;
			if (len > 1000)
				warning("_KBuffer[%d]:%lx, len=%d\n", i, _KBuffer[i], len);
			if(strlen(buf)) {
				strcpy(_KBuffer[i], buf);
				char *b = _KBuffer[i];
				if(b[strlen(b) - 1] == '\n' || b[strlen(b) - 1] == 0x0a || b[strlen(b) - 1] == 0x0d)
					b[strlen(b) - 1] = 0;
				if(b[strlen(b) - 1] == '\n' || b[strlen(b) - 1] == 0x0a || b[strlen(b) - 1] == 0x0d)
					b[strlen(b) - 1] = 0;
			} else
				_KBuffer[i][0] = 0;
		}
		fp.close();
		
		warning("Korean subtitle file loaded -- total %d lines, %ld bytes\n", _numKLines, _KBuffer[_numKLines-1]-_KBuffer[0]+len);
		
		delete[] buf;
		
		return;
	}
	
	bool unloadKorString()
	{
		if (_KBuffer && _KBuffer[0]) delete []_KBuffer[0];
		if (_KBuffer) delete []_KBuffer;
		return true;
	}
	
	void unloadEmergencyFont()
	{
		if (_korFontPtr) delete []_korFontPtr;
		if (_engFontPtr) delete []_engFontPtr;
	}
	
	void unloadKoreanFiles()
	{
		unloadKorString();
		unloadEmergencyFont();
	}
	
	// FIXME/TODO: 아래는 코드 중복 모음
	void loadEmergencyFont()
	{
		Common::File fp;
		int numChar = 0;
		
		numChar = 2350;
		if (fp.open("korean.fnt")) {
			fp.seek(2, SEEK_CUR);
			_korFontWidth = fp.readByte();
			_korFontHeight = fp.readByte();
			_korFontPtr = new byte[((_korFontWidth + 7) / 8) * _korFontHeight * numChar];
			
			fp.read(_korFontPtr, ((_korFontWidth + 7) / 8) * _korFontHeight * numChar);
			fp.close();
			warning("V1 한글 폰트가 로드되었습니다.\n");
		} else {
			warning("V1 한글 폰트를 로드할 수 없습니다!\n");
		}
		
		numChar = 256;
		if (fp.open("english.fnt")) {
			fp.seek(2, SEEK_CUR);
			_engFontWidth = fp.readByte();
			_engFontHeight = fp.readByte();
			_engFontPtr = new byte[((_engFontWidth + 7) / 8) * _engFontHeight * numChar];
			
			fp.read(_engFontPtr, ((_engFontWidth + 7) / 8) * _engFontHeight * numChar);
			fp.close();
			warning("V1 영문 폰트가 로드되었습니다.\n");
		} else {
			warning("V1 영문 폰트를 로드할 수 없습니다!\n");
		}
	}
	
	byte *getFontPtr(int idx)
	{
		if(idx & 0x7f00) {
			idx = ((idx % 256) - 0xb0) * 94 + (idx / 256) - 0xa1;
			return 	_korFontPtr + ((_korFontWidth + 7) / 8) * _korFontHeight * idx;
		} else {
			return 	_engFontPtr + ((_engFontWidth + 7) / 8) * _engFontHeight * idx;
		}
	}
	
	int getTopMargin(uint8 eb)
	{
		switch(eb) {
			case 'g':
			case 'p':
			case 'q':
			case 'y':
			case ',':
			case '!':
				return 1;
			default:
				return 0;
				break;
		}
	}
	
	int getEngWidth(uint8 eb)
	{
		switch(eb) {
			case '_':
				return 9;
			case '5':
			case 'J':
			case 'M':
			case 'W':
			case 'm':
			case 'w':
			case '*':
				return 8;
			case 'e':
				return 7;
			case 'f':
			case 'I':
			case 'i':
			case 'l':
			case 'c':
				return 6;
			case '.':
			case '!':
				return 5;
			case ' ':
				return 4;
			default:
				return 7;
				break;
		}
	}
	
#if defined( __DS__ )
	
	void putEmergencyChar(uint16 *dst, int _screenWidth, uint16 _color, int chr)
	{
		const byte revBitMask[8] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };
		int y, x;
		byte bits = 0;
		const byte *src = getFontPtr(chr);
		
		int height, width;
		
		height = (chr & 0x7f00) ? _korFontHeight : _engFontHeight;
		width = (chr & 0x7f00) ? _korFontWidth : _engFontWidth;
		
		for (y = 0; y < height; y++) {
			for (x = 0; x < width; x++) {
				if ((x % 8) == 0)
					bits = *src++;
				if ((bits & revBitMask[x % 8])) {
					*dst = _color;
				}
				dst++;
			}
			dst += _screenWidth - width;
		}
	}
	
	uint16 getRGB(K_Surface *screen, uint8 R, uint8 G, uint8 B)
	{
		return SDL_MapRGB(screen->format, R, G, B);
	}
	
	void putEmergencyFont(K_Surface *screen, int xpos, int ypos, int scrw, int scrh, uint16 color, const char *buf)
	{
		const byte *buffer = (const byte *)buf;
		
		int clipw = 0;
		
		int px = xpos;
		int py = ypos;
		if ( SDL_MUSTLOCK(screen) ) {
			if ( SDL_LockSurface(screen) < 0 ) {
				//printf("putEmergencyFont(): Lock 실패\n");
				warning("putEmergencyFont(): Lock 실패\n");
				return;
			}
		}
		uint16 *dst;
		
		int c;
		
		int w = 8;
		int h = _engFontHeight;
		
		int engMargin = 0;
		
		do {
			c = *buffer++;
			if (c == 0) {
				break;
			}
			if (c == 0x0B)
				continue;
			if (c == '\\') {
				if ( *buffer == 'n' || *buffer == 'N' ) {
					buffer++;
					px = xpos;
					py += h + 2;
					dst = (uint16 *)screen->pixels + py * screen->pitch / 2 + px;
				} else if (*buffer == 'c' || *buffer == 'C' )	{
					uint8 d = *buffer++;
					if (color)	// 검은색(0,0,0)이 아니라면
						color = getRGB(screen, _kPalette[d].r, _kPalette[d].g, _kPalette[d].b);
				}
				continue;
			} else if ( c == 13 ) {
				if ( *buffer == 10 ) {
					px = xpos;
					py += h + 2;
				}
				continue;
			}
			if (c & 0x80) {
				c += (*buffer++) * 256;	//LE
				w = _korFontWidth;
				h = _korFontHeight;
				engMargin = 0;
			} else {
				w = getEngWidth(c);
				h = _engFontHeight;
				engMargin = getTopMargin(c);
			}
			
			if(px < 0 || px + w > (scrw + (1 - clipw))) {	// 가로로 화면 밖
				px = xpos;
				py += h + 2;
			}
			dst = (uint16 *)screen->pixels + (py + engMargin) * screen->pitch / 2 + px;
			
			int offsetX[9] = { -1,  0, 1, 1, 1, 0, -1, -1, 0 };
			int offsetY[9] = {  -1, -1, -1, 0, 1, 1, 1, 0, 0 };
			int cTable[9] =  {  0,  0, 0, 0, 0, 0, 0, 0, color };
			int i = 0;
			
			int showShadow = 1;
			
			if (!showShadow)
				i = 8;
			
			for (; i < 9; i++) {
				dst = (uint16 *)screen->pixels + (py + engMargin + offsetY[i]) * screen->pitch / 2 + (px + offsetX[i]);
				putEmergencyChar(dst, (screen->pitch / 2), cTable[i], c);
			}
			dst += w;
			px += w;
		} while (1);
		
		if(SDL_MUSTLOCK(screen))
			SDL_UnlockSurface(screen);
	}
	
#endif
	
	////////////////////////////////////
	
	void loadKoreanFiles(const char *id)
	{
		strcpy(_gameID, id);
		
		if(!strncasecmp2(_gameID, "monkeyega", 9))
			strcpy(_gameID, "monkey");
		if(!strncasecmp2(_gameID, "monkeyvga", 9))
			strcpy(_gameID, "monkey");
		if(!strncasecmp2(_gameID, "monkey1", 7))
			strcpy(_gameID, "monkey");
		if(!strncasecmp2(_gameID, "indy3ega", 8))
			strcpy(_gameID, "indy3");
		
		//loadEmergencyFont();
		loadKoreanStrings();
	}
	
} // End of namespace Scumm

#endif

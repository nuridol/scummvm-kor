/* ScummVM - Scumm Interpreter
 * Copyright (C) 2002-2005 The ScummVM project
 * ScummVM Kor. - Korean ScummVM Mod.
 * Copyright (C) 2002-2005 The ScummVM Kor Project
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
 * $Header: /cvsroot/scummvm/scummvm/graphics/font.cpp,v 1.12 2005/06/04 13:19:53 fingolfin Exp $
 */

#ifdef SCUMMVMKOR

#include "scumm/scumm.h"
#include "graphics/font.h"
#include "common/file.h"

namespace Graphics {
	
	// TODO: 다중 폰트 사용
	
	static byte *korfont = NULL;
	static int korwidth = 0;
	static int korheight = 0;
	
	int getKorFontWidth()
	{
		return korwidth;
	}
	
	int getKorFontHeight()
	{
		return korheight;
	}
	
	bool checkKorCode(byte hi, byte lo)
	{
		// hi: xx
		// lo: yy
		if ((0xA1 > lo) || (0xFE < lo)) {
			return false;
		}
		if ((hi >= 0xB0) && (hi <= 0xC8)) {
			return true;
		}
		return false;
	}
	
	byte *getKorCharPtr(int idx)
	{
		idx += 256;	// HACK
		if(!checkKorCode(idx % 256, idx / 256))
			return NULL;
		
		idx = ((idx % 256) - 0xb0) * 94 + (idx / 256) - 0xa1;
		if(idx < 0 || idx >= 2350) {
			//warning("Invaild index %d", idx);
			return NULL;
		}
		
		return korfont + ((korwidth + 7) / 8) * korheight * idx;	// FIXME
	}
	
	void loadKoreanGUIFont()
	{
		Common::File file;
		const char *fontFile = "korean_g.fnt";
		
		if (file.open(fontFile)) {
			int numChar = 2350;
			
			debug(2, "Loading Korean GUI Font");
			file.seek(2, SEEK_CUR);
			korwidth = file.readByte();
			korheight = file.readByte();
			
			korfont = new byte[((korwidth + 7) / 8) * korheight * numChar];
			file.read(korfont, ((korwidth + 7) / 8) * korheight * numChar);
			file.close();
			//warning("Font: %d %d at %x", korwidth, korheight, korfont);
		} else {
			//warning("Couldn't load any font");
		}
	}
	
	void unloadKoreanGUIFont()
	{
		delete []korfont;
	}
	
	void drawKorChar(Surface *dst, uint32 chr, int tx, int ty, uint32 color)
	{
		const byte revBitMask[8] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };
		
		assert(dst != 0);
		byte *ptr = (byte *)dst->getBasePtr(tx, ty);
		
		byte bits = 0;
		
		const byte *tmp = getKorCharPtr(chr);
		if(tmp == NULL) {
			//warning("can't draw char");
			return;
		}
		
		for (int y = 0; y < korheight; y++) {
			if(ty + y < 0 || ty + y >= dst->h)
				continue;
			for (int x = 0; x < korwidth; x++) {
				if (tx + x < 0 || tx + x >= dst->w)
					continue;
				if ((x % 8) == 0)
					bits = *tmp++;
				if ((bits & revBitMask[x % 8])) {
					if (dst->format.bytesPerPixel == 1)
						ptr[x] = color;
					else if (dst->format.bytesPerPixel == 2)
						((uint16 *)ptr)[x] = color;
				}
			}
			ptr += dst->pitch;
		}
	}
	
} // End of namespace Graphics

#endif

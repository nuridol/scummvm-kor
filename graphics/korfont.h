/* ScummVM - Scumm Interpreter
 * Copyright (C) 2002-2005 The ScummVM project
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
 * $Header: /cvsroot/scummvm/scummvm/graphics/font.h,v 1.8 2005/05/15 16:13:52 fingolfin Exp $
 */

#ifdef SCUMMVMKOR

#include "common/str.h"
#include "graphics/surface.h"

namespace Graphics {
	
	int getKorFontWidth();
	int getKorFontHeight();
	bool checkKorCode(byte hi, byte lo);
	void loadKoreanGUIFont();
	void unloadKoreanGUIFont();
	void drawKorChar(Surface *dst, uint32 chr, int tx, int ty, uint32 color);
	
} // End of namespace Graphics

#endif

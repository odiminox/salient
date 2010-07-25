/*
* Umbra
* Copyright (c) 2009 Dominik Marczuk, Jice
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * The names of Dominik Marczuk or Jice may not be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY DOMINIK MARCZUK & JICE ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL DOMINIK MARCZUK OR JICE BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "utilities.hpp"

bool Rect::offset(int sx, int sy) {
	if ((unsigned)(x+sx) <= MAXX-1 &&
		(unsigned)(x+w+sx) <= MAXX-1 &&
		(unsigned)(y+sy) <= MAXY-1 &&
		(unsigned)(y+h+sy) <= MAXY-1) {
		x += sx;
		y += sy;
		return true;
	}
	else
		return false;
}

bool Rect::checkIntersection(const Rect &r) const {
	int sx = MAX(x,r.x);
	int sy = MAX(y,r.y);
	int ex = MIN(x+w,r.x+r.w);
	int ey = MIN(y+h,r.y+r.h);
	if (sx <= ex && sy <= ey)
		return true;
	else
		return false;
}

bool Rect::contains(const Point &p) const {
	if (p.x >= x && p.x < x + w && p.y >= y && p.y < y + h)
		return true;
	else
		return false;
}

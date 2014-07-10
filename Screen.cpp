// Screen routines for nPDF
// Copyright (C) 2014  Legimet
//
// This file is part of nPDF.
//
// nPDF is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// nPDF is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with nPDF.  If not, see <http://www.gnu.org/licenses/>.

#include <algorithm>
#include <cstdlib>
#include <cstdint>
#include <libndls.h>
#include "Screen.hpp"

// Using double-buffering
namespace Screen {
    volatile uint8_t* const origBuf = reinterpret_cast<volatile uint8_t*>(SCREEN_BASE_ADDRESS);
    int curBuf = 0;
    bool hasColors = false;
    volatile uint8_t *buf[2];
    
    bool init() {
	if (has_colors) {
	    hasColors = true;
	}
	buf[0] = new uint8_t[SCREEN_BYTES_SIZE];
	buf[1] = origBuf;
	if (buf[0]) {
	    if (std::atexit(deinit)) {
		deinit();
		return false;
	    } else {
		return true;
	    }
	} else {
	    return false;
	}
    }
    
    void deinit() {
	delete[] buf[0];
	buf[0] = nullptr;
	SCREEN_BASE_ADDRESS = reinterpret_cast<volatile unsigned>(origBuf);
    }
    
    void switchBufs() {
	SCREEN_BASE_ADDRESS = reinterpret_cast<volatile unsigned>(buf[curBuf]);
	curBuf ^= 1;
    }
    
    void setPixel(uint8_t r, uint8_t g, uint8_t b, unsigned int x, unsigned int y) {
	// On color models, each pixel is represented in 16-bit high color
	// On classic models, each pixel is 4 bits grayscale, 0 is black and 15 is white
	if (x < SCREEN_WIDTH && y < SCREEN_HEIGHT) {
	    unsigned int pos = y * SCREEN_WIDTH + x;
	    if (hasColors) {
		(reinterpret_cast<volatile uint16_t*>(buf[curBuf]))[pos] = ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
	    } else if (pos % 2 == 0) {
		buf[curBuf][pos / 2] = (buf[curBuf][pos / 2] & 0x0F) | (((30 * r + 59 * g + 11 * b) / 100) & 0xF0);
	    } else {
		buf[curBuf][pos / 2] = (buf[curBuf][pos / 2] & 0xF0) | (((30 * r + 59 * g + 11 * b) / 100) >> 4);
	    }
	}
    }
    
    void setPixel(uint8_t c, unsigned int x, unsigned int y) {
	// On color models, each pixel is represented in 16-bit high color
	// On classic models, each pixel is 4 bits grayscale, 0 is black and 15 is white
	if (x < SCREEN_WIDTH && y < SCREEN_HEIGHT) {
	    unsigned int pos = y * SCREEN_WIDTH + x;
	    if (hasColors) {
		(reinterpret_cast<volatile uint16_t*>(buf[curBuf]))[pos] = ((c >> 3) << 11) | ((c >> 2) << 5) | (c >> 3);
	    } else if (pos % 2 == 0) {
		buf[curBuf][pos / 2] = (buf[curBuf][pos / 2] & 0x0F) | (c & 0xF0);
	    } else {
		buf[curBuf][pos / 2] = (buf[curBuf][pos / 2] & 0xF0) | (c >> 4);
	    }
	}
    }
    
    void showImgRGB(uint8_t *img, unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1,
		    unsigned int w, unsigned int h, unsigned int wTotal) {
	unsigned int pos;
	for (unsigned int i = x1; i < x1 + w; i++) {
	    for (unsigned int j = y1; j < y1 + h; j++) {
		pos = 3 * (wTotal * j + i);
		setPixel(img[pos], img[pos + 1], img[pos + 2], x0 - x1 + i, y0 - y1 + j);
	    }
	}
    }
    
    void showImgRGBA(uint8_t *img, unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1,
		    unsigned int w, unsigned int h, unsigned int wTotal) {
	unsigned int pos;
	for (unsigned int i = x1; i < x1 + w; i++) {
	    for (unsigned int j = y1; j < y1 + h; j++) {
		pos = 4 * (wTotal * j + i);
		setPixel(img[pos], img[pos + 1], img[pos + 2], x0 - x1 + i, y0 - y1 + j);
	    }
	}
    }
    
    void showImgGray(uint8_t *img, unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1,
		    unsigned int w, unsigned int h, unsigned int wTotal) {
	unsigned int pos;
	for (unsigned int i = x1; i < x1 + w; i++) {
	    for (unsigned int j = y1; j < y1 + h; j++) {
		pos = wTotal * j + i;
		setPixel(img[pos], x0 - x1 + i, y0 - y1 + j);
	    }
	}
    }
    
    void showImgGrayA(uint8_t *img, unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1,
		    unsigned int w, unsigned int h, unsigned int wTotal) {
	unsigned int pos;
	for (unsigned int i = x1; i < x1 + w; i++) {
	    for (unsigned int j = y1; j < y1 + h; j++) {
		pos = 2 * (wTotal * j + i);
		setPixel(img[pos], x0 - x1 + i, y0 - y1 + j);
	    }
	}
    }
    
    void fillScreen(uint8_t r, uint8_t g, uint8_t b) {
	uint16_t color;
	if (hasColors) {
	    color = ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
	} else {
	    color = (30 * r + 59 * g + 11 * b) / 100;
	    color = (color >> 4) * 4369;
	}

	std::fill(reinterpret_cast<volatile uint16_t*>(buf[curBuf]),
		    reinterpret_cast<volatile uint16_t*>(buf[curBuf] + SCREEN_BYTES_SIZE),
		    color);
    }
    
    void fillScreen(uint8_t c) {
	uint16_t color;
	if (hasColors) {
	    color = ((c >> 3) << 11) | ((c >> 2) << 5) | (c >> 3);
	} else {
	    color = (c >> 4) * 4369;
	}

	std::fill(reinterpret_cast<volatile uint16_t*>(buf[curBuf]),
		    reinterpret_cast<volatile uint16_t*>(buf[curBuf] + SCREEN_BYTES_SIZE),
		    color);
    }
}

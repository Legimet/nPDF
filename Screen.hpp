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

#ifndef _SCREEN_HPP_
#define _SCREEN_HPP_

namespace Screen {
    bool init();
    
    void deinit();
    
    void switchBufs();
    
    void setPixel(unsigned char r, unsigned char g, unsigned char b, int x, int y);

    void setPixel(unsigned char c, int x, int y);
    
    void showImgRGB(unsigned char *img, int x0, int y0, int x1, int y1, int w, int h, int wTotal);
    
    void showImgRGBA(unsigned char *img, int x0, int y0, int x1, int y1, int w, int h, int wTotal);
    
    void showImgGray(unsigned char *img, int x0, int y0, int x1, int y1, int w, int h, int wTotal);
    
    void showImgGrayA(unsigned char *img, int x0, int y0, int x1, int y1, int w, int h, int wTotal);
    
    void fillScreen(unsigned char r, unsigned char g, unsigned char b);
    
    void fillScreen(unsigned char c);
}

#endif

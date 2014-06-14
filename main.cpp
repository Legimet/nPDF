// nPDF main function
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

extern "C" {
#include <mupdf/fitz.h>
}
#include <stdlib.h>
#include <keys.h>
#include <libndls.h>
#include "Viewer.hpp"
#include "Screen.hpp"

int main(void) {
    Viewer v;
    Screen::init();
    clrscr();
    v.init();

    v.openDoc("/documents/ndless/example.pdf.tns");
    v.drawPage();
    v.display();

    while (true) {
	if (isKeyPressed(KEY_NSPIRE_ESC)) {
	    break;
	}
	if (isKeyPressed(KEY_NSPIRE_DOWN)) {
	    v.scrollDown();
	    v.display();
	}
	if (isKeyPressed(KEY_NSPIRE_UP)) {
	    v.scrollUp();
	    v.display();
	}
	if (isKeyPressed(KEY_NSPIRE_RIGHT)) {
	    v.scrollRight();
	    v.display();
	}
	if (isKeyPressed(KEY_NSPIRE_LEFT)) {
	    v.scrollLeft();
	    v.display();
	}
	if (isKeyPressed(KEY_NSPIRE_MULTIPLY)) {
	    v.zoomIn();
	    v.display();
	}
	if (isKeyPressed(KEY_NSPIRE_DIVIDE)) {
	    v.zoomOut();
	    v.display();
	}
	sleep(10);
    }
    
    Screen::deinit();
    
    return 0;
}

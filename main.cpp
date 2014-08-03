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
#include <string>
#include "Viewer.hpp"
#include "Screen.hpp"
#include "Timer.hpp"

enum arrowkey {NONE = 0, DOWN, UP, RIGHT, LEFT};
const int delay1 = 600;
const int delay2 = 10;

arrowkey getArrowKey() {
    if (isKeyPressed(KEY_NSPIRE_DOWN) || isKeyPressed(KEY_NSPIRE_2)) {
	return DOWN;
    } else if (isKeyPressed(KEY_NSPIRE_UP) || isKeyPressed(KEY_NSPIRE_8)) {
	return UP;
    } else if (isKeyPressed(KEY_NSPIRE_RIGHT) || isKeyPressed(KEY_NSPIRE_6)) {
	return RIGHT;
    } else if (isKeyPressed(KEY_NSPIRE_LEFT) || isKeyPressed(KEY_NSPIRE_4)) {
	return LEFT;
    } else {
	return NONE;
    }
}

int main(int argc, char **argv) {    
    Viewer v;

    if (argc >= 2) {
	v.init();
	v.openDoc(argv[1]);
    } else if (argc >= 1) {
	std::string s = std::string(argv[0]);
	size_t pos = s.find_last_of("/");
	if (pos != std::string::npos) {
	    s.erase(0, s.find_last_of("/") + 1);
	}
	if (s.size() >= 4 && s.substr(s.size() - 4) == ".tns") {
	    s.erase(s.size() - 4);
	}
	cfg_register_fileext("pdf", s.c_str());
	cfg_register_fileext("xps", s.c_str());
	cfg_register_fileext("cbz", s.c_str());
	show_msgbox("nPDF", "File extensions registered. You can now open a .pdf, .xps, or .cbz file from the Documents screen");
	return 0;
    }
    Screen::init();
    Timer::init();
    v.drawPage();
    v.display();

    arrowkey lastArrowKey = NONE;
    arrowkey current = NONE;
    
    while (true) {
	if ((current = getArrowKey())) {
	    if (current != lastArrowKey || Timer::done()) {
		if (current == DOWN) {
		    if (lastArrowKey != DOWN) {
			Timer::start(delay1);
			lastArrowKey = DOWN;
		    } else if (Timer::done()) {
			Timer::start(delay2);
		    }
		    v.scrollDown();
		    v.display();
		} else if (current == UP) {
		    if (lastArrowKey != UP) {
			Timer::start(delay1);
			lastArrowKey = UP;
		    } else if (Timer::done()) {
			Timer::start(delay2);
		    }
		    v.scrollUp();
		    v.display();
		} else if (current == RIGHT) {
		    if (lastArrowKey != RIGHT) {
			Timer::start(delay1);
			lastArrowKey = RIGHT;
		    } else if (Timer::done()) {
			Timer::start(delay2);
		    }
		    v.scrollRight();
		    v.display();
		} else if (current == LEFT) {
		    if (lastArrowKey != LEFT) {
			Timer::start(delay1);
			lastArrowKey = LEFT;
		    } else if (Timer::done()) {
			Timer::start(delay2);
		    }
		    v.scrollLeft();
		    v.display();
		}
	    }
	} else {
	    lastArrowKey = NONE;
	    Timer::start(0);
	    if (isKeyPressed(KEY_NSPIRE_ESC)) {
		break;
	    } else if (isKeyPressed(KEY_NSPIRE_MULTIPLY)) {
		v.zoomIn();
		v.display();
	    } else if (isKeyPressed(KEY_NSPIRE_DIVIDE)) {
		v.zoomOut();
		v.display();
	    }
	}
	sleep(10);
    }
    
    return 0;
}

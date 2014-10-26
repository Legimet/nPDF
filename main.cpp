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

// Keys that, when held down, have a long delay after the first time and a
// short delay afterwards
enum scrollkey {NONE = 0, DOWN, UP, RIGHT, LEFT, PGDOWN, PGUP, ZOOMOUT, ZOOMIN};
const int delay1 = 400;
const int delay2 = 40;

scrollkey getScrollKey() {
    if (isKeyPressed(KEY_NSPIRE_DOWN) || isKeyPressed(KEY_NSPIRE_2)) {
	return DOWN;
    } else if (isKeyPressed(KEY_NSPIRE_UP) || isKeyPressed(KEY_NSPIRE_8)) {
	return UP;
    } else if (isKeyPressed(KEY_NSPIRE_RIGHT) || isKeyPressed(KEY_NSPIRE_6)) {
	return RIGHT;
    } else if (isKeyPressed(KEY_NSPIRE_LEFT) || isKeyPressed(KEY_NSPIRE_4)) {
	return LEFT;
    } else if (isKeyPressed(KEY_NSPIRE_3)) {
	return PGDOWN;
    } else if (isKeyPressed(KEY_NSPIRE_9)) {
	return PGUP;
    } else if (isKeyPressed(KEY_NSPIRE_DIVIDE)) {
	return ZOOMOUT;
    } else if (isKeyPressed(KEY_NSPIRE_MULTIPLY)) {
	return ZOOMIN;
    } else {
	return NONE;
    }
}

void handleDelays(scrollkey key, scrollkey& lastScrollKey) {
    if (lastScrollKey != key) {
	Timer::start(delay1);
	lastScrollKey = key;
    } else if (Timer::done()) {
	Timer::start(delay2);
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

    scrollkey lastScrollKey = NONE;
    scrollkey current = NONE;
    
    int page;
    while (true) {
	if ((current = getScrollKey())) {
	    if (current != lastScrollKey || Timer::done()) {
		if (current == DOWN) {
		    handleDelays(DOWN, lastScrollKey);
		    v.scrollDown();
		    v.display();
		} else if (current == UP) {
		    handleDelays(UP, lastScrollKey);
		    v.scrollUp();
		    v.display();
		} else if (current == RIGHT) {
		    handleDelays(RIGHT, lastScrollKey);
		    v.scrollRight();
		    v.display();
		} else if (current == LEFT) {
		    handleDelays(LEFT, lastScrollKey);
		    v.scrollLeft();
		    v.display();
		} else if (current == PGDOWN) {
		    handleDelays(PGDOWN, lastScrollKey);
		    v.next();
		    v.display();
		} else if (current == PGUP) {
		    handleDelays(PGUP, lastScrollKey);
		    v.prev();
		    v.display();
		} else if (current == ZOOMOUT) {
		    handleDelays(ZOOMOUT, lastScrollKey);
		    v.zoomOut();
		    v.display();
		} else if (current == ZOOMIN) {
		    handleDelays(ZOOMIN, lastScrollKey);
		    v.zoomIn();
		    v.display();
		}
	    }
	} else {
	    lastScrollKey = NONE;
	    Timer::stop();
	    if (isKeyPressed(KEY_NSPIRE_ESC)) {
		break;
	    }
	    if (isKeyPressed(KEY_NSPIRE_CTRL) && isKeyPressed(KEY_NSPIRE_G)) {
		if (show_1numeric_input("Go to page", "", "Enter page number", &page, 1, 50)) {
		    v.gotoPage(page - 1);
		    v.display();
		}
	    }
	}
	sleep(10);
    }
    
    return 0;
}

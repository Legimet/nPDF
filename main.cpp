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
#define NONE    0
#define DOWN    1
#define UP      2
#define RIGHT   4
#define LEFT    8
#define PGDOWN  16
#define PGUP    32
#define ZOOMOUT 64
#define ZOOMIN  128
typedef int scrollkey;
const int delay1 = 400;
const int delay2 = 40;

scrollkey getScrollKey() {
    scrollkey action=NONE;
    if (isKeyPressed(KEY_NSPIRE_DOWN) || isKeyPressed(KEY_NSPIRE_RIGHTDOWN) || isKeyPressed(KEY_NSPIRE_DOWNLEFT) ||
	    isKeyPressed(KEY_NSPIRE_2) || isKeyPressed(KEY_NSPIRE_3) || isKeyPressed(KEY_NSPIRE_1))
        action |= DOWN;
    if (isKeyPressed(KEY_NSPIRE_UP) || isKeyPressed(KEY_NSPIRE_UPRIGHT) || isKeyPressed(KEY_NSPIRE_LEFTUP) ||
	    isKeyPressed(KEY_NSPIRE_8) || isKeyPressed(KEY_NSPIRE_7) || isKeyPressed(KEY_NSPIRE_9))
        action |= UP;
    if (isKeyPressed(KEY_NSPIRE_RIGHT) || isKeyPressed(KEY_NSPIRE_UPRIGHT) || isKeyPressed(KEY_NSPIRE_RIGHTDOWN) ||
	    isKeyPressed(KEY_NSPIRE_6) || isKeyPressed(KEY_NSPIRE_9) || isKeyPressed(KEY_NSPIRE_3))
        action |= RIGHT;
    if (isKeyPressed(KEY_NSPIRE_LEFT) || isKeyPressed(KEY_NSPIRE_DOWNLEFT) || isKeyPressed(KEY_NSPIRE_LEFTUP) ||
	    isKeyPressed(KEY_NSPIRE_4) || isKeyPressed(KEY_NSPIRE_7) || isKeyPressed(KEY_NSPIRE_3))
        action |= LEFT;
    if (isKeyPressed(KEY_NSPIRE_PLUS))
        action |= PGDOWN;
    if (isKeyPressed(KEY_NSPIRE_MINUS))
        action |= PGUP;
    if (isKeyPressed(KEY_NSPIRE_DIVIDE))
        action |= ZOOMOUT;
    if (isKeyPressed(KEY_NSPIRE_MULTIPLY))
        action |= ZOOMIN;
    return action;
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
                int torefresh = 0;
                if (current & DOWN) {
                    v.scrollDown();
                    torefresh = 1;
                } else if (current & UP) {
                    v.scrollUp();
                    torefresh = 1;
                }
                if (current & RIGHT) {
                    v.scrollRight();
                    torefresh = 1;
                } else if (current & LEFT) {
                    v.scrollLeft();
                    torefresh = 1;
                }
                if (current & PGDOWN) {
                    v.next();
                    torefresh = 1;
                } else if (current & PGUP) {
                    v.prev();
                    torefresh = 1;
                }
                if (current & ZOOMOUT) {
                    v.zoomOut();
                    torefresh = 1;
                } else if (current & ZOOMIN) {
                    v.zoomIn();
                    torefresh = 1;
                }
                if (torefresh) {
                        handleDelays(current, lastScrollKey);
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
		if (show_1numeric_input("Go to page", "", "Enter page number", &page, 1, v.getPages())) {
		    v.gotoPage(page - 1);
		    v.display();
		}
	    }
	}
	sleep(10);
    }
    
    return 0;
}

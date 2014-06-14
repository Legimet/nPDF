// nPDF viewer class
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
#include <cstdio>
#include <algorithm>
#include <libndls.h>
#include <syscall.h>
#include <syscall-list.h>
#include "Viewer.hpp"
#include "Screen.hpp"

const int Viewer::scroll = 20;
const float Viewer::zoom = 1.142857;
const unsigned char Viewer::bgColor = 103;

// We have a separate initialization method for the error handling
Viewer::Viewer() {
    ctx = nullptr;
    doc = nullptr;
    page = nullptr;
    pix = nullptr;
    dev = nullptr;
    scale = 1.0f;
    pageNo = 0;
    xPos = 0;
    yPos = 0;
    curPageLoaded = false;
    fitWidth = true;
    width = SCREEN_WIDTH;
    height = SCREEN_HEIGHT;
}

Viewer::~Viewer() {
    if (dev) {
	fz_free_device(dev);
    }
    if (pix) {
	fz_drop_pixmap(ctx, pix);
    }
    if (page) {
	fz_free_page(doc, page);
    }
    if (doc) {
	fz_close_document(doc);
    }
    if (ctx) {
	fz_free_context(ctx);
    }
}

bool Viewer::init() {
    ctx = fz_new_context(nullptr, nullptr, FZ_STORE_UNLIMITED);

    if (ctx) {
	fz_register_document_handlers(ctx);
	return true;
    } else {
	return false;
    }
}

fz_context* Viewer::getCtx() const {
    return ctx;
}

void Viewer::openDoc(const char *path) {
    fz_try(ctx) {
	doc = fz_open_document(ctx, path);
    }
    fz_catch(ctx) {
	show_msgbox("nPDF", "Can't open document");
	fz_throw(ctx, 1, "Can't open document");
    }
}

void Viewer::drawPage() {
    if (pix) {
	fz_drop_pixmap(ctx, pix);
	pix = nullptr;
    }
    
    if (!curPageLoaded) {
	if (page) {
	    fz_free_page(doc, page);
	    page = nullptr;
	}
	page = fz_load_page(doc, pageNo);
	curPageLoaded = true;
    }
    
    fz_matrix transform;
    fz_bound_page(doc, page, &bounds);
    if (fitWidth) {
	scale = width / (bounds.x1 - bounds.x0);
    }
    
    fz_scale(&transform, scale, scale);
    fz_transform_rect(&bounds, &transform);
    fz_irect bbox;
    fz_round_rect(&bbox, &bounds);
    
    if (xPos >= bounds.x1 - bounds.x0) {
	xPos = 0;
    }
    if (yPos >= bounds.y1 - bounds.y0) {
	xPos = 0;
    }
    
    if (has_colors) {
	pix = fz_new_pixmap_with_bbox(ctx, fz_device_rgb(ctx), &bbox);
    } else {
	pix = fz_new_pixmap_with_bbox(ctx, fz_device_gray(ctx), &bbox);
    }
    fz_clear_pixmap_with_value(ctx, pix, 0xff);
    
    dev = fz_new_draw_device(ctx, pix);
    fz_run_page(doc, page, dev, &transform, nullptr);
    if (dev) {
	fz_free_device(dev);
	dev = nullptr;
    }
}

void Viewer::display() {
    Screen::fillScreen(bgColor);
    // Center it
    int x = 0, y = 0;
    if (pix->w < width) {
	x = (width - pix->w) / 2;
    }
    if (pix->h < height) {
	y = (height - pix->h) / 2;
    }
    if (has_colors) {
	Screen::showImgRGBA(pix->samples, x, y, xPos, yPos, std::min(width, pix->w), std::min(height, pix->h), pix->w);
    } else {
	Screen::showImgGrayA(pix->samples, x, y, xPos, yPos, std::min(width, pix->w), std::min(height, pix->h), pix->w);
    }
    Screen::switchBufs();
}

void Viewer::next() {
    if (pageNo < fz_count_pages(doc) - 1) {
	pageNo++;
	curPageLoaded = false;
	drawPage();
	yPos = 0;
    }
}

void Viewer::prev() {
    if (pageNo > 0) {
	pageNo--;
	curPageLoaded = false;
	drawPage();
	yPos = std::max(0, (int)(bounds.y1 - bounds.y0) - height - 1);
    }
}

void Viewer::scrollUp() {
    if (yPos >= scroll) {
	yPos -= scroll;
    } else {
	prev();
    }
}

void Viewer::scrollDown() {
    if (yPos < (bounds.y1 - bounds.y0) - height - scroll) {
	yPos += scroll;
    } else {
	next();
    }
}

void Viewer::scrollLeft() {
    if (xPos >= scroll) {
	xPos -= scroll;
    }
}

void Viewer::scrollRight() {
    if (xPos < (bounds.x1 - bounds.x0) - width - scroll) {
	xPos += scroll;
    }
}

void Viewer::setFitWidth() {
    fitWidth = true;
    drawPage();
}

void Viewer::unsetFitWidth() {
    fitWidth = false;
}

void Viewer::zoomIn() {
    fitWidth = false;
    scale *= zoom;
    drawPage();
    xPos *= zoom;
    yPos *= zoom;
}

void Viewer::zoomOut() {
    fitWidth = false;
    scale /= zoom;
    drawPage();
    xPos /= zoom;
    yPos /= zoom;
}

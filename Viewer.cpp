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
#include <algorithm>
#include <libndls.h>
#include "Viewer.hpp"
#include "Screen.hpp"

const int Viewer::scroll = 20;
const float Viewer::zoom = 1.142857;
const unsigned char Viewer::bgColor = 103;
const float Viewer::maxScale = 2.0;
const float Viewer::minScale = 0.1;

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
	fz_throw(ctx, 1, "can't open document");
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
    
    // Make sure we don't go out of bounds
    if (xPos < 0 || bounds.x1 - bounds.x0 <= width) {
	xPos = 0;
    } else if (xPos >= (bounds.x1 - bounds.x0) - std::min(width, (int)(bounds.x1 - bounds.x0))) {
	xPos = (bounds.x1 - bounds.x0) - std::min(width, (int)(bounds.x1 - bounds.x0));
    }
    if (yPos < 0 || bounds.y1 - bounds.y0 <= height) {
	yPos = 0;
    } else if (yPos >= (bounds.y1 - bounds.y0) - std::min(height, (int)(bounds.y1 - bounds.y0))) {
	yPos = (bounds.y1 - bounds.y0) - std::min(height, (int)(bounds.y1 - bounds.y0));
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
	yPos = 0;
	drawPage();
    }
}

void Viewer::prev() {
    if (pageNo > 0) {
	pageNo--;
	curPageLoaded = false;
	yPos = std::max(0, (int)(bounds.y1 - bounds.y0) - height - 1);
	drawPage();
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
    // Try to zoom in on the center
    if (scale * zoom <= maxScale) {
	fitWidth = false;
	xPos = (xPos + std::min(width, (int)(bounds.x1 - bounds.x0)) / 2) * zoom;
	xPos -= std::min(width, (int)((bounds.x1 - bounds.x0) * zoom)) / 2;
	yPos = (yPos + std::min(height, (int)(bounds.y1 - bounds.y0)) / 2) * zoom;
	yPos -= std::min(height, (int)((bounds.y1 - bounds.y0) * zoom)) / 2;
	scale *= zoom;
	drawPage();
    }
}

void Viewer::zoomOut() {
    // Try to zoom out from the center
    if (scale / zoom >= minScale) {
	fitWidth = false;
	xPos = (xPos + std::min(width, (int)(bounds.x1 - bounds.x0)) / 2) / zoom;
	xPos -= std::min(width, (int)((bounds.x1 - bounds.x0) / zoom)) / 2;
	yPos = (yPos + std::min(height, (int)(bounds.y1 - bounds.y0)) / 2) / zoom;
	yPos -= std::min(height, (int)((bounds.y1 - bounds.y0) / zoom)) / 2;
	scale /= zoom;
	drawPage();
    }
}

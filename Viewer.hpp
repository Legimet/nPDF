// nPDF viewer class
// Copyright (C) 2014-2016  Legimet
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

#ifndef VIEWER_HPP
#define VIEWER_HPP

extern "C" {
#include <mupdf/fitz.h>
}

class Viewer {
	private:
		static const int scroll;
		static const float zoom;
		static const unsigned char bgColor;
		static const float maxScale;
		static const float minScale;
		fz_context *ctx;
		fz_document *doc;
		fz_page *page;
		fz_pixmap *pix;
		fz_rect bounds;
		fz_matrix transform;
		float scale;
		int pageNo;
		int xPos;
		int yPos;
		bool curPageLoaded;
		bool fitWidth;
		int width;
		int height;

		fz_text_page *pageText;
		fz_rect matches[512];
		int matchesCount;
		int matchIdx;
	public:
		Viewer();
		~Viewer();
		void invert(const fz_rect *rect);
		bool find(const char *s);
		bool findNext(bool dir);
		void openDoc(const char *path);
		int getPages();
		void drawPage();
		void display();
		void next();
		void prev();
		void scrollUp();
		void scrollDown();
		void scrollLeft();
		void scrollRight();
		void setFitWidth();
		void unsetFitWidth();
		void zoomIn();
		void zoomOut();
		void gotoPage(unsigned int page);
		void fixBounds();
};

#endif

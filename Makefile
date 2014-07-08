# nPDF makefile
# Copyright (C) 2014  Legimet
#
# This file is part of nPDF.
#
# nPDF is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# nPDF is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with nPDF.  If not, see <http://www.gnu.org/licenses/>.

MUPDF_BUILD = release
MUPDF_DIR = mupdf
MUPDF_INC = $(MUPDF_DIR)/include
MUPDF_OUT = $(MUPDF_DIR)/build/$(MUPDF_BUILD)

CXX = nspire-g++
LD = nspire-ld
CXXFLAGS = -Os -Wall -W -std=gnu++11 -marm -I $(MUPDF_INC)
MUPDF_XCFLAGS = -DNOCJKFONT -DNODROIDFONT
LDFLAGS = -L $(MUPDF_OUT) -lmupdf -lfreetype -ljbig2dec -ljpeg -lopenjpeg -lz -lm
OBJS = $(patsubst %.cpp,%.o,$(wildcard *.cpp))
LIBS = $(MUPDF_OUT)/libmupdf.a $(MUPDF_OUT)/libfreetype.a $(MUPDF_OUT)/libjbig2dec.a \
       $(MUPDF_OUT)/libjpeg.a $(MUPDF_OUT)/libopenjpeg.a $(MUPDF_OUT)/libz.a
EXE = nPDF.tns
DISTDIR = .
vpath %.tns $(DISTDIR)

all: $(EXE)

%.o: %.cpp $(MUPDF_DIR)
	$(CXX) $(CXXFLAGS) -c $<

$(MUPDF_OUT)/%.a: $(MUPDF_DIR) generate
	$(MAKE) -C $< build/$(MUPDF_BUILD)/$(notdir $@) build=release OS=ti-nspire \
	XCFLAGS="$(MUPDF_XCFLAGS)"

$(EXE): $(LIBS) $(OBJS)
	mkdir -p $(DISTDIR)
	$(LD) $(OBJS) -o $@.elf $(LDFLAGS)
	genzehn --input $@.elf --output $@ --name nPDF --author Legimet

generate: $(MUPDF_DIR)
	$(MAKE) -C $< generate build=release

clean: cleannolibs
	$(MAKE) -C $(MUPDF_DIR) clean build=$(MUPDF_BUILD)

cleannolibs:
	rm -f $(OBJS) $(EXE) $(EXE).gdb

.PHONY: all generate clean cleannolibs

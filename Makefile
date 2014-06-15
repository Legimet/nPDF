MUPDF_VERSION = 1.4
MUPDF_SRC = http://mupdf.com/downloads/mupdf-$(MUPDF_VERSION)-source.tar.gz

CXX = nspire-g++
LD = nspire-ld
CFLAGS = -Os -Wall -W -std=gnu++11 -marm -I mupdf-$(MUPDF_VERSION)-source/include
LDFLAGS = -L mupdf-$(MUPDF_VERSION)-source/build/release -lmupdf -lfreetype -ljbig2dec -ljpeg -lopenjpeg -lz -lm
OBJS = $(patsubst %.cpp,%.o,$(wildcard *.cpp))
EXE = nPDF.tns
DISTDIR = .
vpath %.tns $(DISTDIR)

all: $(EXE)

%.o: %.cpp mupdf-$(MUPDF_VERSION)-source
	$(CXX) $(CFLAGS) -c $<

$(EXE): $(OBJS)
	mkdir -p $(DISTDIR)
	$(LD) $(OBJS) -o $(DISTDIR)/$@ $(LDFLAGS)

libs: mupdf-$(MUPDF_VERSION)-source
	$(MAKE) -C $< generate
	$(MAKE) -C $< OS=ti-nspire build=release XCFLAGS="-DNOCJKFONT -DNODROIDFONT" libs third

mupdf-$(MUPDF_VERSION)-source: mupdf-$(MUPDF_VERSION)-source.tar.gz patches/mupdf.patch patches/openjpeg.patch
	tar -xzvf $<
	patch -d $@ -p1 < patches/mupdf.patch
	patch -d $@/thirdparty/openjpeg -p1 < patches/openjpeg.patch

mupdf-$(MUPDF_VERSION)-source.tar.gz:
	wget $(MUPDF_SRC) -O $@

clean:
	rm -rf mupdf-$(MUPDF_VERSION)-source $(OBJS) $(EXE) $(EXE).gdb

cleannolibs:
	rm -f $(OBJS) $(EXE) $(EXE).gdb

spotless: clean
	rm -f mupdf-$(MUPDF_VERSION)-source.tar.gz

.PHONY: all libs clean cleannolibs spotless

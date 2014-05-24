MUPDF_VERSION = 1.4
MUPDF_SRC = http://mupdf.com/downloads/mupdf-$(MUPDF_VERSION)-source.tar.gz
MUPDF_MD5SUM = 0f6840a7020db0c833b0c090ca1864ec

libs: mupdf-$(MUPDF_VERSION)-source
	$(MAKE) -C $< generate
	$(MAKE) -C $< OS=ti-nspire build=release libs third

mupdf-$(MUPDF_VERSION)-source: mupdf-$(MUPDF_VERSION)-source.tar.gz patches/mupdf.patch patches/openjpeg.patch
	tar -xzvf $<
	patch -d $@ -p1 < patches/mupdf.patch
	patch -d $@/thirdparty/openjpeg -p1 < patches/openjpeg.patch

mupdf-$(MUPDF_VERSION)-source.tar.gz:
	wget $(MUPDF_SRC) -O $@

clean:
	rm -rf mupdf-$(MUPDF_VERSION)-source.tar.gz mupdf-$(MUPDF_VERSION)-source

.PHONY: libs clean

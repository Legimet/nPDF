DEBUG ?= no
ifeq ($(DEBUG),no)
	MUPDF_BUILD ?= release
	OPTIMIZE ?= 3
else
	MUPDF_BUILD ?= debug
	OPTIMIZE ?= g
endif

MUPDF_VERBOSE ?= no
MUPDF_DIR := mupdf
MUPDF_INC := $(MUPDF_DIR)/include
MUPDF_OUT := $(MUPDF_DIR)/build/$(MUPDF_BUILD)
MUPDF_XCFLAGS := -DNOCJK

CXX := nspire-g++
CXXFLAGS := -O$(OPTIMIZE) -Wall -Wextra -std=gnu++11 -marm -I $(MUPDF_INC)
ifeq ($(OPTIMIZE),g)
	CXXFLAGS += -g
endif
NPDF_LDFLAGS = -L $(MUPDF_OUT) -lmupdf -lfreetype -ljbig2dec -ljpeg -lopenjpeg -lz -lm
ZEHNFLAGS := --compress --name "nPDF" --author "Legimet" --notice "Document viewer"
OBJS := $(patsubst %.cpp,%.o,$(wildcard *.cpp))
LIBS := $(patsubst %,$(MUPDF_OUT)/lib%.a,mupdf jbig2dec jpeg openjpeg)
EXE := nPDF

all: $(EXE).tns

%.o: %.cpp $(MUPDF_DIR)

$(MUPDF_OUT)/libmupdf.a: $(MUPDF_DIR) generate

$(MUPDF_OUT)/%.a: $(MUPDF_DIR)
	$(MAKE) -C $< build/$(MUPDF_BUILD)/$(notdir $@) verbose=$(MUPDF_VERBOSE) build=$(MUPDF_BUILD) OS=ti-nspire XCFLAGS="$(MUPDF_XCFLAGS)"

$(EXE).elf: $(LIBS) $(OBJS)
	$(CXX) $(OBJS) -o $@ $(NPDF_LDFLAGS)

$(EXE).zehn.tns: $(EXE).elf
	genzehn --input $^ --output $@ $(ZEHNFLAGS)

$(EXE).tns: $(EXE).zehn.tns
	make-prg $^ $@

generate: $(MUPDF_DIR)
	$(MAKE) -C $< generate verbose=$(MUPDF_VERBOSE) build=$(MUPDF_BUILD)

clean: cleannolibs
	-$(MAKE) -C $(MUPDF_DIR) clean build=$(MUPDF_BUILD)

cleannolibs:
	$(RM) $(OBJS) $(EXE).elf $(EXE).zehn.tns $(EXE).tns

.PHONY: all generate clean cleannolibs

# nPDF 0.2

nPDF is a document viewer for TI-Nspire handheld calculators. It supports PDF,
XPS, CBZ, and various image formats. You will need Ndless 3.1 or 3.6 to run it.

## Controls

* Up/8: Scroll up
* Down/2: Scroll down
* Left/4: Scroll left
* Right/6: Scroll right
* Multiply: Zoom in
* Divide: Zoom out
* Esc: Exit

More controls will be added later.

## Libraries used

The following libraries are used by nPDF.

* MuPDF
* FreeType
* jbig2dec
* libjpeg
* OpenJPEG
* zlib

MuPDF is included, and the others are submodules.

## Compiling from source

The source code can be found on [GitHub](https://github.com/Legimet/nPDF/) or
[Gitorious](https://gitorious.org/npdf/npdf). Make sure you get all of the
submodules.

You must Use [Fabian Vogt's fork of Ndless](https://github.com/Vogtinator/Ndless/).
Simply run `make` to compile.

## License

nPDF is licensed under the GPLv3+:

nPDF is Copyright (C) 2014 Legimet.

nPDF is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

nPDF is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with nPDF.  If not, see <http://www.gnu.org/licenses/>.

## Links

* Omnimaga thread: http://www.omnimaga.org/ti-nspire-projects/npdf-a-document-viewer-for-the-nspire/
* Ndless: http://ndless.me
* MuPDF: http://mupdf.com

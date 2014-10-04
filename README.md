# nPDF 0.2

nPDF is a document viewer for TI-Nspire handheld calculators. It supports PDF,
XPS, CBZ, and various image formats. Ndless 3.1 or 3.6 is required.

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

The source code can be found on [Gitorious](https://gitorious.org/npdf/npdf) or
[GitHub](https://github.com/Legimet/nPDF/). Make sure you get only the submodules
jbig2dec, jpeg, and openjpeg. If you get the freetype or zlib submodules, they
will conflict with the ones provided in the Ndless SDK, and nPDF won't compile.

You must have the [Ndless SDK](http://ndlessly.wordpress.com/ndless-for-developers/)
installed.

Simply run `make` to compile nPDF.

## License

nPDF is licensed under the GNU GPL, version 3 or above:

> nPDF is Copyright (C) 2014 Legimet.
> 
> nPDF is free software: you can redistribute it and/or modify
> it under the terms of the GNU General Public License as published by
> the Free Software Foundation, either version 3 of the License, or
> (at your option) any later version.
>
> nPDF is distributed in the hope that it will be useful,
> but WITHOUT ANY WARRANTY; without even the implied warranty of
> MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
> GNU General Public License for more details.
> 
> You should have received a copy of the GNU General Public License
> along with nPDF.  If not, see <http://www.gnu.org/licenses/>.

The licenses of MuPDF and its dependencies, which are all free software, can be
found in their respective directories.

## Links

* [Omnimaga thread](http://www.omnimaga.org/ti-nspire-projects/npdf-a-document-viewer-for-the-nspire/)
* [Ndless](http://ndless.me)
* [MuPDF](http://mupdf.com)

# nPDF 0.3

nPDF is a document viewer for TI-Nspire handheld calculators using MuPDF 1.6.
It supports PDF, XPS, CBZ, and various image formats. [Ndless](http://ndless.me)
is required.

## Controls

* Touchpad/Numpad: Scrolling
* ×: Zoom in
* ÷: Zoom out
* -: Previous page
* +: Next page
* Ctrl+G: Go to page
* Esc: Exit

More controls will be added later.

## Compiling from source

You must have the [Ndless SDK](http://ndlessly.wordpress.com/ndless-for-developers/)
installed.

The source code can be found on [GitHub](https://github.com/Legimet/nPDF/) or
[Gitorious](https://gitorious.org/npdf/npdf).

Once you have the code, clone submodules jbig2dec, jpeg, and openjpeg:

    git submodule init
    git submodule update mupdf/thirdparty/jbig2dec mupdf/thirdparty/jpeg mupdf/thirdparty/openjpeg

(Do **not** clone the freetype or zlib modules. They will conflict with the ones
provided in the Ndless SDK.)

Simply run `make` to compile nPDF.

## Libraries used

The following libraries are used by nPDF.

* [MuPDF](http://mupdf.com/) (modified, October 4, 2014)
* [FreeType](http://freetype.org/)
* [jbig2dec](http://git.ghostscript.com/?p=jbig2dec.git)
* [libjpeg](http://ijg.org/)
* [OpenJPEG](http://www.openjpeg.org/) (modified, July 5, 2014)
* [zlib](http://zlib.net/)

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
* [MuPDF](http://mupdf.com)
* [Ndless](http://ndless.me)

## Credits

Thanks to the following people:

* Xavier Andréani ([critor](https://github.com/critor)) - improvements in the
display, scrolling, and controls
* blauemauritius - feature requests
* Adrien Bertrand ([adriweb](https://github.com/adriweb)) - feature requests

And thanks to everyone else who reported bugs or requested features, along with
the developers of Ndless for making this possible!

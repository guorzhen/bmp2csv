# bmp2csv
This is a BMP to grayscale CSV converter, still under development.

It already produces correct results (as a pixel table parser) with BMP file format limited to:

* DIB header version: BITMAPINFOHEADER (see definition [here](https://msdn.microsoft.com/en-us/library/windows/desktop/dd183376(v=vs.85).aspx))
* Color depth: 1 / 2 / 4 / 8 / 16 / 24 / 32 (bits per pixel)
* Compression: none

# bmp2csv
This is a BMP pixel table parser, output in CSV format.

It works with BMP file format limited to:

* DIB header version: BITMAPINFOHEADER (see definition [here](https://msdn.microsoft.com/en-us/library/windows/desktop/dd183376(v=vs.85).aspx))
* Color depth: 1 / 2 / 4 / 8 / 16 / 24 / 32 (bits per pixel)
* Compression: none

## Usage
mode:

      0 - RGBA (default, optional)

      1 - BT.601 / SDTV

      2 - BT.709 / HDTV

Linux:
```shell
./bmp2csv in.bmp [mode] > out.csv
```
Windows:
```shell
bmp2csv in.bmp [mode] > out.csv
```

# ZIP File reader
## Overview

The goal of this project is to decode a flat ZIP file, read the file directory, and read the file contents.

## Tier 1

 - Read a non-compressed ZIP file's central directory record, and the various file headers. Display for each file the full path inside the ZIP file, and the file size.

## Tier 2

 - Unzip in the local directory the contents of a flat (non compressed) ZIP file.

## Tier 3

 - Preserve the file's "modified" and "created" dates
 - Add support for the DEFLATE compression method with the [zlib](http://www.zlib.net/) library

## Tips

 - See the [wikipedia page on the ZIP format](https://en.wikipedia.org/wiki/Zip_(file_format)) for how to decode zip files
 - Use `std::ifstream` in binary mode to read the file

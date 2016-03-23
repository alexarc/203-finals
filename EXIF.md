# EXIF Harvester
## Overview

The goal of this assignment is to read GPS coordinates from geotagged JPEG files and display the locations on a map.

## Tier 1

Use the specifications of the [JPEG Format](https://en.wikipedia.org/wiki/JPEG), 
the [TIFF Format](https://en.wikipedia.org/wiki/Tagged_Image_File_Format) and the 
[EXIF Format](https://en.wikipedia.org/wiki/Exchangeable_image_file_format) to extract geographic coordinates from a picture.

You can use the [test picture](https://github.com/kbok/203-finals/blob/master/help/test3.jpg) provided in this repository to test
your program. 

## Tier 2

Use [ShellExecute()](https://msdn.microsoft.com/en-us/library/windows/desktop/bb762153(v=vs.85).aspx) to open a
[Google Static Map](http://stackoverflow.com/questions/1771879/url-for-multiple-markers-in-google-maps) showing the location of where
the picture was taken.

## Tier 3

 - Read several pictures from a directory, and display the location of the picture all on the same map.
 - Read the time to assign an incremental number to each picture
 - Group pictures that were taken close to each other

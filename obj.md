# OBJ File Renderer
## Overview

The goal of this assignment is to create a program able to render a wireframe view of a 3D model into an image file.

## Tier 1

 - Read from an [OBJ file](https://en.wikipedia.org/wiki/Wavefront_.obj_file) a list of vertrices and faces composing the 3D model. Faces will not be rendered.
 - Normalize each vertrex's coordinates such that the loaded model is entirely contained in a cube of length 1 and center (0, 0, 0).
 - Project all vertrices to the plane normal to the vector (1, 1, 1).
 - Output, in a corresponding text file, a list of vertrices with their projected coordinates in 2D. 
 - Output in the same file a list of edges computed from the face coordinates. 

## Tier 2

 - Create a [BMP file](https://en.wikipedia.org/wiki/BMP_file_format) of size 512x512 with a white background. 
 - Render each edge from the projected list as a black line of pixels.

## Tier 3

 - Support rotating the 3D object
 - Smooth lines in the rendered output

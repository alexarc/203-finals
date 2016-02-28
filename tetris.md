# Tetris
## Overview

The goal of this assignment is to create a console-mode Tetris game where blocks will be rendered as ASCII characters.

## Tier 1

 - The Tetris board is 10 blocks wide and 20 blocks tall. 
 - Read from a file a sequence of blocks specified as 3x3 character blocks, followed by the horizontal offset of the block.
 - Calculate the final state of the board and the final score, given that each line award 100 points.
 - Output on the console the final state, as well as the score and the number of blocks placed.

## Tier 2

 - Use the console API to read keyboard input and show the game board in real-time.
 - Use a random number generator (see [std::rand()](http://en.cppreference.com/w/cpp/numeric/random/rand)) to generate blocks
 - Add the possibility for the player to rotate pieces

## Tier 3

 - Display in a box next to the board the oncoming piece
 - Display the score in real-time
 - Add colors to the whole game, especially give each piece its own color
 - Accelerate the game as the score increases

## Tips

 - Check out the [Console Example](help/console_sample.cpp) to see how to implement most of Tier 2 features
 - It's probably best to hardcode pieces into the program to get an even distribution

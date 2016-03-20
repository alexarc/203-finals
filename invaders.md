# Space Invaders
## Overview

This assignment is about implementing a console based clone of the Space Invaders game. The player will move ASCII-based graphics across the screen.

## Tier 1

Read from a text file:
 - The contents of the upper part of the screen, which contains the aliens. Each alien will be represented by a single character, and empty space by space characters.
 - The width and height of the alien moving space, which should be larger than the actual aliens block so that they can move.
 - A list of entries specifying when and where the player fires the cannon: when is a tick, the basic game time unit, and where is the horizontal coordinate from which the cannon is fired.

From these inputs, process the outcome of the game, given that:
 - The aliens move horizontally by one column every ten ticks. If they hit the end of the screen, they move vertically by one line, and change their horizontal direction
 - The game is lost when aliens touch the player's row (the bottom one)
 - A cannon shell moves upward every tick

Output in the console the details of what is happening in the game (which aliens are killed, etc.) and the outcome of the game.

## Tier 2

Use the Windows Console API to display the game in real-time. The player should be able to move sideways with the keyboard, and to shoot shells on the aliens. Display a message at the end of the game indicating if the player has won or lost.

## Tier 3

 - Display a score on-screen counting how many aliens have been killed
 - Add some colors, or display Unicode characters for more scary aliens
 - Let the aliens shoot too, and make the player loses if he is hit
 - Add several levels with increasing difficulty

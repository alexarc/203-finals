# Snake
## Overview

The goal of this project is to replicate the popular snake game, in the windows console. 
The matrix display of ASCII characters will be used to represent the game's state, thus implementing a basic
but usable graphics device.

 - In the first part, the game's logic will be implemented: How the snake moves, how it reacts to touching apples,
and walls. 
 - In the second part, the game's logic will be combined with console IOs to build a playable game. 
 - In the third part, additional gameplay and technical features will be added to the game.

## Tier 1

Build a two-dimensional grid of cells. Those cells will be the space in which the game will take place.
Read from a file a sequence of events, each happening at a specified time, where time is measured in game ticks. 
The events can be of three types:
 - The game starts. This must be the first event. In the event is specified the size of the grid, the position of the snake's head,
and the snake's length.
 - The snake changes direction. Directions are up, down, left and right.
 - An apple is created on the board. The apple's coordinates is specified in the event entry.
 
From a given file input, display the number of steps the snake will survive, its final length, and the final view of the
game board, given that:
 - Eating an apple increases the snake's length by 1
 - The snake dies if it hits either itself or a wall
 - The snakes moves by one cell in its current direction per tick.
 
## Tier 2

- Use the windows console API to display the game board in real-time, while waiting a bit before each step. Use different colors 
for the snake and the apples. 
- Use the console API to read keyboard input in real time.
- Use a random number generator to create apples at random places on the board, with a probability of creation at each step.
 
Organize your code so that the I/O classes are separated from the game logic classes.

## Tier 3

- Display on-screen at all times the current score of the player. When the game ends, let the user input its score and name. 
Keep a leaderboard of best scores in a local file.
- Use QueryPerformanceCounter() to stabilize display frequency at exactly 24 frames per second.
- Detect window resizes and change board size accordingly.
- Accelerate the game as the score goes higher.

## Tips

- `std::list` is probably a good idea for keeping track of the snake's position. 
- At time 0, the snake starts motionless, before the direction is set.
- Look at the examples to see how to read keyboard input and print to the console using the console API.
- Generate random numbers with `std::rand`

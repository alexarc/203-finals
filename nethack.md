# Nethack
## Overview

The goal of this assignment is to create a 2D adventure game in which the player evolves in a randomly generated dungeon and fights monsters.
The graphics will be made using ASCII characters in a terminal window.

## Tier 1

Read from a text file a plan of the dungeon. The plan will be a map of the rooms, as should be displayed on the screen. Some characters have a special significance: Floor, Wall, Enemy and Character.
Read from a second text file a list of commands. Commands include: Move (left, right, up, down) and Fight, which kills a nearby monster.
From the map file and the command file, calculate the final position of the character and the number of enemies killed.

## Tier 2

Use the windows Console API to display the game on-screen, with a scrolling system that follows the character.
Use the Console API to read commands from the keyboard in real-time.

##Tier 3

Add doors that the character must open to cross.
Add a Health Point system and a game termination condition.

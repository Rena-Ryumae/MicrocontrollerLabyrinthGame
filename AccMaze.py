#!/usr/bin/env python
# -*- coding: utf-8 -*-

#the next line is only needed for python2.x and not necessary for python3.x
from __future__ import print_function, division

import pygame, array
pygame.init()

import sys, os, random, math
from pygame.locals import *

# Define some colors
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
GREEN = (0, 255, 0)
RED = (255, 0, 0)

# This sets the WIDTH and HEIGHT of each box
BOX = 50

# Length of entire maze
SIZE = 2

# This sets the margin between each cell
MARGIN = 6

# start will be true when the first maze comes in where ball is at (0,0)
START = True

###### TEMPORARY hardcoded grid whose info will be communicated from board ###############
GRID = [[26, 9],[6, 5]]

##########################################################################################
GRID_INITIAL = [[]]

# Set the HEIGHT and WIDTH of the screen
WINDOW_S = ((SIZE + 1) * MARGIN) + (SIZE * BOX)
WINDOW_SIZE = [WINDOW_S, WINDOW_S]
screen = pygame.display.set_mode(WINDOW_SIZE)

# Set title of screen
pygame.display.set_caption("Accelerometer Maze Game")

# Loop until the user clicks the close button.
done = False

# Used to manage how fast the screen updates
clock = pygame.time.Clock()

#### TODO: Modify this function to draw walls given a grid
def draw_grid():
    # Draw the grid
    for row in range(SIZE):
        for column in range(SIZE):
            color = WHITE
            if START & (row == 0) & (column == 0):
                print(row,column)
                color = GREEN


            pygame.draw.rect(screen,
                             color,
                             [(MARGIN + BOX) * column + MARGIN,
                              (MARGIN + BOX) * row + MARGIN,
                              BOX,
                              BOX])


# -------- Main Program Loop -----------
while not done:
    for event in pygame.event.get():  # User did something
        if event.type == pygame.QUIT:  # If user clicked close
            done = True  # Flag that we are done so we exit this loop
        elif event.type == pygame.MOUSEBUTTONDOWN:
            # User clicks the mouse. Get the position
            pos = pygame.mouse.get_pos()
            # Change the x/y screen coordinates to grid coordinates
            column = pos[0] // (BOX + MARGIN)
            row = pos[1] // (BOX + MARGIN)
            # Set that location to one
            # grid[row][column] = 1
            print("Click ", pos, "Grid coordinates: ", row, column)
    # Set the screen background
    screen.fill(BLACK)

    ### TODO: Modify this section so that it keeps the grid until told to change
    # Start situation
    if START:
        # Preserve original maze contents
        GRID_INITIAL = GRID
        draw_grid()
        START = False
    draw_grid()

    # Limit to 60 frames per second
    clock.tick(60)

    # Go ahead and update the screen with what we've drawn.
    pygame.display.flip()

# Be IDLE friendly. If you forget this line, the program will 'hang'
# on exit.
pygame.quit()
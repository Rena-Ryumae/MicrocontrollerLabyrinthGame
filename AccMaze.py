#!/usr/bin/env python
# -*- coding: utf-8 -*-

#the next line is only needed for python2.x and not necessary for python3.x
from __future__ import print_function, division

import pygame
pygame.init()

# Some basic colors
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
GREEN = (0, 255, 0)
RED = (255, 0, 0)

# BOX is the width and height of one box
BOX = 50

# WALLCOLOR is wall color
WALLCOLOR = BLACK

# SIZE x SIZE board
SIZE = 10

# MARGIN is the wall/white space between each cell
MARGIN = 6

# START will be true when the first maze comes in where ball is at (0,0)
START = True

###### TEMPORARY hardcoded grid whose info will be communicated from board ###############
#GRID = [[26, 9],[6, 5]]
#GRID = [[10, 22],[6, 5]]

GRID = [[27,14,8,12,12,8,13,10,9,15],
        [3,14,4,8,13,3,14,1,2,13],
        [6,12,9,2,9,2,9,3,3,15],
        [11,11,3,6,1,2,5,3,6,9],
        [2,5,3,14,1,3,14,1,10,5],
        [2,12,5,10,5,6,9,3,2,9],
        [6,9,10,4,9,10,1,3,7,3],
        [14,1,6,13,3,2,5,3,10,5],
        [15,3,10,12,1,6,12,5,6,9],
        [6,4,4,13,6,12,12,12,13,7]]


##########################################################################################

#### NOT SURE IF NECESSARY YET ####
# GRID_INITIAL will save the initial board
GRID_INITIAL = [[]]

# Set the height and width of the screen, based on number of boxes
WINDOW_S = ((SIZE + 1) * MARGIN) + (SIZE * BOX)
WINDOW_SIZE = [WINDOW_S, WINDOW_S]
screen = pygame.display.set_mode(WINDOW_SIZE)   # screen is a pygame Surface object

# Set title of screen
pygame.display.set_caption("Accelerometer Maze Game")

# Loop until the user clicks the close button.
done = False

# Used to manage how fast the screen updates
clock = pygame.time.Clock()

# Draws right wall on screen
def wall_right(row, column, wallc):
    pygame.draw.rect(screen,
                     wallc,
                     [(MARGIN + BOX) * column + MARGIN + BOX,
                      (MARGIN + BOX) * row,
                      MARGIN,
                      (2 * MARGIN) + BOX])

# Draws left wall on screen
def wall_left(row, column, wallc):
    pygame.draw.rect(screen,
                     wallc,
                     [(MARGIN + BOX) * column,
                      (MARGIN + BOX) * row,
                      MARGIN,
                      (2 * MARGIN) + BOX])

# Draws top wall on screen
def wall_up(row, column, wallc):
    pygame.draw.rect(screen,
                     wallc,
                     [(MARGIN + BOX) * column,
                      (MARGIN + BOX) * row,
                      (2 * MARGIN) + BOX,
                      MARGIN])

# Draws bottom wall on screen
def wall_down(row, column, wallc):
    pygame.draw.rect(screen,
                     wallc,
                     [(MARGIN + BOX) * column,
                      (MARGIN + BOX) * row + MARGIN + BOX,
                      (2 * MARGIN) + BOX,
                      MARGIN])

# Returns white if v is 0 and WALLCOLOR otherwise
def get_color(v):
    if v == 0:
        return WHITE
    else:
        return WALLCOLOR

# Adds walls to screen given 0 or 1 for u, d, l, and r
def add_walls(u,d,l,r, row, column):
    wall_up(row, column, get_color(u))
    wall_down(row, column, get_color(d))
    wall_left(row, column, get_color(l))
    wall_right(row, column, get_color(r))

# Picks which walls to add based on given v
def pick_walls(v, row, column):
    if v == 0:
        add_walls(0, 0, 0, 0, row, column)
    elif v == 1:
        add_walls(0, 0, 0, 1, row, column)
    elif v == 2:
        add_walls(0, 0, 1, 0, row, column)
    elif v == 3:
        add_walls(0, 0, 1, 1, row, column)
    elif v == 4:
        add_walls(0, 1, 0, 0, row, column)
    elif v == 5:
        add_walls(0, 1, 0, 1, row, column)
    elif v == 6:
        add_walls(0, 1, 1, 0, row, column)
    elif v == 7:
        add_walls(0, 1, 1, 1, row, column)
    elif v == 8:
        add_walls(1, 0, 0, 0, row, column)
    elif v == 9:
        add_walls(1, 0, 0, 1, row, column)
    elif v == 10:
        add_walls(1, 0, 1, 0, row, column)
    elif v == 11:
        add_walls(1, 0, 1, 1, row, column)
    elif v == 12:
        add_walls(1, 1, 0, 0, row, column)
    elif v == 13:
        add_walls(1, 1, 0, 1, row, column)
    elif v == 14:
        add_walls(1, 1, 1, 0, row, column)
    elif v == 15:
        add_walls(1, 1, 1, 1, row, column)
    # If v >= 16, then ball is located there! Recurse pick_walls to find correct walls
    else:
        pick_walls(v - 16, row, column)

def draw_grid(grid):
    # Draw the grid
    for row in range(SIZE):
        for column in range(SIZE):
            # value is provided from microcontroller board
            value = grid[row][column]

            # Creates walls
            pick_walls(value, row, column)

            # Creates box
            pygame.draw.rect(screen,
                             WHITE,
                             [(MARGIN + BOX) * column + MARGIN,
                              (MARGIN + BOX) * row + MARGIN,
                              BOX,
                              BOX])

            # Ball
            if START & (value >= 16):
                pygame.draw.circle(screen,
                                   GREEN,
                                   [(MARGIN + BOX) * column + MARGIN + (int(BOX / 2)),
                                    (MARGIN + BOX) * row + MARGIN + (int(BOX / 2))],
                                   int(BOX / 2))


# -------- Main Program Loop -----------
while not done:
    for event in pygame.event.get():  # User did something
        if event.type == pygame.QUIT:  # If user clicked close
            done = True  # Flag that we are done so we exit this loop

    # Set the screen background
    screen.fill(WHITE)

    # Start situation
    if START:
        # Preserve original maze contents (MAY NOT NEED TO DO THIS)
        GRID_INITIAL = GRID
        draw_grid(GRID_INITIAL)
        if GRID_INITIAL[0][0] < 16:
            START = False
    draw_grid(GRID)

    # Limit to 60 frames per second
    clock.tick(60)

    # Go ahead and update the screen with what we've drawn.
    pygame.display.flip()

# Be IDLE friendly. If you forget this line, the program will 'hang'
# on exit.
pygame.quit()
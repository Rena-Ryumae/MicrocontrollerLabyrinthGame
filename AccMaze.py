#!/usr/bin/env python
# -*- coding: utf-8 -*-

#########################################################################
# This file ONLY creates the GUI and displays it. All other processes
# are done solely on the FRDM board!
#########################################################################

# the next line is only needed for python2.x and not necessary for python3.x
from __future__ import print_function, division

import pygame, serial

# Some basic colors
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
GREEN = (75, 248, 58)
RED = (255, 0, 0)
BLUE = (40, 250, 247)

# WALLCOLOR is wall color
WALLCOLOR = BLACK

# MARGIN is the wall/white space between each cell
MARGIN = 6


###### TEMPORARY hardcoded grid whose info will be communicated from board ###############
# GRID = [[26, 9],[6, 5]]
# GRID = [[10, 22],[6, 5]]

# GRID = [[27,14,8,12,12,8,13,10,9,15],
#         [3,14,4,8,13,3,14,1,2,13],
#         [6,12,9,2,9,2,9,3,3,15],
#         [11,11,3,6,1,2,5,3,6,9],
#         [2,5,3,14,1,3,14,1,10,5],
#         [2,12,5,10,5,6,9,3,2,9],
#         [6,9,10,4,9,10,1,3,7,3],
#         [14,1,6,13,3,2,5,3,10,5],
#         [15,3,10,12,1,6,12,5,6,9],
#         [6,4,4,13,6,12,12,12,13,7]]
#
# FINALX = 9
# FINALY = 9
##########################################################################################

# Draws right wall on screen
def wall_right(row, column, wallc, screen, BOX):
    pygame.draw.rect(screen,
                     wallc,
                     [(MARGIN + BOX) * column + MARGIN + BOX,
                      (MARGIN + BOX) * row,
                      MARGIN,
                      (2 * MARGIN) + BOX])


# Draws left wall on screen
def wall_left(row, column, wallc, screen, BOX):
    pygame.draw.rect(screen,
                     wallc,
                     [(MARGIN + BOX) * column,
                      (MARGIN + BOX) * row,
                      MARGIN,
                      (2 * MARGIN) + BOX])


# Draws top wall on screen
def wall_up(row, column, wallc, screen, BOX):
    pygame.draw.rect(screen,
                     wallc,
                     [(MARGIN + BOX) * column,
                      (MARGIN + BOX) * row,
                      (2 * MARGIN) + BOX,
                      MARGIN])


# Draws bottom wall on screen
def wall_down(row, column, wallc, screen, BOX):
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
def add_walls(u, d, l, r, row, column, screen, BOX):
    wall_up(row, column, get_color(u), screen, BOX)
    wall_down(row, column, get_color(d), screen, BOX)
    wall_left(row, column, get_color(l), screen, BOX)
    wall_right(row, column, get_color(r), screen, BOX)


# Picks which walls to add based on given v
def pick_walls(v, row, column, screen, BOX):
    if v == 0:
        add_walls(0, 0, 0, 0, row, column, screen, BOX)
    elif v == 1:
        add_walls(0, 0, 0, 1, row, column, screen, BOX)
    elif v == 2:
        add_walls(0, 0, 1, 0, row, column, screen, BOX)
    elif v == 3:
        add_walls(0, 0, 1, 1, row, column, screen, BOX)
    elif v == 4:
        add_walls(0, 1, 0, 0, row, column, screen, BOX)
    elif v == 5:
        add_walls(0, 1, 0, 1, row, column, screen, BOX)
    elif v == 6:
        add_walls(0, 1, 1, 0, row, column, screen, BOX)
    elif v == 7:
        add_walls(0, 1, 1, 1, row, column, screen, BOX)
    elif v == 8:
        add_walls(1, 0, 0, 0, row, column, screen, BOX)
    elif v == 9:
        add_walls(1, 0, 0, 1, row, column, screen, BOX)
    elif v == 10:
        add_walls(1, 0, 1, 0, row, column, screen, BOX)
    elif v == 11:
        add_walls(1, 0, 1, 1, row, column, screen, BOX)
    elif v == 12:
        add_walls(1, 1, 0, 0, row, column, screen, BOX)
    elif v == 13:
        add_walls(1, 1, 0, 1, row, column, screen, BOX)
    elif v == 14:
        add_walls(1, 1, 1, 0, row, column, screen, BOX)
    elif v == 15:
        add_walls(1, 1, 1, 1, row, column, screen, BOX)
    # If v >= 16, then ball is located there! Recurse pick_walls to find correct walls
    else:
        pick_walls(v - 16, row, column, screen, BOX)


def draw_grid(grid, SIZE, screen, BOX, FINALX, FINALY):
    # Draw the grid
    for row in range(SIZE):
        for column in range(SIZE):
            # value is provided from microcontroller board
            value = grid[row][column]

            # Creates walls
            pick_walls(value, row, column, screen, BOX)

            # Creates box
            pygame.draw.rect(screen,
                             WHITE,
                             [(MARGIN + BOX) * column + MARGIN,
                              (MARGIN + BOX) * row + MARGIN,
                              BOX,
                              BOX])

            # End square
            if (row == FINALX) & (column == FINALY):
                pygame.draw.rect(screen,
                                 RED,
                                 [(MARGIN + BOX) * column + MARGIN,
                                  (MARGIN + BOX) * row + MARGIN,
                                  BOX,
                                  BOX])

            # Ball
            if (value >= 16):
                pygame.draw.circle(screen,
                                   GREEN,
                                   [(MARGIN + BOX) * column + MARGIN + (int(BOX / 2)),
                                    (MARGIN + BOX) * row + MARGIN + (int(BOX / 2))],
                                   int(BOX / 2))


def main():
    # Open serial port
    ser = serial.Serial()
    ser.timeout = None
    ser.baudrate = 115200
    ser.port = 'COM3'
    ser.open()
    
    # Initialize first screen
    pygame.init()
    myfont = pygame.font.SysFont('sitkasmallsitkatextsitkasubheadingsitkaheadingsitkadisplaysitkabanner', 30)

    WINDOW_SIZE = [500, 500]
    screen = pygame.display.set_mode(WINDOW_SIZE)  # screen is a pygame Surface object
    pygame.display.set_caption("Accelerometer Maze Game")
    clock = pygame.time.Clock()
    screen.fill(WHITE)
    i1 = myfont.render('Microcontroller-based ', True, BLACK)
    i2 = myfont.render('Labyrinth Game', True, BLACK)
    i3 = myfont.render('Rena Ryumae', True, BLACK)
    i4 = myfont.render('&', True, BLACK)
    i5 = myfont.render('Ryan Hornung', True, BLACK)
    i6 = myfont.render('Please press the reset button', True, BLACK)
    i7 = myfont.render('on the FRDM board prior to ', True, BLACK)
    i8 = myfont.render('starting. Have fun!! ', True, BLACK)

    TMPCT = 0
    done = False
    initialize = False
    intro = False
    while (intro == False):
        for event in pygame.event.get():  # User did something
            if event.type == pygame.QUIT:  # If user clicked close
                intro = True  # Flag that we are done so we exit this loop
                initialize = True
                done = True
        # Only display screen for short amount of time
        if (TMPCT == 100):
            intro = True
        TMPCT = TMPCT + 1
        pygame.draw.rect(screen,
                         BLUE,
                         [0, 0, 500, 500])
        screen.blit(i1, (90, 55))
        screen.blit(i2, (140, 85))
        screen.blit(i3, (90, 175))
        screen.blit(i4, (225, 205))
        screen.blit(i5, (180, 235))
        screen.blit(i6, (30, 335))
        screen.blit(i7, (40, 375))
        screen.blit(i8, (90, 405))

        clock.tick(60)
        pygame.display.flip()

    pygame.display.quit()
    
    # Verify that the reset button was pressed 
    v = ser.readline()
    vstr = v.decode("utf-8")
    if ((len(vstr) == 9)):
        print("RESET")

    # Second screen
    pygame.init()
    myfont = pygame.font.SysFont('sitkasmallsitkatextsitkasubheadingsitkaheadingsitkadisplaysitkabanner', 30)

    WINDOW_SIZE = [500, 500]
    screen = pygame.display.set_mode(WINDOW_SIZE)  # screen is a pygame Surface object
    pygame.display.set_caption("Accelerometer Maze Game")
    clock = pygame.time.Clock()
    screen.fill(WHITE)
    label1 = myfont.render('Tilt UP for Normal Play', True, BLACK)
    label2 = myfont.render('Tilt DOWN for Challenge Play', True, BLACK)

    # Start values for grid and finalx/finaly
    CT = 0
    GCT = 1

    # Loop until the user clicks the close button.
    while (initialize == False):
        for event in pygame.event.get():  # User did something
            if event.type == pygame.QUIT:  # If user clicked close
                initialize = True  # Flag that we are done so we exit this loop
                done = True
                SIZE = 0

        v = ser.readline()
        vstr = v.decode("utf-8")
        if ((len(vstr) < 5)):
            initialize = True

        pygame.draw.rect(screen,
                         BLUE,
                         [0, 0, 500, 250])
        pygame.draw.rect(screen,
                         GREEN,
                         [0, 250, 500, 250])
        screen.blit(label1, (70, 115))
        screen.blit(label2, (30, 365))

        clock.tick(60)
        pygame.display.flip()

    if ((len(vstr) < 5) & (CT == 0)):
        SIZE = int(vstr[0:2])
        CT = CT + 1
        print("size")
        print(SIZE)

    # BOX is width and height of one box
    if (SIZE >= 15):
        BOX = 30
    else:
        BOX = 50

    # START will be true when the first maze comes in where ball is at (0,0)
    START = True

    GRID = []

    pygame.display.quit()
    
    # Maze screen
    # Set the height and width of the screen, based on number of boxes
    WINDOW_S = ((SIZE + 1) * MARGIN) + (SIZE * BOX)
    WINDOW_SIZE = [WINDOW_S, WINDOW_S]
    screen = pygame.display.set_mode(WINDOW_SIZE)  # screen is a pygame Surface object

    # Set title of screen
    pygame.display.set_caption("Accelerometer Maze Game")

    # Used to manage how fast the screen updates
    clock = pygame.time.Clock()
    # Set the screen background
    screen.fill(WHITE)
    # -------- Main Program Loop -----------
    while not done:
        for event in pygame.event.get():  # User did something
            if event.type == pygame.QUIT:  # If user clicked close
                done = True  # Flag that we are done so we exit this loop

        v = ser.readline()
        vstr = v.decode("utf-8")

        if ((len(vstr) < 5) & (CT == 0)):
            SIZE = int(vstr[0:2])
            CT = CT + 1
            if (SIZE >= 15):
                BOX = 30
            else:
                BOX = 50
            print("size")
            print(SIZE)
        elif ((len(vstr) < 5) & (CT == 1)):
            FINALY = int(vstr[0:2])
            CT = CT + 1
            print("finalx")
            print(FINALY)
        elif ((len(vstr) < 5) & (CT == 2)):
            FINALX = int(vstr[0:2])
            print("finaly")
            print(FINALX)
            START = False
        elif (len(vstr) == 7):
            CT = 0
        elif ((GCT == SIZE) & (START == False)):
            GRID.append((list(map(int, vstr.split()))))
            draw_grid(GRID, SIZE, screen, BOX, FINALX, FINALY)
            GCT = 1
            GRID = []
        else:
            GRID.append((list(map(int, vstr.split()))))
            GCT = GCT + 1

        # Limit to 60 frames per second
        clock.tick(60)

        # Go ahead and update the screen with what we've drawn.
        pygame.display.flip()

    # Be IDLE friendly. If you forget this line, the program will 'hang'
    # on exit.
    pygame.quit()


if __name__ == '__main__':
    main()

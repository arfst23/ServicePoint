# Service Point Display at CCCB

The access to the Service Point display is done via https://github.com/cccb/servicepoint

Run `build` to create or update this library and header.

Run `make` to build the aps in this repo.

## Service Point Display Tools

  - `spres`: reset the service point display
  - `spclr`: clear the service point display
  - `tty2sp`: user the service point display as dumb 56x20 terminal
  - `pbm2sp`: display a portable bitmap on the service point display
  - `jpg2sp`: display a jpeg dithered on the service point display
  - `pbms2sp`: display a series of portable bitmaps looped on the service point display

### Options

  - `-r`: reverse
  - `-x`: use service point display emulator
  - `-s`: send to service point display
  - `-w`: wait for button press to terminate
  - `-23`: time in seconds to run

### Files

  - `sp.h`, `libsp.a`: derived from https://github.com/cccb/servicepoint, checked in for convenience
  - `servicepoint.h`, `servicepoint.c`: abstraction level to the service point library
  - `graphix.h`, `graphicx.c`: simple X11 window api
  - `display_graphix.h`, `display_graphix.c`: service point display emulation
  - `display.h`, `display.c`: common api for service point display library and emulator
  - `display_text.h`, `display_text.c`: client side text rendering on display

### Examples

    make pbm
    pbm2sp -x -s < frame-01.pbm
    pbms2sp -r -x -s frame-??.pbm

    jpg2sp foo.jpg

    mktty | tty2sp -x -s

## BSD Game Robots

This is a 56x20 charaters BSD games robots clone.

### Files

  - `robots.h`, `robots.c`: robots implementation
  - `robots_auto.h`, `robots_auto.c`: solver from arfst23
  - `robots_bsd.h`, `robots_bsd.c`: bsd solver
  - `strategy`: python template to solve robots via socket (option `-p`)

### Options

  - `-l`: log output
  - `-t`: simple color terminal output
  - `-x`: use service point display emulator
  - `-s`: send to service point display
  - `-d`: delay after every move (for non interative viewing
  - `-p`: open port to stear
  - `-a`: solver by arfst23
  - `-b`: bsd solver
  - `-g`: guided mode (in both solves yield the same key)
  - `-23`: play number of roounds

### Examples

    robots -t -x -a -d
    robots -l -b

## Bubbles

Displays a pattern of expanding bubbles

### Options

  - `-x`: use service point display emulator
  - `-s`: send to service point display
  - `-r`: dirplay rings instead of circles
  - `-23`: time in seconds to run

### Examples

    bubbles -x -s

## Disks

Displays disks rolling in the rows of the display.  Disks can have 2 different
velocities and colors.  Disks reflect at the left/right border and when hitting
a disk of the same color.  Running into a disk of mismatching color makes both
disks drop to new rows depending on their velocity.

### Options

  - `-x`: use service point display emulator
  - `-s`: send to service point display
  - `-23`: time in seconds to run

### Examples

    disks -x -s

## Tunnel

Display a fly through a tunnel animation.

### Options

  - `-x`: use service point display emulator
  - `-s`: send to service point display
  - `-23`: time in seconds to run

### Examples

    tunnel -x -s

## Eyes

An animation of 2 eyes, blinking and looking into different directions

The default storyline is:
  - look straight for some time
  - look around, discover something iteresting on the right
  - think about it, thing even harder
  - feel dizzy from thinking, fall asleep

### Options

  - `-x`: use service point display emulator
  - `-s`: send to service point display
  - `-p...`: provide a playbook as string; chars mean:
    - `.` / `_`: wait short / long
    - `x` / `X`: move eyes straight slow / fast
    - `l` / `l`: move eyes left down slow / fast
    - `r` / `R`: move eyes right down slow / fast
    - `t` / `T`: move eyes up thinking slow / fast
    - `h` / `H`: move eyes up thinking hard slow / fast
    - `d` / `D`: move eyes dizzy slow / fast
    - `s`: eyes fall asleep very slow 
  - `-23`: number of times the playbook is played

### Examples

    eyes -s -x -p_ # look straight only

## Airport

Emulator for a [Split-flap display](https://en.wikipedia.org/wiki/Split-flap_display)
like the ones that used to be in airports.

Reads from standard in, a blank line starts the display to update.

### Options

  - `-x`: use service point display emulator
  - `-s`: send to service point display
  - `-23`: time in seconds to run

### Examples

    friedrichstrasse | airport -x -s

## Roll

Read from stdin up to an empty line, display 56x20 charackers, repeat.
Charakters are updated with an roling effect

### Options

  - `-x`: use service point display emulator
  - `-s`: send to service point display
  - `-23`: time in seconds to run

### Examples

    friedrichstrasse | roll -x -s

## Demo

Show off all.

    demo

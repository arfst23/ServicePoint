# Service Point Display at CCCB

The access to the Service Point display is done via
`https://github.com/cccb/servicepoint`.

## Service Point Display Tools

  - `spres`: reset the service point display
  - `spclr`: clear the service point display
  - `tty2sp`: user the service point display as dumb 56x20 terminal
  - `pbm2sp`: display a portable bitmap on the service point display
  - `jpg2sp`: display a jpeg dithered on the service point display
  - `pbms2sp`: display a series of portable bitmaps looped on the service point display

### Files

  - `sp.h`, `libsp.a`: derived from https://github.com/cccb/servicepoint,
    checked in for convenience
  - `servicepoint.h`, `servicepoint.c`: abstraction level to the service point library
  - `graphix.h`, `graphicx.c`: simple X11 window api
  - `display\_graphix.h`, `display\_graphix.c`: service point display emulation
  - `display.h`, `display.c`: common api for service point display library and emulator
  - `display\_text.h`, `display\_text.c': client side text rendering on display

### Optionsre

  - `-r`: reverse
  - `-x`: use service point display emulator
  - `-s`: send to service point display
  - `-23`: frame delay for pbms2sp

### Examples

    make pbm
    pbm2sp -x -s < frame-01.pbm
    pbms2sp -r -x -s frame-??.pbm

    jpg2sp foo.jpg

    mktty | tty2sp g

## BSD Game Robots

This is a 56x20 charaters BSD games robots clone.

### Files

  - `robots.h`, `robots.c`: robots implementation
  - `robots\_auto.h`, `robots\_auto.c`: solver from arfst23
  - `robots\_bsd.h`, `robots\_bsd.c`: bsd solver
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

### Examples

    robots -t -x -a -d
    robots -l -b

## Boop

Implements the board game Boop

TODO

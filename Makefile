##############################################################################

CC		= gcc
CPPFLAGS	=
CFLAGS		= -march=native -O3 -funroll-loops -fpic -fstack-protector-all \
		  -Wall -Wextra -Werror -Wno-deprecated-declarations #-Og -g
#LDFLAGS		= -L. -lsp -llzma -lX11 -lm
LDFLAGS		= -L. -lX11 -lm
REASON		= @if [ -f $@ ]; then echo "[$@: $?]"; else echo "[$@]"; fi

.PHONY: tags depend clean distclean
.SUFFIXES: .png .pbm

.c.o:
	$(REASON)
	$(COMPILE.c) $< $(OUTPUT_OPTION)

.png.pbm:
	$(REASON)
	pngtopnm $< | ppmtopgm| pgmtopbm > $@

################################################################################

#all: spres spclr tty2sp pbm2sp pbms2sp robots bubbles disks tunnel eyes airport watch nichts
all: tty2sp pbm2sp pbms2sp robots bubbles disks tunnel eyes airport watch nichts

pbm: frame-01.pbm frame-02.pbm frame-03.pbm frame-04.pbm frame-05.pbm \
	frame-06.pbm frame-07.pbm frame-08.pbm frame-09.pbm frame-10.pbm \
	frame-11.pbm frame-12.pbm frame-13.pbm frame-14.pbm frame-15.pbm \
	frame-16.pbm frame-17.pbm frame-18.pbm frame-19.pbm

spres: spres.o
	$(REASON)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

spclr: spclr.o
	$(REASON)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

tty2sp: tty2sp.o display_text.o display.o display_graphix.o graphix.o sp.o
	$(REASON)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

pbm2sp: pbm2sp.o display.o display_graphix.o graphix.o sp.o
	$(REASON)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) -lnetpbm

pbms2sp: pbms2sp.o display.o display_graphix.o graphix.o sp.o
	$(REASON)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) -lnetpbm

robots: robots.o robots_auto.o robots_bsd.o term.o port.o display_text.o display.o \
	display_graphix.o graphix.o sp.o
	$(REASON)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

bubbles: bubbles.o display.o display_graphix.o graphix.o sp.o
	$(REASON)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

disks: disks.o display.o display_graphix.o graphix.o sp.o
	$(REASON)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

tunnel: tunnel.o display.o display_graphix.o graphix.o sp.o
	$(REASON)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

eyes: eyes.o display.o display_graphix.o graphix.o sp.o
	$(REASON)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

airport: airport.o display.o display_graphix.o graphix.o sp.o
	$(REASON)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

watch: watch.o  display_text.o display.o display_graphix.o graphix.o sp.o
	$(REASON)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

nichts: nichts.o display.o display_graphix.o graphix.o sp.o
	$(REASON)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

################################################################################

graphix_test: graphix_test.o graphix.o
	$(REASON)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

sp_test: sp_test.o
	$(REASON)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

display_graphix_test: display_graphix_test.o display_graphix.o graphix.o
	$(REASON)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

display_test: display_test.o display.o display_graphix.o graphix.o servicepoint.o
	$(REASON)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

display_text_test: display_text_test.c display_text.o display.o display_graphix.o graphix.o servicepoint.o
	$(REASON)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

################################################################################

test: test_offline test_online

test_offline: graphix_test display_graphix_test display_test
	$(REASON)
	graphix_test
	display_graphix_test
	display_test gx

test_online: spclr sp_test servicepoint_test display_test
	$(REASON)
	spclr
	sp_test
	spclr
	servicepoint_test
	spclr
	display_test sp
	spclr
	display_test

################################################################################

tags:
	$(REASON)
	ctags --format=2 -o $@ *.h *.c

deps depend: *.h *.c
	$(REASON)
	$(CC) -MM $(CPPFLAGS) *.c > deps

clean:
	$(REASON)
	$(RM) *.o *~ *_test *.pbm

distclean: clean
	$(REASON)
	git clean -xdf

-include deps

################################################################################

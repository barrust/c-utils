TESTDIR=tests
DISTDIR=dist
SRCDIR=src

CCFLAGS=-Wall -Wpedantic -Winline -O3

all: string timing
	gcc $(DISTDIR)/stringlib.o $(TESTDIR)/stringlib_test.c $(CCFLAGS) -o ./$(DISTDIR)/strlib

string:
	gcc -c $(SRCDIR)/stringlib.c -o $(DISTDIR)/stringlib.o $(CCFLAGS)

timing:
	gcc $(TESTDIR)/timing_test.c $(CCFLAGS) -o ./$(DISTDIR)/timing

clean:
	rm -rf ./$(DISTDIR)/*

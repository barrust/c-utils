TESTDIR=tests
DISTDIR=dist
SRCDIR=src

CCFLAGS=-Wall -Wpedantic -Winline -O3

all: string
	gcc $(DISTDIR)/stringlib.o $(TESTDIR)/stringlib_test.c $(CCFLAGS) -o ./$(DISTDIR)/strlib
	gcc $(TESTDIR)/timing_test.c $(CCFLAGS) -o ./$(DISTDIR)/timing

string:
	gcc -c $(SRCDIR)/stringlib.c -o $(DISTDIR)/stringlib.o $(CCFLAGS)

clean:
	rm -rf ./$(DISTDIR)/*

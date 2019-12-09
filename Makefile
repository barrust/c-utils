TESTDIR=tests
DISTDIR=dist
SRCDIR=src

CC=gcc
CCFLAGS=-lm -Wall -Wpedantic -Winline -O3

all: string bitarray fileutils
	$(CC) $(DISTDIR)/stringlib.o $(TESTDIR)/stringlib_test.c $(CCFLAGS) -o ./$(DISTDIR)/strlib
	$(CC) $(TESTDIR)/timing_test.c $(CCFLAGS) -o ./$(DISTDIR)/timing
	$(CC) $(DISTDIR)/bitarray-lib.o $(TESTDIR)/bitarray_test.c $(CCFLAGS) -o ./$(DISTDIR)/bitarray
	$(CC) $(DISTDIR)/fileutils-lib.o $(TESTDIR)/fileutils_test.c $(CCFLAGS) -o ./$(DISTDIR)/fileutils

string:
	$(CC) -c $(SRCDIR)/stringlib.c -o $(DISTDIR)/stringlib.o $(CCFLAGS)

bitarray:
	$(CC) -c $(SRCDIR)/bitarray.c -o $(DISTDIR)/bitarray-lib.o $(CCFLAGS)

fileutils:
	$(CC) -c $(SRCDIR)/fileutils.c -o $(DISTDIR)/fileutils-lib.o $(CCFLAGS)

debug: CCFLAGS += -g
debug: all

clean:
	rm -rf ./$(DISTDIR)/*

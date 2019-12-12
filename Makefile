TESTDIR=tests
DISTDIR=dist
SRCDIR=src
CCFLAGS=-lm -Wall -Wpedantic -Winline


all: string bitarray fileutils
	$(CC) $(STD) $(DISTDIR)/stringlib.o $(TESTDIR)/stringlib_test.c $(CCFLAGS) -o ./$(DISTDIR)/strlib
	$(CC) $(STD) $(TESTDIR)/timing_test.c $(CCFLAGS)  $(STD) -o ./$(DISTDIR)/timing
	$(CC) $(STD) $(DISTDIR)/bitarray-lib.o $(TESTDIR)/bitarray_test.c $(CCFLAGS) -o ./$(DISTDIR)/bitarray
	$(CC) $(STD) $(DISTDIR)/fileutils-lib.o $(TESTDIR)/fileutils_test.c $(CCFLAGS) -o ./$(DISTDIR)/fileutils

string:
	$(CC) $(STD) -c $(SRCDIR)/stringlib.c -o $(DISTDIR)/stringlib.o $(CCFLAGS)

bitarray:
	$(CC) $(STD) -c $(SRCDIR)/bitarray.c -o $(DISTDIR)/bitarray-lib.o $(CCFLAGS)

fileutils:
	$(CC) $(STD) -c $(SRCDIR)/fileutils.c -o $(DISTDIR)/fileutils-lib.o $(CCFLAGS)

debug: CCFLAGS += -g
debug: all

test: CCFLAGS += -coverage
test: all

clean:
	rm -rf ./$(DISTDIR)/*
	rm -rf ./*.gcno
	rm -rf ./*.gcda
	rm -rf ./*.gcov

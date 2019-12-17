TESTDIR=tests
DISTDIR=dist
SRCDIR=src
EXAMPLEDIR=examples
COMPFLAGS=-Wall -Wpedantic -Winline


all: libraries
	$(CC) $(STD) $(DISTDIR)/stringlib.o $(TESTDIR)/stringlib_test.c $(CCFLAGS) $(COMPFLAGS) -o ./$(DISTDIR)/strlib
	$(CC) $(STD) $(TESTDIR)/timing_test.c $(CCFLAGS) $(COMPFLAGS) -o ./$(DISTDIR)/timing
	$(CC) $(STD) $(DISTDIR)/bitarray-lib.o $(TESTDIR)/bitarray_test.c $(CCFLAGS) $(COMPFLAGS) -o ./$(DISTDIR)/bitarray
	$(CC) $(STD) $(DISTDIR)/fileutils-lib.o $(TESTDIR)/fileutils_test.c $(CCFLAGS) $(COMPFLAGS) -o ./$(DISTDIR)/fileutils

libraries: string bitarray fileutils

string:
	$(CC) $(STD) -c $(SRCDIR)/stringlib.c -o $(DISTDIR)/stringlib.o $(CCFLAGS) $(COMPFLAGS)

bitarray:
	$(CC) $(STD) -c $(SRCDIR)/bitarray.c -o $(DISTDIR)/bitarray-lib.o $(CCFLAGS) $(COMPFLAGS)

fileutils:
	$(CC) $(STD) -c $(SRCDIR)/fileutils.c -o $(DISTDIR)/fileutils-lib.o $(CCFLAGS) $(COMPFLAGS)

debug: CCFLAGS += -g
debug: all

test: CCFLAGS += -coverage
test: all

examples: libraries
	$(CC) $(STD) $(EXAMPLEDIR)/timing_example.c $(CCFLAGS) $(COMPFLAGS) -lm -o ./$(DISTDIR)/ex_timing
	#$(CC) $(STD) $(DISTDIR)/bitarray-lib.o $(EXAMPLEDIR)/bitarray_example.c $(CCFLAGS) $(COMPFLAGS) -o ./$(DISTDIR)/ex_bitarray
	#$(CC) $(STD) $(DISTDIR)/fileutils-lib.o $(EXAMPLEDIR)/fileutils_example.c $(CCFLAGS) $(COMPFLAGS) -o ./$(DISTDIR)/ex_fileutils

clean:
	rm -rf ./$(DISTDIR)/*
	rm -f ./*.gcno
	rm -f ./*.gcda
	rm -f ./*.gcov

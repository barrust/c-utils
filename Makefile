TESTDIR=tests
LIBDIR=libs
DISTDIR=dist
SRCDIR=src
EXAMPLEDIR=examples
COMPFLAGS=-Wall -Wpedantic -Winline -Wextra -Wno-unknown-pragmas -Wno-long-long


all: libraries examples test

libraries: string bitarray fileutils linkedlist doublylinkedlist graph queue stack permutations

string:
	$(CC) $(STD) -c $(SRCDIR)/stringlib.c -o $(LIBDIR)/string-lib.o $(CCFLAGS) $(COMPFLAGS)

bitarray:
	$(CC) $(STD) -c $(SRCDIR)/bitarray.c -o $(LIBDIR)/bitarray-lib.o $(CCFLAGS) $(COMPFLAGS)

fileutils:
	$(CC) $(STD) -c $(SRCDIR)/fileutils.c -o $(LIBDIR)/fileutils-lib.o $(CCFLAGS) $(COMPFLAGS)

linkedlist:
	$(CC) $(STD) -c $(SRCDIR)/llist.c -o $(LIBDIR)/llist-lib.o $(CCFLAGS) $(COMPFLAGS)

doublylinkedlist:
	$(CC) $(STD) -c $(SRCDIR)/dllist.c -o $(LIBDIR)/dllist-lib.o $(CCFLAGS) $(COMPFLAGS)

stack:
	$(CC) $(STD) -c $(SRCDIR)/stack.c -o $(LIBDIR)/stack-lib.o $(CCFLAGS) $(COMPFLAGS)

queue:
	$(CC) $(STD) -c $(SRCDIR)/queue.c -o $(LIBDIR)/queue-lib.o $(CCFLAGS) $(COMPFLAGS)

graph:
	$(CC) $(STD) -c $(SRCDIR)/graph.c -o $(LIBDIR)/graph-lib.o $(CCFLAGS) $(COMPFLAGS)

permutations:
	$(CC) $(STD) -c $(SRCDIR)/permutations.c -o $(LIBDIR)/permutations-lib.o $(CCFLAGS) $(COMPFLAGS)

debug: CCFLAGS += -g
debug: all

openmp: CCFLAGS += -fopenmp
openmp: all

test: CCFLAGS += -coverage
test: libraries
	$(CC) $(STD) $(LIBDIR)/string-lib.o $(TESTDIR)/stringlib_test.c $(CCFLAGS) $(COMPFLAGS) -o ./$(DISTDIR)/strlib
	$(CC) $(STD) $(TESTDIR)/timing_test.c $(CCFLAGS) $(COMPFLAGS) -o ./$(DISTDIR)/timing
	$(CC) $(STD) $(TESTDIR)/minunit_test.c $(CCFLAGS) $(COMPFLAGS) -lm -o ./$(DISTDIR)/minunit
	$(CC) $(STD) $(LIBDIR)/bitarray-lib.o $(TESTDIR)/bitarray_test.c $(CCFLAGS) $(COMPFLAGS) -o ./$(DISTDIR)/bitarray
	$(CC) $(STD) $(LIBDIR)/fileutils-lib.o $(TESTDIR)/fileutils_test.c $(CCFLAGS) $(COMPFLAGS) -o ./$(DISTDIR)/fileutils
	$(CC) $(STD) $(LIBDIR)/llist-lib.o $(TESTDIR)/linked_list_test.c $(CCFLAGS) $(COMPFLAGS) -o ./$(DISTDIR)/linkedlist
	$(CC) $(STD) $(LIBDIR)/dllist-lib.o $(TESTDIR)/doubly_linked_list_test.c $(CCFLAGS) $(COMPFLAGS) -o ./$(DISTDIR)/doublelinkedlist
	$(CC) $(STD) $(LIBDIR)/queue-lib.o $(TESTDIR)/queue_test.c $(CCFLAGS) $(COMPFLAGS) -o ./$(DISTDIR)/queue
	$(CC) $(STD) $(LIBDIR)/stack-lib.o $(TESTDIR)/stack_test.c $(CCFLAGS) $(COMPFLAGS) -o ./$(DISTDIR)/stack
	$(CC) $(STD) $(LIBDIR)/graph-lib.o $(TESTDIR)/graph_test.c $(CCFLAGS) $(COMPFLAGS) -o ./$(DISTDIR)/graph
	$(CC) $(STD) $(LIBDIR)/permutations-lib.o $(TESTDIR)/permutations_test.c $(CCFLAGS) $(COMPFLAGS) -o ./$(DISTDIR)/permutations

examples: libraries
	$(CC) $(STD) $(EXAMPLEDIR)/timing_example.c $(CCFLAGS) $(COMPFLAGS) -lm -o ./$(DISTDIR)/ex_timing
	$(CC) $(STD) $(LIBDIR)/bitarray-lib.o $(EXAMPLEDIR)/bitarray_example.c $(CCFLAGS) $(COMPFLAGS) -o ./$(DISTDIR)/ex_bitarray
	$(CC) $(STD) $(LIBDIR)/fileutils-lib.o $(EXAMPLEDIR)/fileutils_example.c $(CCFLAGS) $(COMPFLAGS) -o ./$(DISTDIR)/ex_fileutils
	$(CC) $(STD) $(LIBDIR)/string-lib.o $(EXAMPLEDIR)/stringlib_example.c $(CCFLAGS) $(COMPFLAGS) -o ./$(DISTDIR)/ex_stringlib
	$(CC) $(STD) $(LIBDIR)/llist-lib.o $(EXAMPLEDIR)/linkedlist_example.c $(CCFLAGS) $(COMPFLAGS) -o ./$(DISTDIR)/ex_linkedlist
	$(CC) $(STD) $(LIBDIR)/dllist-lib.o $(EXAMPLEDIR)/doublylinkedlist_example.c $(CCFLAGS) $(COMPFLAGS) -o ./$(DISTDIR)/ex_doublylinkedlist
	$(CC) $(STD) $(LIBDIR)/queue-lib.o $(EXAMPLEDIR)/queue_example.c $(CCFLAGS) $(COMPFLAGS) -o ./$(DISTDIR)/ex_queue
	$(CC) $(STD) $(LIBDIR)/stack-lib.o $(EXAMPLEDIR)/stack_example.c $(CCFLAGS) $(COMPFLAGS) -o ./$(DISTDIR)/ex_stack
	$(CC) $(STD) $(LIBDIR)/graph-lib.o $(EXAMPLEDIR)/graph_example.c $(CCFLAGS) $(COMPFLAGS) -o ./$(DISTDIR)/ex_graph
	$(CC) $(STD) $(LIBDIR)/permutations-lib.o $(EXAMPLEDIR)/permutations_example.c $(CCFLAGS) $(COMPFLAGS) -o ./$(DISTDIR)/ex_permutations

clean:
	if [ -d "./$(DISTDIR)/" ]; then rm ./$(DISTDIR)/*; fi
	if [ -d "./$(LIBDIR)/" ]; then rm ./$(LIBDIR)/*; fi
	rm -f ./*.gcno
	rm -f ./*.gcda
	rm -f ./*.gcov

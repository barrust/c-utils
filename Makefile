TESTDIR=tests
DISTDIR=dist
SRCDIR=src
EXAMPLEDIR=examples
COMPFLAGS=-Wall -Wpedantic -Winline -Wextra -Wno-unknown-pragmas


all: libraries examples test

libraries: string bitarray fileutils linkedlist doublylinkedlist graph queue stack

string:
	$(CC) $(STD) -c $(SRCDIR)/stringlib.c -o $(DISTDIR)/stringlib.o $(CCFLAGS) $(COMPFLAGS)

bitarray:
	$(CC) $(STD) -c $(SRCDIR)/bitarray.c -o $(DISTDIR)/bitarray-lib.o $(CCFLAGS) $(COMPFLAGS)

fileutils:
	$(CC) $(STD) -c $(SRCDIR)/fileutils.c -o $(DISTDIR)/fileutils-lib.o $(CCFLAGS) $(COMPFLAGS)

linkedlist:
	$(CC) $(STD) -c $(SRCDIR)/llist.c -o $(DISTDIR)/llist-lib.o $(CCFLAGS) $(COMPFLAGS)

doublylinkedlist:
	$(CC) $(STD) -c $(SRCDIR)/dllist.c -o $(DISTDIR)/dllist-lib.o $(CCFLAGS) $(COMPFLAGS)

stack:
	$(CC) $(STD) -c $(SRCDIR)/stack.c -o $(DISTDIR)/stack-lib.o $(CCFLAGS) $(COMPFLAGS)

queue:
	$(CC) $(STD) -c $(SRCDIR)/queue.c -o $(DISTDIR)/queue-lib.o $(CCFLAGS) $(COMPFLAGS)

graph:
	$(CC) $(STD) -c $(SRCDIR)/graph.c -o $(DISTDIR)/graph-lib.o $(CCFLAGS) $(COMPFLAGS)

debug: CCFLAGS += -g
debug: all

openmp: CCFLAGS += -fopenmp
openmp: all

test: CCFLAGS += -coverage
test: libraries
	$(CC) $(STD) $(DISTDIR)/stringlib.o $(TESTDIR)/stringlib_test.c $(CCFLAGS) $(COMPFLAGS) -o ./$(DISTDIR)/strlib
	$(CC) $(STD) $(TESTDIR)/timing_test.c $(CCFLAGS) $(COMPFLAGS) -o ./$(DISTDIR)/timing
	$(CC) $(STD) $(TESTDIR)/minunit_test.c $(CCFLAGS) $(COMPFLAGS) -o ./$(DISTDIR)/minunit
	$(CC) $(STD) $(DISTDIR)/bitarray-lib.o $(TESTDIR)/bitarray_test.c $(CCFLAGS) $(COMPFLAGS) -o ./$(DISTDIR)/bitarray
	$(CC) $(STD) $(DISTDIR)/fileutils-lib.o $(TESTDIR)/fileutils_test.c $(CCFLAGS) $(COMPFLAGS) -o ./$(DISTDIR)/fileutils
	$(CC) $(STD) $(DISTDIR)/llist-lib.o $(TESTDIR)/linked_list_test.c $(CCFLAGS) $(COMPFLAGS) -o ./$(DISTDIR)/linkedlist
	$(CC) $(STD) $(DISTDIR)/dllist-lib.o $(TESTDIR)/doubly_linked_list_test.c $(CCFLAGS) $(COMPFLAGS) -o ./$(DISTDIR)/doublelinkedlist
	$(CC) $(STD) $(DISTDIR)/queue-lib.o $(TESTDIR)/queue_test.c $(CCFLAGS) $(COMPFLAGS) -o ./$(DISTDIR)/queue
	$(CC) $(STD) $(DISTDIR)/stack-lib.o $(TESTDIR)/stack_test.c $(CCFLAGS) $(COMPFLAGS) -o ./$(DISTDIR)/stack
	$(CC) $(STD) $(DISTDIR)/graph-lib.o $(TESTDIR)/graph_test.c $(CCFLAGS) $(COMPFLAGS) -o ./$(DISTDIR)/graph

examples: libraries
	$(CC) $(STD) $(EXAMPLEDIR)/timing_example.c $(CCFLAGS) $(COMPFLAGS) -lm -o ./$(DISTDIR)/ex_timing
	$(CC) $(STD) $(DISTDIR)/bitarray-lib.o $(EXAMPLEDIR)/bitarray_example.c $(CCFLAGS) $(COMPFLAGS) -o ./$(DISTDIR)/ex_bitarray
	$(CC) $(STD) $(DISTDIR)/fileutils-lib.o $(EXAMPLEDIR)/fileutils_example.c $(CCFLAGS) $(COMPFLAGS) -o ./$(DISTDIR)/ex_fileutils
	$(CC) $(STD) $(DISTDIR)/stringlib.o $(EXAMPLEDIR)/stringlib_example.c $(CCFLAGS) $(COMPFLAGS) -o ./$(DISTDIR)/ex_stringlib
	$(CC) $(STD) $(DISTDIR)/llist-lib.o $(EXAMPLEDIR)/linkedlist_example.c $(CCFLAGS) $(COMPFLAGS) -o ./$(DISTDIR)/ex_linkedlist
	$(CC) $(STD) $(DISTDIR)/dllist-lib.o $(EXAMPLEDIR)/doublylinkedlist_example.c $(CCFLAGS) $(COMPFLAGS) -o ./$(DISTDIR)/ex_doublylinkedlist
	$(CC) $(STD) $(DISTDIR)/queue-lib.o $(EXAMPLEDIR)/queue_example.c $(CCFLAGS) $(COMPFLAGS) -o ./$(DISTDIR)/ex_queue
	$(CC) $(STD) $(DISTDIR)/stack-lib.o $(EXAMPLEDIR)/stack_example.c $(CCFLAGS) $(COMPFLAGS) -o ./$(DISTDIR)/ex_stack
	$(CC) $(STD) $(DISTDIR)/graph-lib.o $(EXAMPLEDIR)/graph_example.c $(CCFLAGS) $(COMPFLAGS) -o ./$(DISTDIR)/ex_graph

clean:
	rm -rf ./$(DISTDIR)/*
	rm -f ./*.gcno
	rm -f ./*.gcda
	rm -f ./*.gcov

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
	$(CC) $(STD) $(LIBDIR)/string-lib.o $(TESTDIR)/stringlib_test.c $(CCFLAGS) $(COMPFLAGS) -o $(CURDIR)/$(DISTDIR)/strlib
	$(CC) $(STD) $(TESTDIR)/timing_test.c $(CCFLAGS) $(COMPFLAGS) -o $(CURDIR)/$(DISTDIR)/timing
	$(CC) $(STD) $(TESTDIR)/minunit_test.c $(CCFLAGS) $(COMPFLAGS) -lm -o $(CURDIR)/$(DISTDIR)/minunit
	$(CC) $(STD) $(LIBDIR)/bitarray-lib.o $(TESTDIR)/bitarray_test.c $(CCFLAGS) $(COMPFLAGS) -o $(CURDIR)/$(DISTDIR)/bitarray
	$(CC) $(STD) $(LIBDIR)/fileutils-lib.o $(TESTDIR)/fileutils_test.c $(CCFLAGS) $(COMPFLAGS) -o $(CURDIR)/$(DISTDIR)/fileutils
	$(CC) $(STD) $(LIBDIR)/llist-lib.o $(TESTDIR)/linked_list_test.c $(CCFLAGS) $(COMPFLAGS) -o $(CURDIR)/$(DISTDIR)/linkedlist
	$(CC) $(STD) $(LIBDIR)/dllist-lib.o $(TESTDIR)/doubly_linked_list_test.c $(CCFLAGS) $(COMPFLAGS) -o $(CURDIR)/$(DISTDIR)/doublelinkedlist
	$(CC) $(STD) $(LIBDIR)/queue-lib.o $(TESTDIR)/queue_test.c $(CCFLAGS) $(COMPFLAGS) -o $(CURDIR)/$(DISTDIR)/queue
	$(CC) $(STD) $(LIBDIR)/stack-lib.o $(TESTDIR)/stack_test.c $(CCFLAGS) $(COMPFLAGS) -o $(CURDIR)/$(DISTDIR)/stack
	$(CC) $(STD) $(LIBDIR)/graph-lib.o $(TESTDIR)/graph_test.c $(CCFLAGS) $(COMPFLAGS) -o $(CURDIR)/$(DISTDIR)/graph
	$(CC) $(STD) $(LIBDIR)/permutations-lib.o $(TESTDIR)/permutations_test.c $(CCFLAGS) $(COMPFLAGS) -o $(CURDIR)/$(DISTDIR)/permutations

examples: libraries
	$(CC) $(STD) $(EXAMPLEDIR)/timing_example.c $(CCFLAGS) $(COMPFLAGS) -lm -o $(CURDIR)/$(DISTDIR)/ex_timing
	$(CC) $(STD) $(LIBDIR)/bitarray-lib.o $(EXAMPLEDIR)/bitarray_example.c $(CCFLAGS) $(COMPFLAGS) -o $(CURDIR)/$(DISTDIR)/ex_bitarray
	$(CC) $(STD) $(LIBDIR)/fileutils-lib.o $(EXAMPLEDIR)/fileutils_example.c $(CCFLAGS) $(COMPFLAGS) -o $(CURDIR)/$(DISTDIR)/ex_fileutils
	$(CC) $(STD) $(LIBDIR)/string-lib.o $(EXAMPLEDIR)/stringlib_example.c $(CCFLAGS) $(COMPFLAGS) -o $(CURDIR)/$(DISTDIR)/ex_stringlib
	$(CC) $(STD) $(LIBDIR)/llist-lib.o $(EXAMPLEDIR)/linkedlist_example.c $(CCFLAGS) $(COMPFLAGS) -o $(CURDIR)/$(DISTDIR)/ex_linkedlist
	$(CC) $(STD) $(LIBDIR)/dllist-lib.o $(EXAMPLEDIR)/doublylinkedlist_example.c $(CCFLAGS) $(COMPFLAGS) -o $(CURDIR)/$(DISTDIR)/ex_doublylinkedlist
	$(CC) $(STD) $(LIBDIR)/queue-lib.o $(EXAMPLEDIR)/queue_example.c $(CCFLAGS) $(COMPFLAGS) -o $(CURDIR)/$(DISTDIR)/ex_queue
	$(CC) $(STD) $(LIBDIR)/stack-lib.o $(EXAMPLEDIR)/stack_example.c $(CCFLAGS) $(COMPFLAGS) -o $(CURDIR)/$(DISTDIR)/ex_stack
	$(CC) $(STD) $(LIBDIR)/graph-lib.o $(EXAMPLEDIR)/graph_example.c $(CCFLAGS) $(COMPFLAGS) -o $(CURDIR)/$(DISTDIR)/ex_graph
	$(CC) $(STD) $(LIBDIR)/permutations-lib.o $(EXAMPLEDIR)/permutations_example.c $(CCFLAGS) $(COMPFLAGS) -o $(CURDIR)/$(DISTDIR)/ex_permutations

runtests:
	@ if [ -f "$(CURDIR)/$(DISTDIR)/fileutils" ]; then $(CURDIR)/$(DISTDIR)/fileutils; fi
	@ if [ -f "$(CURDIR)/$(DISTDIR)/bitarray" ]; then $(CURDIR)/$(DISTDIR)/bitarray; fi
	@ if [ -f "$(CURDIR)/$(DISTDIR)/strlib" ]; then $(CURDIR)/$(DISTDIR)/strlib; fi
	@ if [ -f "$(CURDIR)/$(DISTDIR)/timing" ]; then $(CURDIR)/$(DISTDIR)/timing; fi
	@ if [ -f "$(CURDIR)/$(DISTDIR)/linkedlist" ]; then $(CURDIR)/$(DISTDIR)/linkedlist; fi
	@ if [ -f "$(CURDIR)/$(DISTDIR)/doublelinkedlist" ]; then $(CURDIR)/$(DISTDIR)/doublelinkedlist; fi
	@ if [ -f "$(CURDIR)/$(DISTDIR)/queue" ]; then $(CURDIR)/$(DISTDIR)/queue; fi
	@ if [ -f "$(CURDIR)/$(DISTDIR)/stack" ]; then $(CURDIR)/$(DISTDIR)/stack; fi
	@ if [ -f "$(CURDIR)/$(DISTDIR)/graph" ]; then $(CURDIR)/$(DISTDIR)/graph; fi
	@ if [ -f "$(CURDIR)/$(DISTDIR)/permutations" ]; then $(CURDIR)/$(DISTDIR)/permutations; fi

# Windows-specific targets
windows: CCFLAGS += -D_WIN32
windows: LIBEXT = .exe
windows: windows-libraries windows-test windows-examples

windows-libraries:
	$(CC) $(STD) -D_WIN32 -c $(SRCDIR)/stringlib.c -o $(LIBDIR)/string-lib.o $(CCFLAGS) $(COMPFLAGS)
	$(CC) $(STD) -D_WIN32 -c $(SRCDIR)/bitarray.c -o $(LIBDIR)/bitarray-lib.o $(CCFLAGS) $(COMPFLAGS)
	$(CC) $(STD) -D_WIN32 -c $(SRCDIR)/fileutils.c -o $(LIBDIR)/fileutils-lib.o $(CCFLAGS) $(COMPFLAGS)
	$(CC) $(STD) -D_WIN32 -c $(SRCDIR)/llist.c -o $(LIBDIR)/llist-lib.o $(CCFLAGS) $(COMPFLAGS)
	$(CC) $(STD) -D_WIN32 -c $(SRCDIR)/dllist.c -o $(LIBDIR)/dllist-lib.o $(CCFLAGS) $(COMPFLAGS)
	$(CC) $(STD) -D_WIN32 -c $(SRCDIR)/graph.c -o $(LIBDIR)/graph-lib.o $(CCFLAGS) $(COMPFLAGS)
	$(CC) $(STD) -D_WIN32 -c $(SRCDIR)/queue.c -o $(LIBDIR)/queue-lib.o $(CCFLAGS) $(COMPFLAGS)
	$(CC) $(STD) -D_WIN32 -c $(SRCDIR)/stack.c -o $(LIBDIR)/stack-lib.o $(CCFLAGS) $(COMPFLAGS)
	$(CC) $(STD) -D_WIN32 -c $(SRCDIR)/permutations.c -o $(LIBDIR)/permutations-lib.o $(CCFLAGS) $(COMPFLAGS)

windows-test: windows-libraries
	$(CC) $(STD) -D_WIN32 $(LIBDIR)/string-lib.o $(TESTDIR)/stringlib_test.c $(CCFLAGS) $(COMPFLAGS) -o $(CURDIR)/$(DISTDIR)/strlib.exe
	$(CC) $(STD) -D_WIN32 $(TESTDIR)/timing_test.c $(CCFLAGS) $(COMPFLAGS) -o $(CURDIR)/$(DISTDIR)/timing.exe
	$(CC) $(STD) -D_WIN32 $(TESTDIR)/minunit_test.c $(CCFLAGS) $(COMPFLAGS) -lm -o $(CURDIR)/$(DISTDIR)/minunit.exe
	$(CC) $(STD) -D_WIN32 $(LIBDIR)/bitarray-lib.o $(TESTDIR)/bitarray_test.c $(CCFLAGS) $(COMPFLAGS) -o $(CURDIR)/$(DISTDIR)/bitarray.exe
	$(CC) $(STD) -D_WIN32 $(LIBDIR)/fileutils-lib.o $(TESTDIR)/fileutils_test.c $(CCFLAGS) $(COMPFLAGS) -o $(CURDIR)/$(DISTDIR)/fileutils.exe
	$(CC) $(STD) -D_WIN32 $(LIBDIR)/llist-lib.o $(TESTDIR)/linked_list_test.c $(CCFLAGS) $(COMPFLAGS) -o $(CURDIR)/$(DISTDIR)/linkedlist.exe
	$(CC) $(STD) -D_WIN32 $(LIBDIR)/dllist-lib.o $(TESTDIR)/doubly_linked_list_test.c $(CCFLAGS) $(COMPFLAGS) -o $(CURDIR)/$(DISTDIR)/doublelinkedlist.exe
	$(CC) $(STD) -D_WIN32 $(LIBDIR)/queue-lib.o $(TESTDIR)/queue_test.c $(CCFLAGS) $(COMPFLAGS) -o $(CURDIR)/$(DISTDIR)/queue.exe
	$(CC) $(STD) -D_WIN32 $(LIBDIR)/stack-lib.o $(TESTDIR)/stack_test.c $(CCFLAGS) $(COMPFLAGS) -o $(CURDIR)/$(DISTDIR)/stack.exe
	$(CC) $(STD) -D_WIN32 $(LIBDIR)/graph-lib.o $(TESTDIR)/graph_test.c $(CCFLAGS) $(COMPFLAGS) -o $(CURDIR)/$(DISTDIR)/graph.exe
	$(CC) $(STD) -D_WIN32 $(LIBDIR)/permutations-lib.o $(TESTDIR)/permutations_test.c $(CCFLAGS) $(COMPFLAGS) -o $(CURDIR)/$(DISTDIR)/permutations.exe

windows-examples: windows-libraries
	$(CC) $(STD) -D_WIN32 $(EXAMPLEDIR)/timing_example.c $(CCFLAGS) $(COMPFLAGS) -lm -o $(CURDIR)/$(DISTDIR)/ex_timing.exe
	$(CC) $(STD) -D_WIN32 $(LIBDIR)/bitarray-lib.o $(EXAMPLEDIR)/bitarray_example.c $(CCFLAGS) $(COMPFLAGS) -o $(CURDIR)/$(DISTDIR)/ex_bitarray.exe
	$(CC) $(STD) -D_WIN32 $(LIBDIR)/fileutils-lib.o $(EXAMPLEDIR)/fileutils_example.c $(CCFLAGS) $(COMPFLAGS) -o $(CURDIR)/$(DISTDIR)/ex_fileutils.exe
	$(CC) $(STD) -D_WIN32 $(LIBDIR)/string-lib.o $(EXAMPLEDIR)/stringlib_example.c $(CCFLAGS) $(COMPFLAGS) -o $(CURDIR)/$(DISTDIR)/ex_stringlib.exe
	$(CC) $(STD) -D_WIN32 $(LIBDIR)/llist-lib.o $(EXAMPLEDIR)/linkedlist_example.c $(CCFLAGS) $(COMPFLAGS) -o $(CURDIR)/$(DISTDIR)/ex_linkedlist.exe
	$(CC) $(STD) -D_WIN32 $(LIBDIR)/dllist-lib.o $(EXAMPLEDIR)/doublylinkedlist_example.c $(CCFLAGS) $(COMPFLAGS) -o $(CURDIR)/$(DISTDIR)/ex_doublylinkedlist.exe
	$(CC) $(STD) -D_WIN32 $(LIBDIR)/queue-lib.o $(EXAMPLEDIR)/queue_example.c $(CCFLAGS) $(COMPFLAGS) -o $(CURDIR)/$(DISTDIR)/ex_queue.exe
	$(CC) $(STD) -D_WIN32 $(LIBDIR)/stack-lib.o $(EXAMPLEDIR)/stack_example.c $(CCFLAGS) $(COMPFLAGS) -o $(CURDIR)/$(DISTDIR)/ex_stack.exe
	$(CC) $(STD) -D_WIN32 $(LIBDIR)/graph-lib.o $(EXAMPLEDIR)/graph_example.c $(CCFLAGS) $(COMPFLAGS) -o $(CURDIR)/$(DISTDIR)/ex_graph.exe
	$(CC) $(STD) -D_WIN32 $(LIBDIR)/permutations-lib.o $(EXAMPLEDIR)/permutations_example.c $(CCFLAGS) $(COMPFLAGS) -o $(CURDIR)/$(DISTDIR)/ex_permutations.exe

windows-runtests:
	@ if [ -f "$(CURDIR)/$(DISTDIR)/fileutils.exe" ]; then $(CURDIR)/$(DISTDIR)/fileutils.exe; fi
	@ if [ -f "$(CURDIR)/$(DISTDIR)/bitarray.exe" ]; then $(CURDIR)/$(DISTDIR)/bitarray.exe; fi
	@ if [ -f "$(CURDIR)/$(DISTDIR)/strlib.exe" ]; then $(CURDIR)/$(DISTDIR)/strlib.exe; fi
	@ if [ -f "$(CURDIR)/$(DISTDIR)/timing.exe" ]; then $(CURDIR)/$(DISTDIR)/timing.exe; fi
	@ if [ -f "$(CURDIR)/$(DISTDIR)/linkedlist.exe" ]; then $(CURDIR)/$(DISTDIR)/linkedlist.exe; fi
	@ if [ -f "$(CURDIR)/$(DISTDIR)/doublelinkedlist.exe" ]; then $(CURDIR)/$(DISTDIR)/doublelinkedlist.exe; fi
	@ if [ -f "$(CURDIR)/$(DISTDIR)/queue.exe" ]; then $(CURDIR)/$(DISTDIR)/queue.exe; fi
	@ if [ -f "$(CURDIR)/$(DISTDIR)/stack.exe" ]; then $(CURDIR)/$(DISTDIR)/stack.exe; fi
	@ if [ -f "$(CURDIR)/$(DISTDIR)/graph.exe" ]; then $(CURDIR)/$(DISTDIR)/graph.exe; fi
	@ if [ -f "$(CURDIR)/$(DISTDIR)/permutations.exe" ]; then $(CURDIR)/$(DISTDIR)/permutations.exe; fi

clean:
	if [ -d "$(CURDIR)/$(DISTDIR)/" ]; then rm $(CURDIR)/$(DISTDIR)/*; fi
	if [ -d "$(CURDIR)/$(LIBDIR)/" ]; then rm $(CURDIR)/$(LIBDIR)/*; fi
	rm -f $(CURDIR)/*.gcno
	rm -f $(CURDIR)/*.gcda
	rm -f $(CURDIR)/*.gcov

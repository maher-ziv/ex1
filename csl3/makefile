CC = gcc
OBJS = election.o area.o tribe.o electionTestsExample.o 
EXEC = election
DEBUG_FLAG = -g
COMP_FLAG = -std=c99 -Wall -pedantic-errors -Werror -DNDEBUG

$(EXEC):$(OBJS)
	$(CC) -std=c99 -o $@ $(DEBUG_FLAG) $(OBJS) -L. -lmap

election.o: election.c election.h mtm_map/map.h area.h tribe.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
area.o: area.c area.h mtm_map/map.h 
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
tribe.o: tribe.c tribe.h mtm_map/map.h 
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
electionTestsExample.o: tests/electionTestsExample.c election.h mtm_map/map.h test_utilities.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) tests/$*.c

clean:
	rm -f $(OBJS) $(EXEC)

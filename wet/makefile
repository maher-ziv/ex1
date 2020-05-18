CC = gcc
OBJS = election.o area.o tribe.o 2electionTestsExample.o map.o
EXEC = election
DEBUG_FLAG = -g
COMP_FLAG = -std=c99 -Wall -pedantic-errors -Werror -DNDEBUG

$(EXEC):$(OBJS)
	$(CC) -std=c99 -o $@ $(DEBUG_FLAG) $(OBJS) 
	#$(CC) -std=c99 -o $@ $(DEBUG_FLAG) $(OBJS) -no-pie  -L./libmap_for_csl3 -lmap

election.o: election.c election.h mtm_map/map.h area.h tribe.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
area.o: area.c area.h mtm_map/map.h 
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
tribe.o: tribe.c tribe.h mtm_map/map.h 
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
2electionTestsExample.o: tests/2electionTestsExample.c election.h mtm_map/map.h test_utilities.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) tests/$*.c

map.o:  mtm_map/map.c mtm_map/map.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) mtm_map/$*.c

clean:
	rm -f $(OBJS) $(EXEC)

#include <stdlib.h>
#include "../election.h"
#include "../test_utilities.h"

/*The number of tests*/
#define NUMBER_TESTS 1

bool deleteOnlyFirstArea (int area_id) {
	return area_id == 1;
}

bool testElectionRemoveAreas() {
	Election election = electionCreate();
	ASSERT_TEST(electionAddArea(election, 1, "first area") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddArea(election, 2, "second area") == ELECTION_SUCCESS);

	ASSERT_TEST(electionRemoveAreas(election, deleteOnlyFirstArea) == ELECTION_SUCCESS);
	electionDestroy(election);
	return true;
}

/*The functions for the tests should be added here*/
bool (*tests[]) (void) = {
                      testElectionRemoveAreas
};

/*The names of the test functions should be added here*/
const char* testNames[] = {
                           "testElectionRemoveAreas"
};

int main(int argc, char *argv[]) {
    if (argc == 1) {
        for (int test_idx = 0; test_idx < NUMBER_TESTS; test_idx++) {
              RUN_TEST(tests[test_idx], testNames[test_idx]);
        }
        return 0;
    }
    if (argc != 2) {
      fprintf(stdout, "Usage: election <test index>\n");
      return 0;
  }

  int test_idx = strtol(argv[1], NULL, 10);
  if (test_idx < 1 || test_idx > NUMBER_TESTS) {
      fprintf(stderr, "Invalid test index %d\n", test_idx);
      return 0;
  }

  RUN_TEST(tests[test_idx - 1], testNames[test_idx - 1]);
  return 0;
}
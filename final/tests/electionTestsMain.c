#include <stdio.h>
#include <stdlib.h>
#include "electionTests.h"
#include "../test_utilities.h"

#define NUMBER_TESTS 11

bool (*tests[]) (void) = {
                      testElectionCreate,
                      testElectionDestroy,
                      testElectionAddTribe,
                      testElectionAddArea,
                      testElectionGetTribeName,
                      testElectionSetTribeName,
                      testElectionRemoveTribe,
                      testElectionRemoveAreas,
                      testElectionAddVote,
                      testElectionRemoveVote,
                      testElectionComputeAreasToTribesMapping
};

const char* testNames[] = {
                           "testElectionCreate",
                           "testElectionDestroy",
                           "testElectionAddTribe",
                           "testElectionAddArea",
                           "testElectionGetTribeName",
                           "testElectionSetTribeName",
                           "testElectionRemoveTribe",
                           "testElectionRemoveAreas",
                           "testElectionAddVote",
                           "testElectionRemoveVote",
                           "testElectionComputeAreasToTribesMapping "
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
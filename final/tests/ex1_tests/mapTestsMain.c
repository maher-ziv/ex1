//
// Created by bardv on 3/19/2020.
//

#include "../test_utilities.h"
#include <stdlib.h>
#include "../mtm_map/map.h"
#include "mapTests.h"

#define NUMBER_TESTS 11

bool (*tests[]) (void) = {
                      testMapCreate,
                      testMapCopy,
                      testMapDestroy,
                      testMapGetSize,
                      testMapContains,
                      testMapPut,
                      testMapGet,
                      testMapRemove,
                      testMapGetFirst,
                      testMapGetNext,
                      testMapClear
};

const char* testNames[] = {
                           "testMapCreate",
                           "testMapCopy",
                           "testMapDestroy",
                           "testMapGetSize",
                           "testMapContains",
                           "testMapPut",
                           "testMapGet",
                           "testMapRemove",
                           "testMapGetFirst",
                           "testMapGetNext",
                           "testMapClear"
};

int main(int argc, char *argv[]) {
    if (argc == 1) {
        for (int test_idx = 0; test_idx < NUMBER_TESTS; test_idx++) {
              RUN_TEST(tests[test_idx], testNames[test_idx]);
        }
        return 0;
    }
    if (argc != 2) {
      fprintf(stdout, "Usage: map <test index>\n");
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
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "../election.h"
#include "../test_utilities.h"

/*The number of tests*/
#define NUMBER_TESTS 5
#define MAX_STR_LEN 200 //Must be greater than 1
#define MAX_VOTES 100 //Must be greater than 1
#define NUM_OF_AREAS 300 //Do not lower beneath 12
#define NUM_OF_TRIBES 500 //Do not lower beneath 12

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define FIRST_TRIBE 3
#define SECOND_TRIBE 4
#define THIRD_TRIBE 5
#define FIRST_AREA 1
#define SECOND_AREA 2 
#define THIRD_AREA 3   
#define FOURTH_AREA 404

static char *rand_string(char *str, size_t size)
{
    const char charset[] = "abcdefghijklmnopqrstuvwxyz ";
    if (size)
    {
        --size;
        for (size_t n = 0; n < size; n++)
        {
            srand(time(NULL)*n);
            int key = rand() % (int) (sizeof charset - 1);
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
    return str;
}

static int rand_int(int max)
{
    int res = 0;
    static int feed = 251640;
    srand(time(NULL)*(++feed));
    res = (rand()*feed) % (max + 1);
    return abs(res);
}

bool deleteOnlyFirstArea(int area_id) {
    return area_id == 1;
}

bool deleteEvenNumberAreas(int area_id)
{
    return !(area_id % 2);
}

bool deleteAllAreas(int area_id)
{
    return area_id >= 0;
}

bool testElectionRemoveAreas() {
    Election election = electionCreate();
    ASSERT_TEST(electionAddArea(election, FIRST_AREA, "first area") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddArea(election, SECOND_AREA, "second area") == ELECTION_SUCCESS); 
    ASSERT_TEST(electionRemoveAreas(election, deleteOnlyFirstArea) == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddArea(election, SECOND_AREA, "second area") == ELECTION_AREA_ALREADY_EXIST);
    ASSERT_TEST(electionAddArea(election, 3, "THIRD ") == ELECTION_INVALID_NAME);
    electionDestroy(election);
    return true;
}
bool testElectionRemoveAddtribe()
{
    Election election = electionCreate();
    ASSERT_TEST(electionAddTribe(election, FIRST_TRIBE, "first tribe") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddTribe(election, SECOND_TRIBE, "second tribe") == ELECTION_SUCCESS);
    ASSERT_TEST(electionSetTribeName(election, FIRST_TRIBE , "first") == ELECTION_SUCCESS);
    char *name = NULL;
    ASSERT_TEST((name = electionGetTribeName(election, 4)) != NULL);
    ASSERT_TEST(strcmp(name, "second tribe") == 0);
    free(name);
    ASSERT_TEST(electionSetTribeName(election, 2, "first") == ELECTION_TRIBE_NOT_EXIST);
    ASSERT_TEST(electionRemoveTribe(election, FIRST_TRIBE) == ELECTION_SUCCESS);
    ASSERT_TEST(electionRemoveTribe(election,FIRST_TRIBE) == ELECTION_TRIBE_NOT_EXIST);
    ASSERT_TEST(electionSetTribeName(election, -2, "third") == ELECTION_INVALID_ID);
    ASSERT_TEST(electionSetTribeName(election, SECOND_TRIBE, "SECOND") == ELECTION_INVALID_NAME);
    electionDestroy(election);
    return true;
}
bool  testAddRemoveVotes() {
    Election election = electionCreate();
    ASSERT_TEST(electionAddArea(election, FIRST_AREA, "first area") == ELECTION_SUCCESS); //create area 1
    ASSERT_TEST(electionAddArea(election, SECOND_AREA, "second area") == ELECTION_SUCCESS); //create area 2
    ASSERT_TEST(electionAddTribe(election, FIRST_TRIBE, "first tribe") == ELECTION_SUCCESS); //create tribe 1
    ASSERT_TEST(electionAddTribe(election, SECOND_TRIBE, "second tribe") == ELECTION_SUCCESS); //create tribe 2
    ASSERT_TEST(electionAddTribe(election, THIRD_TRIBE, "third tribe") == ELECTION_SUCCESS); //create tribe 3
    ASSERT_TEST(electionAddVote(election,FIRST_AREA, FIRST_TRIBE, 10) == ELECTION_SUCCESS); //+10 votes: area 1->tribe 1
    ASSERT_TEST(electionAddVote(election, FIRST_AREA,SECOND_TRIBE, 14) == ELECTION_SUCCESS); //+14 votes: area 1->tribe 2
    ASSERT_TEST(electionAddVote(election, SECOND_AREA, FIRST_TRIBE, 17) == ELECTION_SUCCESS); //+17 votes: area 2->tribe 1
    ASSERT_TEST(electionRemoveVote(election,FIRST_AREA, SECOND_TRIBE, 4) == ELECTION_SUCCESS); //-4 votes: area 1->tribe 2 :: TOTAL: 10
    ASSERT_TEST(electionAddVote(election, FIRST_AREA, SECOND_TRIBE, -1) == ELECTION_INVALID_VOTES);
    ASSERT_TEST(electionAddVote(NULL, SECOND_AREA, SECOND_TRIBE, 3) == ELECTION_NULL_ARGUMENT);
    ASSERT_TEST(electionAddVote(election, FIRST_AREA, 7, 27) == ELECTION_TRIBE_NOT_EXIST);
    ASSERT_TEST(electionRemoveVote(election, 9, SECOND_TRIBE,6) ==ELECTION_AREA_NOT_EXIST);
    ASSERT_TEST(electionRemoveVote(election, FIRST_AREA,THIRD_TRIBE, 6) == ELECTION_SUCCESS); //-6 votes: area 1->tribe 3 :: TOTAL: STAYS 0
    ASSERT_TEST(electionAddVote(election, FIRST_AREA, THIRD_TRIBE,5) == ELECTION_SUCCESS); //+5 votes: area1->tribe3 :: TOTAL 5
    ASSERT_TEST(electionRemoveVote(election, FIRST_AREA, THIRD_TRIBE, 30) == ELECTION_SUCCESS); //-30 votes: area1->tribe3 :: TOTAL 0
    electionDestroy(election);
    return true;
}

bool testComputeAreasToTribesMapping()
{
    Election election = electionCreate();
    ASSERT_TEST(electionAddArea(election, FIRST_AREA, "first area") == ELECTION_SUCCESS); //create area 1
    ASSERT_TEST(electionAddArea(election, SECOND_AREA, "second area") == ELECTION_SUCCESS); //create area 2
    ASSERT_TEST(electionAddArea(election, THIRD_AREA, "third area") == ELECTION_SUCCESS); //create area 3
    ASSERT_TEST(electionAddArea(election, FOURTH_AREA, "fourth area") == ELECTION_SUCCESS); //create area 4
    ASSERT_TEST(electionAddTribe(election, FIRST_TRIBE, "first tribe") == ELECTION_SUCCESS); //create tribe 1
    ASSERT_TEST(electionAddTribe(election, SECOND_TRIBE, "second tribe") == ELECTION_SUCCESS); //create tribe 2
    ASSERT_TEST(electionAddTribe(election, THIRD_TRIBE, "third tribe") == ELECTION_SUCCESS); //create tribe 3
    ASSERT_TEST(electionAddVote(election,FIRST_AREA, FIRST_TRIBE, 10) == ELECTION_SUCCESS); //+10 votes: area 1->tribe 1 :: TOTAL: 10/
    ASSERT_TEST(electionAddVote(election, FIRST_AREA,SECOND_TRIBE, 14) == ELECTION_SUCCESS); //+14 votes: area 1->tribe 2 :: TOTAL: 14
    ASSERT_TEST(electionAddVote(election, SECOND_AREA, FIRST_TRIBE, 17) == ELECTION_SUCCESS); //+17 votes: area 2->tribe 1 :: TOTAL: 17
    ASSERT_TEST(electionRemoveVote(election,FIRST_AREA, SECOND_TRIBE, 4) == ELECTION_SUCCESS); //-4 votes: area 1->tribe 2 :: TOTAL: 10/
    ASSERT_TEST(electionAddVote(election, FIRST_AREA, SECOND_TRIBE, -1) == ELECTION_INVALID_VOTES);
    ASSERT_TEST(electionAddVote(NULL, SECOND_AREA, SECOND_TRIBE, 3) == ELECTION_NULL_ARGUMENT);
    ASSERT_TEST(electionAddVote(election, FIRST_AREA, 7, 27) == ELECTION_TRIBE_NOT_EXIST);
    ASSERT_TEST(electionRemoveVote(election, 9, SECOND_TRIBE,6) ==ELECTION_AREA_NOT_EXIST);
    ASSERT_TEST(electionRemoveVote(election, FIRST_AREA,THIRD_TRIBE, 6) == ELECTION_SUCCESS); //-6 votes: area 1->tribe 3 :: TOTAL: STAYS 0
    ASSERT_TEST(electionAddVote(election, FIRST_AREA, THIRD_TRIBE,5) == ELECTION_SUCCESS); //+5 votes: area 1->tribe 3 :: TOTAL: 5
    ASSERT_TEST(electionAddVote(election, FOURTH_AREA, SECOND_TRIBE,10) == ELECTION_SUCCESS); //+10 votes: area 4->tribe 2 :: TOTAL: 10
    ASSERT_TEST(electionAddVote(election, FOURTH_AREA, THIRD_TRIBE,50) == ELECTION_SUCCESS); //+50 votes: area 4->tribe 3 :: TOTAL: 50
    ASSERT_TEST(electionRemoveVote(election, FOURTH_AREA, THIRD_TRIBE,45) == ELECTION_SUCCESS); //-45 votes: area 4->tribe 3 :: TOTAL: 5
    ASSERT_TEST(electionRemoveVote(election, FIRST_AREA, THIRD_TRIBE, 30) == ELECTION_SUCCESS); //-30 votes: area 1->tribe 3 :: TOTAL: 0/
    ASSERT_TEST(electionAddVote(election, SECOND_AREA, THIRD_TRIBE,200) == ELECTION_SUCCESS); //+200 votes: area 2->tribe 3 :: TOTAL: 200
    /**
     * LOWEST ID: FIRST_TRIBE
     * MAX VOTES AREA 1: FIRST_TRIBE (same as SECOND_TRIBE)
     * MAX VOTES AREA 2: THIRD_TRIBE
     * MAX VOTES AREA 3: LOWEST ID
     * MAX VOTES AREA 4: SECOND_TRIBE
     * */
    Map tester = NULL;
    ASSERT_TEST((tester = electionComputeAreasToTribesMapping(election)) != NULL);
    ASSERT_TEST((strcmp(mapGet(tester, TOSTRING(FIRST_AREA)), TOSTRING(FIRST_TRIBE))==0));
    ASSERT_TEST(strcmp(mapGet(tester, TOSTRING(SECOND_AREA)), TOSTRING(THIRD_TRIBE))==0);
    ASSERT_TEST(strcmp(mapGet(tester, TOSTRING(THIRD_AREA)), TOSTRING(FIRST_TRIBE))==0);
    ASSERT_TEST(strcmp(mapGet(tester, TOSTRING(FOURTH_AREA)), TOSTRING(SECOND_TRIBE))==0);

    Map copy = mapCopy(tester);

    mapDestroy(copy);
    mapDestroy(tester);
    electionDestroy(election);
    return true;
}

bool raiseHell()
{
    Election election = electionCreate();
    char *tmp = malloc(MAX_STR_LEN + 1);
    assert(tmp != NULL);
    for(int i = 0; i < NUM_OF_AREAS; i++)
    {
        ASSERT_TEST(electionAddArea(election, i, "ooga") == ELECTION_SUCCESS);
    }
    for(int i = 0; i < NUM_OF_TRIBES; i++)
    {
        int len = rand_int(MAX_STR_LEN);
        rand_string(tmp, len == 0? 1 : len);
        ASSERT_TEST(electionAddTribe(election, i, tmp) == ELECTION_SUCCESS);
    }
    ASSERT_TEST(electionAddArea(election, NUM_OF_AREAS-1, tmp) == ELECTION_AREA_ALREADY_EXIST);
    ASSERT_TEST(electionAddTribe(election, NUM_OF_TRIBES-1, tmp) == ELECTION_TRIBE_ALREADY_EXIST);
    ASSERT_TEST(electionRemoveAreas(election, deleteEvenNumberAreas) == ELECTION_SUCCESS);
    for(int i = 0; i < NUM_OF_AREAS; i += 2)
    {
        ASSERT_TEST(electionAddArea(election, i, "booga") == ELECTION_SUCCESS);
    }
    int id1 = rand_int(NUM_OF_TRIBES-1);
    int id2 = 0;
    ASSERT_TEST(electionRemoveTribe(election, id1) == ELECTION_SUCCESS);
    for(int i = 0; i < NUM_OF_AREAS; i++)
    {
        do
        {
            id2 = rand_int(NUM_OF_TRIBES-1);
        } while (id2 == id1);
        ASSERT_TEST(electionAddVote(election, i, id2,rand_int(MAX_VOTES) + 1) == ELECTION_SUCCESS);
    }
    Map statistics1, statistics2;
    ASSERT_TEST((statistics1 = electionComputeAreasToTribesMapping(election)) != NULL);
    ASSERT_TEST(electionAddTribe(election, id1, tmp) == ELECTION_SUCCESS);
    ASSERT_TEST(electionRemoveAreas(election, deleteAllAreas) == ELECTION_SUCCESS);
    ASSERT_TEST((statistics2 = electionComputeAreasToTribesMapping(election)) != NULL);
    ASSERT_TEST(mapGetFirst(statistics2) == NULL);
    mapDestroy(statistics1);
    mapDestroy(statistics2);
    electionDestroy(election);
    free(tmp);
    return true;
}

/*The functions for the tests should be added here*/
bool (*tests[]) (void) = {
                        testElectionRemoveAreas,
                        testElectionRemoveAddtribe,
                        testAddRemoveVotes,
                        testComputeAreasToTribesMapping,
                        raiseHell
};

/*The names of the test functions should be added here*/
const char* testNames[] = {
                            "testElectionRemoveAreas",
                            "testElectionRemoveAddtribe",
                            "testAddRemoveVotes",
                            "testComputeAreasToTribesMapping",
                            "raiseHell"
};

int main(int argc, char* argv[]) {
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
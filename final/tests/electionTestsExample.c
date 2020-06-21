#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#include "../election.h"
#include "../test_utilities.h"
#include "../mtm_map/map.h"

/*The number of tests*/
#define NUMBER_TESTS 4

enum { FIRST_AREA=1 , SECOND_AREA , THIRD_AREA, FOURTH_AREA };
enum { FIRST_TRIBE=1 , SECOND_TRIBE , THIRD_TRIBE, FOURTH_TRIBE};

#define INT_TO_STING(num, str)              \
      do{                                        \
                sprintf(str,"%d",num);        \
      }while(0) 


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

//Test_2
bool deleteArea (int area_id) {
	return area_id == 1 || area_id==3;
}
bool deleteNonExistsArea (int area_id) {
	return area_id == 0 || area_id==-19 || area_id==8;
}
bool testElectionRemoveAreas_2(){
  Election election=electionCreate();
  ASSERT_TEST(electionAddArea(election, 1, "first area") == ELECTION_SUCCESS);
  ASSERT_TEST(electionAddArea(election, 2, "second area") == ELECTION_SUCCESS);
  ASSERT_TEST(electionAddArea(election, 1, "first area") == ELECTION_AREA_ALREADY_EXIST);
  ASSERT_TEST(electionRemoveAreas(election, deleteOnlyFirstArea) == ELECTION_SUCCESS);
  ASSERT_TEST(electionRemoveAreas(election, deleteArea) == ELECTION_SUCCESS);
  ASSERT_TEST(electionAddArea(election, -9, "second area") == ELECTION_INVALID_ID);
  ASSERT_TEST(electionAddArea(election, 3, "secoEd area") == ELECTION_INVALID_NAME);
  ASSERT_TEST(electionRemoveAreas(election, deleteOnlyFirstArea) == ELECTION_SUCCESS);

    electionDestroy(election);
  return true;
}
//Test_3
bool testElectionAddTribe(){
  Election election=electionCreate();
  ASSERT_TEST(electionAddArea(election, FIRST_AREA, "first area") == ELECTION_SUCCESS); //create area 1
  ASSERT_TEST(electionAddArea(election, SECOND_AREA, "second area") == ELECTION_SUCCESS); //create area 2
 // ASSERT_TEST(electionAddArea(election, THIRD_AREA, "third area") == ELECTION_SUCCESS);  
  
  ASSERT_TEST(electionAddTribe(election,1,"first tribe")==ELECTION_SUCCESS);
  ASSERT_TEST(electionAddTribe(election,2,"second tribe")==ELECTION_SUCCESS);
  ASSERT_TEST(electionAddTribe(election,THIRD_TRIBE,"thered tribe")==ELECTION_SUCCESS);
  ASSERT_TEST(electionAddTribe(election,FOURTH_TRIBE,"forth tribe")==ELECTION_SUCCESS);
  ASSERT_TEST(electionAddTribe(election,2,"secondtribe")==ELECTION_TRIBE_ALREADY_EXIST);
  ASSERT_TEST(electionAddTribe(election,5,"5'th tribe")==ELECTION_INVALID_NAME);
  ASSERT_TEST(electionRemoveTribe(election,FOURTH_TRIBE)==ELECTION_SUCCESS);
  ASSERT_TEST(electionRemoveTribe(election,THIRD_TRIBE)==ELECTION_SUCCESS);
  ASSERT_TEST(electionAddTribe(election,FOURTH_TRIBE,"forth tribe")==ELECTION_SUCCESS);
  ASSERT_TEST(electionRemoveTribe(election,FIRST_TRIBE)==ELECTION_SUCCESS);
  ASSERT_TEST(electionAddTribe(election,FIRST_TRIBE,"first tribe")==ELECTION_SUCCESS); 

  ASSERT_TEST(electionAddVote(election,SECOND_AREA, FIRST_TRIBE, 10) == ELECTION_SUCCESS); //+10 votes: area 1->tribe 1 :: TOTAL: 10/
  ASSERT_TEST(electionAddVote(election, SECOND_AREA,SECOND_TRIBE, 14) == ELECTION_SUCCESS); //+14 votes: area 1->tribe 2 :: TOTAL: 14
  ASSERT_TEST(electionAddVote(election, SECOND_AREA, FOURTH_TRIBE, 17) == ELECTION_SUCCESS); //+17 votes: area 2->tribe 1 :: TOTAL: 17
  ASSERT_TEST(electionRemoveVote(election,SECOND_AREA, SECOND_TRIBE, 4) == ELECTION_SUCCESS); 
  
  ASSERT_TEST(electionRemoveTribe(election,FOURTH_TRIBE)==ELECTION_SUCCESS);
  ASSERT_TEST(electionAddTribe(election,FOURTH_TRIBE,"forth tribe")==ELECTION_SUCCESS);
  ASSERT_TEST(electionRemoveTribe(election,FIRST_TRIBE)==ELECTION_SUCCESS);
  ASSERT_TEST(electionAddTribe(election,1,"first tribe")==ELECTION_SUCCESS);
  ASSERT_TEST(electionRemoveTribe(election,FIRST_TRIBE)==ELECTION_SUCCESS);
  ASSERT_TEST(electionAddTribe(election,1,"first tribe")==ELECTION_SUCCESS);

  char* test=NULL;
  test=electionGetTribeName(election,333);
  ASSERT_TEST(test==NULL);
  ASSERT_TEST(electionSetTribeName(election,1,"new first tribe")==ELECTION_SUCCESS);
  test=electionGetTribeName(election,1);
  ASSERT_TEST((strcmp (test,"new first tribe") == 0));
  ASSERT_TEST(electionRemoveTribe(election,6)==ELECTION_TRIBE_NOT_EXIST);

  free(test);
  electionDestroy(election);
  return true;
}

//Test_4
bool testElectionMapping(){

  Election election=electionCreate();
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
    ASSERT_TEST(!strcmp( mapGet(tester, "1") , "1"));
    ASSERT_TEST(!strcmp(mapGet(tester, "2"), "3"));
    ASSERT_TEST(!strcmp(mapGet(tester, "3"),"1"));
    ASSERT_TEST(!strcmp(mapGet(tester, "4"),"2"));

     Map mapcpy = mapCopy(tester);
     
     mapDestroy(mapcpy);
   
   mapDestroy(tester);
  electionDestroy(election);


  return true;
}











/*-------------------------------------------------------------------------------------*/

/*The functions for the tests should be added here*/
bool (*tests[]) (void) = {
                      testElectionRemoveAreas,
                      testElectionRemoveAreas_2,
                      testElectionAddTribe,
                      testElectionMapping
};

/*The names of the test functions should be added here*/
const char* testNames[] = {
                           "testElectionRemoveAreas",
                           "testElectionRemoveAreas_2",
                           "testElectionAddTribe",
                           "testElectionMapping"
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

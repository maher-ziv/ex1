#include <stdlib.h>
#include "electionTests.h"
#include "../election.h"
#include "../test_utilities.h"

bool testElectionCreate() {
    Election election = electionCreate();
    ASSERT_TEST(election != NULL);
    electionDestroy(election);
    return true;
}

bool testElectionDestroy() {
    Election election = electionCreate();
    electionDestroy(election);
    return true;
}

bool testElectionAddTribe() {
    Election election = electionCreate();

    //check ELECTION_NULL_ARGUMENT:
    ASSERT_TEST(electionAddTribe(NULL, 1, "tribename") == ELECTION_NULL_ARGUMENT);
    ASSERT_TEST(electionAddTribe(election, 1, NULL) == ELECTION_NULL_ARGUMENT);

    //check ELECTION_INVALID_ID :
    ASSERT_TEST(electionAddTribe(election, -1, "tribename") == ELECTION_INVALID_ID);

    //check ELECTION_PARTY_ALREADY_EXIST :
    ASSERT_TEST(electionAddTribe(election, 0, "tribename") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddTribe(election, 0, "tribename") == ELECTION_TRIBE_ALREADY_EXIST);
    ASSERT_TEST(electionAddTribe(election, 1, "tribename") == ELECTION_SUCCESS);

    //check ELECTION_INVALID_NAME :
    ASSERT_TEST(electionAddTribe(election, 2, "tribeName") == ELECTION_INVALID_NAME);
    ASSERT_TEST(electionAddTribe(election, 2, "tribename@") == ELECTION_INVALID_NAME);
    ASSERT_TEST(electionAddTribe(election, 2, "tribe name") == ELECTION_SUCCESS);

    //check the order of the errors is correct:
    ASSERT_TEST(electionAddTribe(NULL, -1, "tribename@") == ELECTION_NULL_ARGUMENT);
    ASSERT_TEST(electionAddTribe(election, -1, "tribename@") == ELECTION_INVALID_ID);
    ASSERT_TEST(electionAddTribe(election, 1, "tribename@") == ELECTION_TRIBE_ALREADY_EXIST);


    electionDestroy(election);
    return true;
}

bool testElectionAddArea() {
    Election election = electionCreate();

    //check ELECTION_NULL_ARGUMENT:
    ASSERT_TEST(electionAddArea(NULL, 1, "areaname") == ELECTION_NULL_ARGUMENT);
    ASSERT_TEST(electionAddArea(election, 1, NULL) == ELECTION_NULL_ARGUMENT);

    //check ELECTION_INVALID_ID :
    ASSERT_TEST(electionAddArea(election, -1, "areaname") == ELECTION_INVALID_ID);

    //check ELECTION_AREA_ALREADY_EXIST :
    ASSERT_TEST(electionAddArea(election, 0, "areaname") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddArea(election, 0, "areaname") == ELECTION_AREA_ALREADY_EXIST);
    ASSERT_TEST(electionAddArea(election, 1, "areaname") == ELECTION_SUCCESS);

    //check ELECTION_INVALID_NAME :
    ASSERT_TEST(electionAddArea(election, 2, "areaName") == ELECTION_INVALID_NAME);
    ASSERT_TEST(electionAddArea(election, 2, "areaname@") == ELECTION_INVALID_NAME);
    ASSERT_TEST(electionAddArea(election, 2, "areaname") == ELECTION_SUCCESS);

    //check the order of the errors is correct:
    ASSERT_TEST(electionAddArea(NULL, -1, "areaname@") == ELECTION_NULL_ARGUMENT);
    ASSERT_TEST(electionAddArea(election, -1, "areaname@") == ELECTION_INVALID_ID);
    ASSERT_TEST(electionAddArea(election, 1, "areaname@") == ELECTION_AREA_ALREADY_EXIST);

    electionDestroy(election);
    return true;
}

bool testElectionGetTribeName() {
    Election election = electionCreate();

    ASSERT_TEST(electionAddTribe(election, 1, "tribe name") == ELECTION_SUCCESS);

    //NULL argument:
    ASSERT_TEST(electionGetTribeName(NULL, 1) == NULL);

    //tribe doesn't exists:
    ASSERT_TEST(electionGetTribeName(election, -1) == NULL);
    ASSERT_TEST(electionGetTribeName(election, 2) == NULL);

    char *tribe_name = electionGetTribeName(election, 1);
    ASSERT_TEST(strcmp(tribe_name, "tribe name") == 0);

    free(tribe_name);
    electionDestroy(election);
    return true;
}


bool testElectionSetTribeName() {
    Election election = electionCreate();
    ASSERT_TEST(electionAddTribe(election, 1, "tribe name") == ELECTION_SUCCESS);

    //check ELECTION_NULL_ARGUMENT:
    ASSERT_TEST(electionSetTribeName(NULL, 1, "new name") == ELECTION_NULL_ARGUMENT);
    ASSERT_TEST(electionSetTribeName(election, 1, NULL) == ELECTION_NULL_ARGUMENT);
    ASSERT_TEST(electionSetTribeName(NULL, 1, NULL) == ELECTION_NULL_ARGUMENT);

    //check ELECTION_INVALID_ID:
    ASSERT_TEST(electionSetTribeName(election, -1, "new name") == ELECTION_INVALID_ID);

    //check ELECTION_PARTY_NOT_EXIST:
    ASSERT_TEST(electionSetTribeName(election, 2, "new name") == ELECTION_TRIBE_NOT_EXIST);

    //check ELECTION_INVALID_NAME:
    ASSERT_TEST(electionSetTribeName(election, 1, "new name@") == ELECTION_INVALID_NAME);
    ASSERT_TEST(electionSetTribeName(election, 1, "new Name") == ELECTION_INVALID_NAME);

    //good scenario:
    ASSERT_TEST(electionSetTribeName(election, 1, "new name") == ELECTION_SUCCESS);
    char *new_name = electionGetTribeName(election, 1);
    ASSERT_TEST(strcmp(new_name, "new name") == 0);

    //check the order of the errors is correct:
    ASSERT_TEST(electionSetTribeName(NULL, -1, "new name@") == ELECTION_NULL_ARGUMENT);
    ASSERT_TEST(electionSetTribeName(election, -1, "new name@") == ELECTION_INVALID_ID);
    ASSERT_TEST(electionSetTribeName(election, 2, "new name@") == ELECTION_TRIBE_NOT_EXIST);

    free(new_name);
    electionDestroy(election);
    return true;
}

bool testElectionRemoveTribe() {
    Election election = electionCreate();
    ASSERT_TEST(electionAddTribe(election, 1, "tribe name") == ELECTION_SUCCESS);

    //check ELECTION_NULL_ARGUMENT:
    ASSERT_TEST(electionRemoveTribe(NULL, 1) == ELECTION_NULL_ARGUMENT);

    //check ELECTION_INVALID_ID:
    ASSERT_TEST(electionRemoveTribe(election, -1) == ELECTION_INVALID_ID);

    //check ELECTION_PARTY_NOT_EXIST:
    ASSERT_TEST(electionRemoveTribe(election, 2) == ELECTION_TRIBE_NOT_EXIST);

    //good scenario:
    ASSERT_TEST(electionRemoveTribe(election, 1) == ELECTION_SUCCESS);
    ASSERT_TEST(electionGetTribeName(election, 1) == NULL);
    ASSERT_TEST(electionSetTribeName(election, 1, "new name") == ELECTION_TRIBE_NOT_EXIST);

    //check the order of the errors is correct:
    ASSERT_TEST(electionRemoveTribe(NULL, -1) == ELECTION_NULL_ARGUMENT);

    electionDestroy(election);
    return true;
}

bool electionDeleteEvenAreas (int area_id) {
    if(area_id % 2 == 0) {
        return true;
    }
    return false;
}

bool testElectionRemoveAreas() {
    Election election = electionCreate();
    ASSERT_TEST(electionAddArea(election, 1, "area name") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddArea(election, 2, "area name") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddArea(election, 3, "area name") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddArea(election, 4, "area name") == ELECTION_SUCCESS);

    //check ELECTION_NULL_ARGUMENT:
    ASSERT_TEST(electionRemoveAreas(NULL, NULL) == ELECTION_NULL_ARGUMENT);
    //TODO
    //ASSERT_TEST(electionRemoveAreas(election, NULL) == ELECTION_NULL_ARGUMENT);
    ASSERT_TEST(electionRemoveAreas(NULL, electionDeleteEvenAreas) == ELECTION_NULL_ARGUMENT);

    //good scenario:
    ASSERT_TEST(electionRemoveAreas(election, electionDeleteEvenAreas) == ELECTION_SUCCESS);

    //check if only even areas where deleted:

    ASSERT_TEST(electionAddArea(election, 1, "area name") == ELECTION_AREA_ALREADY_EXIST);
    ASSERT_TEST(electionAddArea(election, 2, "area name") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddArea(election, 3, "area name") == ELECTION_AREA_ALREADY_EXIST);
    ASSERT_TEST(electionAddArea(election, 4, "area name") == ELECTION_SUCCESS);

    electionDestroy(election);
    return true;
}

bool testElectionAddVote() {
    Election election = electionCreate();

    //add tribes:
    ASSERT_TEST(electionAddTribe(election, 1, "first tribe") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddTribe(election, 2, "second tribe") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddTribe(election, 3, "third tribe") == ELECTION_SUCCESS);
    //add areas:
    ASSERT_TEST(electionAddArea(election, 1, "first area") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddArea(election, 2, "second area") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddArea(election, 3, "third area") == ELECTION_SUCCESS);

    //check ELECTION_NULL_ARGUMENT:
    ASSERT_TEST(electionAddVote(NULL, 1, 1, 1) == ELECTION_NULL_ARGUMENT);

    //check ELECTION_INVALID_ID:
    ASSERT_TEST(electionAddVote(election, -1, 1, 1) == ELECTION_INVALID_ID);
    ASSERT_TEST(electionAddVote(election, 1, -1, 1) == ELECTION_INVALID_ID);

    //check ELECTION_INVALID_VOTES:
    ASSERT_TEST(electionAddVote(election, 1, 1, -1) == ELECTION_INVALID_VOTES);
    ASSERT_TEST(electionAddVote(election, 1, 1, 0) == ELECTION_INVALID_VOTES);

    //check ELECTION_AREA_NOT_EXIST:
    ASSERT_TEST(electionAddVote(election, 4, 1, 1) == ELECTION_AREA_NOT_EXIST);

    //check ELECTION_PARTY_NOT_EXIST :
    ASSERT_TEST(electionAddVote(election, 1, 4, 1) == ELECTION_TRIBE_NOT_EXIST);

    //check the order of the errors is correct:
    ASSERT_TEST(electionAddVote(NULL, -1, 4, -1) == ELECTION_NULL_ARGUMENT);
    ASSERT_TEST(electionAddVote(election, -1, 4, -1) == ELECTION_INVALID_ID);
    ASSERT_TEST(electionAddVote(election, 4, -1, -1) == ELECTION_INVALID_ID);
    ASSERT_TEST(electionAddVote(election, 4, 4, -1) == ELECTION_INVALID_VOTES);
    ASSERT_TEST(electionAddVote(election, 4, 4, 1) == ELECTION_AREA_NOT_EXIST);

    //good scenarios:
    ASSERT_TEST(electionAddVote(election, 1, 1, 10) == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddVote(election, 1, 2, 10) == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddVote(election, 1, 3, 10) == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddVote(election, 2, 1, 10) == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddVote(election, 2, 2, 10) == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddVote(election, 2, 3, 10) == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddVote(election, 3, 1, 10) == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddVote(election, 3, 2, 10) == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddVote(election, 3, 3, 10) == ELECTION_SUCCESS);

    electionDestroy(election);
    return true;
}

bool testElectionRemoveVote() {
    Election election = electionCreate();

    //add tribes:
    ASSERT_TEST(electionAddTribe(election, 1, "first tribe") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddTribe(election, 2, "second tribe") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddTribe(election, 3, "third tribe") == ELECTION_SUCCESS);
    //add areas:
    ASSERT_TEST(electionAddArea(election, 1, "first area") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddArea(election, 2, "second area") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddArea(election, 3, "third area") == ELECTION_SUCCESS);

    //check ELECTION_NULL_ARGUMENT:
    ASSERT_TEST(electionRemoveVote(NULL, 1, 1, 1) == ELECTION_NULL_ARGUMENT);

    //check ELECTION_INVALID_ID:
    ASSERT_TEST(electionRemoveVote(election, -1, 1, 1) == ELECTION_INVALID_ID);
    ASSERT_TEST(electionRemoveVote(election, 1, -1, 1) == ELECTION_INVALID_ID);

    //check ELECTION_INVALID_VOTES:
    ASSERT_TEST(electionRemoveVote(election, 1, 1, -1) == ELECTION_INVALID_VOTES);
    ASSERT_TEST(electionRemoveVote(election, 1, 1, 0) == ELECTION_INVALID_VOTES);

    //check ELECTION_AREA_NOT_EXIST:
    ASSERT_TEST(electionRemoveVote(election, 4, 1, 1) == ELECTION_AREA_NOT_EXIST);

    //check ELECTION_PARTY_NOT_EXIST :
    ASSERT_TEST(electionRemoveVote(election, 1, 4, 1) == ELECTION_TRIBE_NOT_EXIST);

    //check the order of the errors is correct:
    ASSERT_TEST(electionRemoveVote(NULL, -1, 4, -1) == ELECTION_NULL_ARGUMENT);
    ASSERT_TEST(electionRemoveVote(election, -1, 4, -1) == ELECTION_INVALID_ID);
    ASSERT_TEST(electionRemoveVote(election, 4, -1, -1) == ELECTION_INVALID_ID);
    ASSERT_TEST(electionRemoveVote(election, 4, 4, -1) == ELECTION_INVALID_VOTES);
    ASSERT_TEST(electionRemoveVote(election, 4, 4, 1) == ELECTION_AREA_NOT_EXIST);

    //good scenarios:
    ASSERT_TEST(electionAddVote(election, 1, 1, 5) == ELECTION_SUCCESS);
    ASSERT_TEST(electionRemoveVote(election, 1, 1, 3) == ELECTION_SUCCESS);
    ASSERT_TEST(electionRemoveVote(election, 1, 1, 3) == ELECTION_SUCCESS);

    ASSERT_TEST(electionAddVote(election, 1, 2, 5) == ELECTION_SUCCESS);
    ASSERT_TEST(electionRemoveVote(election, 1, 2, 3) == ELECTION_SUCCESS);
    ASSERT_TEST(electionRemoveVote(election, 1, 2, 3) == ELECTION_SUCCESS);

    ASSERT_TEST(electionAddVote(election, 1, 3, 5) == ELECTION_SUCCESS);
    ASSERT_TEST(electionRemoveVote(election, 1, 3, 3) == ELECTION_SUCCESS);
    ASSERT_TEST(electionRemoveVote(election, 1, 3, 3) == ELECTION_SUCCESS);

    electionDestroy(election);
    return true;
}

bool testElectionComputeAreasToTribesMapping() {
    Election election = electionCreate();

    //check NULL argument://TODO
  //  ASSERT_TEST(electionComputeAreasToTribesMapping(NULL) == NULL);

    //add tribes:
    ASSERT_TEST(electionAddTribe(election, 1, "first tribe") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddTribe(election, 2, "second tribe") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddTribe(election, 3, "third tribe") == ELECTION_SUCCESS);
    //add areas:
    ASSERT_TEST(electionAddArea(election, 1, "first area") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddArea(election, 2, "second area") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddArea(election, 3, "third area") == ELECTION_SUCCESS);

    //add votes:
    ASSERT_TEST(electionAddVote(election, 1, 1, 5) == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddVote(election, 1, 2, 5) == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddVote(election, 1, 3, 4) == ELECTION_SUCCESS);

    ASSERT_TEST(electionAddVote(election, 2, 1, 1) == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddVote(election, 2, 2, 8) == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddVote(election, 2, 3, 70) == ELECTION_SUCCESS);

    ASSERT_TEST(electionAddVote(election, 3, 1, 1) == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddVote(election, 3, 2, 2) == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddVote(election, 3, 3, 2) == ELECTION_SUCCESS);

    Map areas_to_tribes = electionComputeAreasToTribesMapping(election);

    ASSERT_TEST(mapContains(areas_to_tribes, "1") == true);
    ASSERT_TEST(mapContains(areas_to_tribes, "2") == true);
    ASSERT_TEST(mapContains(areas_to_tribes, "3") == true);
    ASSERT_TEST(mapContains(areas_to_tribes, "4") == false);

    char *first_area_favorite_tribe = mapGet(areas_to_tribes, "1");
    char *second_area_favorite_tribe = mapGet(areas_to_tribes, "2");
    char *third_area_favorite_tribe = mapGet(areas_to_tribes, "3");

    ASSERT_TEST(strcmp(first_area_favorite_tribe, "1") == 0);
    ASSERT_TEST(strcmp(second_area_favorite_tribe, "3") == 0);
    ASSERT_TEST(strcmp(third_area_favorite_tribe, "2") == 0);

    mapDestroy(areas_to_tribes);
    electionDestroy(election);
    return true;
}



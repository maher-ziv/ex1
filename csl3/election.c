#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include "election.h"
#include "area.h"
#include "tribe.h"


static bool isValidName(const char* name);
static bool electionIsTribeExists(Election election, int tribe_id);
/*-------------------------------------------------------------------------------------*/

struct election_t{    
    Area area;
    Tribe tribe;
};

Election electionCreate(){
    Election election = malloc(sizeof(*election));
    if(election==NULL){
        return NULL;
    }
    election->area = areaCreat();
    if ( election->area == NULL ){
        free(election);
        return NULL;
    }
    election->tribe = tribeCreat();
    if ( election->tribe == NULL ) {
        electionDestroy(election);
        return NULL;
    }
    return election;
}

void electionDestroy(Election election){
    if ( election == NULL ) {
        return;
    }
    areaDestroy(election->area);
    tribeDestroy(election->tribe);
    free(election);
}

ElectionResult electionAddTribe (Election election, int tribe_id, const char* tribe_name){
    if ( (election == NULL) || (tribe_name == NULL) ) {
        return ELECTION_NULL_ARGUMENT;
    }
    if ( tribe_id < 0 ) {
        return ELECTION_INVALID_ID;
    }
    if ( electionIsTribeExists(election,tribe_id) ) {
        return ELECTION_TRIBE_ALREADY_EXIST;
    }
    if ( !isValidName(tribe_name) ) {  
        return ELECTION_INVALID_NAME;
    }
    int result = tribeAdd(election->tribe,tribe_id,tribe_name);
    if ( result == TRIBE_OUT_OF_MEMORY ) {
        return ELECTION_OUT_OF_MEMORY;
    }
    if ( result == TRIBE_ERROR ) {
        return ELECTION_ERROR;
    }
    assert(election->tribe!=NULL);
    return ELECTION_SUCCESS;         
} 

ElectionResult electionAddArea(Election election, int area_id, const char* area_name){
    if ( (election == NULL) || (area_name == NULL) ) {
        return ELECTION_NULL_ARGUMENT;
    }
    if ( area_id < 0 ) {
        return ELECTION_INVALID_ID;
    }
    if ( areaIsExists(election->area,area_id) ) {
        return ELECTION_AREA_ALREADY_EXIST;
    }
    if ( !isValidName(area_name) ) {
        return ELECTION_INVALID_NAME;
    }
    if ( (election->area = areaAdd(election->area,area_id,area_name)) == NULL ) {                                        
       return ELECTION_OUT_OF_MEMORY;
    }
    assert(election->area!=NULL);
    return ELECTION_SUCCESS;         
} 

char* electionGetTribeName (Election election, int tribe_id){
    if ( election == NULL ) {
        return NULL;
    }
    assert( ( election!=NULL ) && ( tribe_id>=0 ) );
    // return NULL if area not found
    char* tribe_name = tribeGetName(election->tribe,tribe_id);
    return tribe_name;
}

ElectionResult electionSetTribeName (Election election, int tribe_id, const char* tribe_name){
    if ( (election == NULL) || (tribe_name == NULL) ) {
        return ELECTION_NULL_ARGUMENT;
    }
    if ( tribe_id < 0) {
        return ELECTION_INVALID_ID;
    }
    if ( !electionIsTribeExists(election,tribe_id) ) {
        return ELECTION_TRIBE_NOT_EXIST;
    }
    if ( !isValidName(tribe_name) ) {
        return ELECTION_INVALID_NAME;
    }
    int result1 = tribeChangeName(election->tribe,tribe_id,tribe_name);
    int result2 = areaChangeTribeName(election->area , tribe_id , tribe_name);
    if ( result1 == TRIBE_OUT_OF_MEMORY || result2 == AREA_OUT_OF_MEMORY) { 
        return ELECTION_OUT_OF_MEMORY;
    }
    if ( result1 == TRIBE_ERROR || result2 == TRIBE_ERROR ) {
        return ELECTION_ERROR;
    }
    return ELECTION_SUCCESS;
}

ElectionResult electionRemoveTribe (Election election, int tribe_id){
    if ( election == NULL ) {
        return ELECTION_NULL_ARGUMENT;
    }
    if ( tribe_id < 0 ) {
        return ELECTION_INVALID_ID;
    }
    if ( !electionIsTribeExists(election,tribe_id) ) {
        return ELECTION_TRIBE_NOT_EXIST;
    }
    int result1 = areaRemoveAllVotesfromTribe(election->area,tribe_id);
    int result2 = tribeDelete(election->tribe,tribe_id);
    if(  result1 == TRIBE_ERROR || result2 == TRIBE_ERROR ) {
        return ELECTION_ERROR;
    }
    return ELECTION_SUCCESS;
}

ElectionResult electionRemoveAreas(Election election, AreaConditionFunction should_delete_area){
    if ( election == NULL ) {
        return ELECTION_NULL_ARGUMENT;
    }
    election->area = areaRemove(election->area,should_delete_area);
    return ELECTION_SUCCESS;
}

ElectionResult electionAddVote (Election election, int area_id, int tribe_id, int num_of_votes){
    if ( election == NULL ) {
        return ELECTION_NULL_ARGUMENT;
    }
    if ( (area_id < 0) || (tribe_id < 0) ){
        return ELECTION_INVALID_ID;
    }
    if ( num_of_votes <= 0 ) {
        return ELECTION_INVALID_VOTES;
    }
    if ( !areaIsExists(election->area,area_id) ) {
        return ELECTION_AREA_NOT_EXIST;
    }
    if ( !electionIsTribeExists(election,tribe_id) ) {
        return ELECTION_TRIBE_NOT_EXIST;
    }
    int result = areaAddVoteToTribe(election->area,area_id,tribe_id,num_of_votes);
    if ( result == AREA_OUT_OF_MEMORY ) {                                                           
        return ELECTION_OUT_OF_MEMORY;
    }
    if ( result == AREA_ERROR ) {                                                           
        return ELECTION_ERROR;
    }
    return ELECTION_SUCCESS;
}

ElectionResult electionRemoveVote(Election election, int area_id, int tribe_id, int num_of_votes){
    if ( election == NULL ) {
        return ELECTION_NULL_ARGUMENT;
    }
    if ( (area_id < 0) || (tribe_id < 0) ) {
        return ELECTION_INVALID_ID;
    }
    if ( num_of_votes <= 0 ) {
        return ELECTION_INVALID_VOTES;
    }
    if ( !areaIsExists(election->area,area_id) ) {
        return ELECTION_AREA_NOT_EXIST;
    }
    if ( !electionIsTribeExists(election,tribe_id) ) {
        return ELECTION_TRIBE_NOT_EXIST;
    }
    int result = areaRemoveVoteFromTribe(election->area,area_id,tribe_id,num_of_votes);
    if ( result == AREA_OUT_OF_MEMORY ) {                                                           
        return ELECTION_OUT_OF_MEMORY;
    }
    if ( result == AREA_ERROR ) {                                                           
        return ELECTION_ERROR;
    }
    return ELECTION_SUCCESS;
}

Map electionComputeAreasToTribesMapping (Election election){
    Map map = NULL;
    map = mapCreate();
    if ( map == NULL ) {
        return NULL;
    }
    int default_tribe_winner = tribeGetSmallestId(election->tribe);
    int result = areaGetWinnerToMap(election->area,default_tribe_winner,map);
    if ( result == AREA_OUT_OF_MEMORY || result == AREA_ERROR ) {  
        mapDestroy(map);   
        return NULL;
    } 
    return map;
}

static bool isValidName(const char* name){
    while ( *name ) {
        if ( ( *name<'a' || *name>'z' ) && ( *name!=' ' )) {
            return false;
        }
        name++;
    }
    return true;
}

// returns true if tribe found //
static bool electionIsTribeExists(Election election, int tribe_id){
    if ( election->tribe == NULL ) {
        return false ;
    } 
    char* tmp = NULL;
    tmp = electionGetTribeName(election,tribe_id);
    if ( tmp != NULL ) {
        free(tmp);
        return true;
    }
    free(tmp);
    return false;
}
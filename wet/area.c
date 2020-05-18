#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "area.h"
#include "mtm_map/map.h"

#define BUFFER_SIZE 256
#define NO_DATA -1
#define NO_VOTES "0"



static Area areaDeletNode(Area head,int target_id);
static Area areaGet(Area area ,int get_id );

typedef struct data_t* Data;
static Data dataAdd(int id , const char* name);
static void dataDestroy(Data data);
static int dataGetId(Data data);

#define MIN_ID(a, b)  ( ((a) < (b)) ? (a) : (b) )
#define INT_TO_STR(str, num , result)  ( (result) = sprintf( (str) , "%d" , (num) ) ) 
#define STR_TO_INT(num , str, result) ( (result) = sscanf(str , "%d" , &num) ) 
/*-------------------------------------------------------------------------------------*/

struct area_t {
    Data area_data;
    Map area_votes; // for each area there is a map that contain the elected tribe as key , and the number of votes as data
    struct area_t* next;
};

Area areaCreat(){
    Area area = malloc(sizeof(*area));
    if ( area == NULL ){
        return NULL;
    }
    area->area_data = NULL;
    area->area_votes = NULL;
    area->next = NULL ;
    return area ;
}

void areaDestroy(Area area){
    if ( area == NULL ) {
        return;
    }
    while ( area != NULL ) {
        Area area_to_delete = area;
        dataDestroy(area_to_delete->area_data);
        mapDestroy(area_to_delete->area_votes);
        area = area->next;
        free(area_to_delete);
    }
}

Area areaAdd (Area area,int new_area_id ,const char* new_area_name){
    Area new_area = areaCreat();
    if ( new_area == NULL ) {
        return NULL;
    }
    new_area->area_data = dataAdd(new_area_id,new_area_name);
    if ( (new_area->area_data) == NULL ) {
        free(new_area);
        return NULL;
    }
    new_area->area_votes = mapCreate();
    if ( new_area->area_votes == NULL ) {
        dataDestroy(area->area_data);
        free(new_area);
        return NULL;
    }
    new_area->next=area;
    return new_area;
}

static Area areaDeletNode(Area head,int target_id){
    Area tmp = head , prev = NULL;
    //if head itself needed to be deleted//
    if ( (head != NULL) && ( dataGetId(head->area_data) == target_id ) ) {
        head = head->next;
        mapDestroy(tmp->area_votes);
        dataDestroy(tmp->area_data);
        free(tmp);
        return head;
    }
    // search for the area to be deleted//
    while( (tmp != NULL) && ( dataGetId(tmp->area_data) != target_id ) ){
        prev=tmp;
        tmp=tmp->next;
    }
    // area not funded //
    if ( tmp == NULL ) {
        return head;
    }
    //unlink the node //
    prev->next = tmp->next;
    mapDestroy(tmp->area_votes);
    dataDestroy(tmp->area_data);
    free(tmp);
    return head;
}

//retune pointer to area , NULL if not found
static Area areaGet(Area area ,int get_id ){
    for ( Area ptr = area ; ptr!=NULL ; ptr = ptr->next ){
        if ( dataGetId(ptr->area_data) ==  get_id ){
            assert(ptr!=NULL );
            return ptr;
        }
    }
    return NULL;
}

bool areaIsExists(Area area ,int get_id ){
    Area tmp = areaGet(area,get_id);  
    if ( tmp == NULL ) {
        return false; //area not found
    }
    return true;
}

AreaResult areaRemoveAllVotesfromTribe(Area area, int tribe_id){
     //looping through the list removing tribe votes from each area//
    for ( Area ptr = area; ptr!=NULL ; ptr = ptr->next){
        
        int result = AREA_SUCCESS;
        char str_tribe_id[BUFFER_SIZE]="";
        INT_TO_STR(str_tribe_id, tribe_id ,result) ;
        if ( result < 0 ) {
            return AREA_ERROR ;
        }
        
      if ( mapContains(ptr->area_votes,str_tribe_id) ) {
          assert( ptr->area_votes != NULL );
          mapRemove(ptr->area_votes,str_tribe_id);
      }
    }
    return AREA_SUCCESS;
}

Area areaRemove(Area area,ConditionFunction should_delete_area){
    if ( area == NULL ){
        return NULL;
    }
     Area ptr = area;
     while( ptr != NULL ){
        if ( should_delete_area(dataGetId(ptr->area_data)) ) {
            Area tmp = ptr->next;
            area = areaDeletNode(area,dataGetId(ptr->area_data));
            ptr = tmp;
        }else{
            ptr = ptr->next;
        }
    }
    return area;
}

AreaResult areaAddVoteToTribe(Area area,int area_id,int tripe_id,int num_votes){
    Area area_to_add = areaGet(area,area_id);
    assert(area_to_add!=NULL);

    int result = AREA_SUCCESS;
    char str_tripe_id[BUFFER_SIZE]="", str_num_votes[BUFFER_SIZE]="";;
    INT_TO_STR(str_tripe_id,tripe_id , result) ;
    if ( result < 0) {
        return AREA_ERROR;
    }
    INT_TO_STR(str_num_votes,num_votes , result) ;
    if ( result < 0) {
        return AREA_ERROR;
    }

    if ( !mapContains(area_to_add->area_votes , str_tripe_id) ) {
        if( mapPut(area_to_add->area_votes,str_tripe_id,str_num_votes) == MAP_OUT_OF_MEMORY ){
            return AREA_OUT_OF_MEMORY ;
        }
        return AREA_SUCCESS;
    }

    int new_votes = 0;
    char* str_old_vote = mapGet(area_to_add->area_votes,str_tripe_id);
    STR_TO_INT(new_votes ,str_old_vote , result);
    if ( result == EOF) {
        return AREA_ERROR;
    }
   
    new_votes += num_votes;
    INT_TO_STR(str_num_votes ,new_votes ,result);
    if ( result < 0) {
        return AREA_ERROR;
    }

    if ( mapPut(area_to_add->area_votes, str_tripe_id, str_num_votes) == MAP_OUT_OF_MEMORY ){
        return AREA_OUT_OF_MEMORY;
    }
    return AREA_SUCCESS;
}


AreaResult areaRemoveVoteFromTribe(Area area,int area_id,int tripe_id,int votes_to_remove){
    Area area_to_remove_from = areaGet(area,area_id);
    int result = AREA_SUCCESS;
    char str_tripe_id[BUFFER_SIZE]="";
    INT_TO_STR(str_tripe_id ,tripe_id ,result);
    if ( result < 0) {
        return AREA_ERROR;
    }
    char* str_old_vote = mapGet(area_to_remove_from->area_votes,str_tripe_id);

    if ( !str_old_vote ) { // no votes to this tribe , nothing to remove
        return AREA_SUCCESS ;
    }

    int votes_num = 0;
    STR_TO_INT(votes_num ,str_old_vote ,result);
    if ( result == EOF) {
        return AREA_ERROR;
    }
    if ( votes_num < votes_to_remove ) {
       // votes_num = 0;
       if ( mapPut(area_to_remove_from->area_votes,str_tripe_id,NO_VOTES) == MAP_OUT_OF_MEMORY ) {
           return AREA_OUT_OF_MEMORY ;
       }
       return AREA_SUCCESS ;
    }

    votes_num -= votes_to_remove;
    char new_vote_num[BUFFER_SIZE] = "";
    INT_TO_STR(new_vote_num, votes_num ,result);
    if ( result < 0) {
        return AREA_ERROR;
    }

    if ( mapPut(area_to_remove_from->area_votes, str_tripe_id, new_vote_num) == MAP_OUT_OF_MEMORY) {
        return AREA_OUT_OF_MEMORY ;
    }
    return AREA_SUCCESS ;
}

AreaResult areaChangeTribeName(Area area, int tribe_id ,  const char* new_tribe_name){

    if ( area == NULL ){
        return AREA_SUCCESS; // nothing to change 
    }
    for ( Area ptr = area; ptr!=NULL ; ptr = ptr->next){
        int result = AREA_SUCCESS;
        char str_tribe_id[BUFFER_SIZE]="";
        INT_TO_STR(str_tribe_id, tribe_id ,result);
        if ( result < 0 ) {
            return AREA_ERROR;
        }
        
      if ( mapContains(ptr->area_votes , str_tribe_id) ) {
          assert( ptr->area_votes != NULL );
         if ( mapPut(ptr->area_votes , str_tribe_id , new_tribe_name) == MAP_OUT_OF_MEMORY ) {
             return AREA_OUT_OF_MEMORY;
         }
      }
    }
    return AREA_SUCCESS;
}

AreaResult areaGetWinnerToMap(Area area, int default_tribe_winner, Map map){

    for( Area area_winner = area ; area_winner!=NULL ; area_winner = area_winner->next){
        if ( dataGetId(area_winner->area_data) == NO_DATA ){
            continue;
        }

        char *tmp_max_tribe_votes = mapGet( area_winner->area_votes , mapGetFirst(area_winner->area_votes));
        int max_tribe_num_of_votes = 0 , winner_tribe_int = default_tribe_winner;
        int result1 = AREA_SUCCESS , result2 = AREA_SUCCESS;

        if ( tmp_max_tribe_votes ) {  // if there is votes in this area 
            STR_TO_INT(max_tribe_num_of_votes, tmp_max_tribe_votes ,result1);
            STR_TO_INT(winner_tribe_int, mapGetFirst(area_winner->area_votes) ,result2);
            if ( result1 == EOF || result2 == EOF ) {
                return AREA_ERROR;
            }
           
            MAP_FOREACH(tribe , area_winner->area_votes){

                char* tmp_num_of_votes_str = mapGet(area_winner->area_votes, tribe);
                int tmp_num_of_votes = 0,tmp_tribe_id = 0;
                STR_TO_INT(tmp_tribe_id, tribe ,result1);
                STR_TO_INT(tmp_num_of_votes, tmp_num_of_votes_str ,result2 );
                 if ( result1 == EOF || result2 == EOF ) {
                     return AREA_ERROR;
                 }

                if ( tmp_num_of_votes > max_tribe_num_of_votes ) {
                    max_tribe_num_of_votes = tmp_num_of_votes;
                    winner_tribe_int = tmp_tribe_id;
                }
                if ( tmp_num_of_votes == max_tribe_num_of_votes ) {
                    winner_tribe_int = MIN_ID(winner_tribe_int,tmp_tribe_id);
                }
            }
        }

        char winner_area[BUFFER_SIZE]=" ",winner_tribe[BUFFER_SIZE]=" ";
        INT_TO_STR(winner_area, dataGetId(area_winner->area_data) ,result1);
        INT_TO_STR(winner_tribe ,winner_tribe_int ,result2);
        if ( result1 < 0 || result2 < 0 ) {
            return AREA_ERROR;
        }
        // now we have the tribe winner  for area //
        if (mapPut(map,winner_area,winner_tribe)==MAP_OUT_OF_MEMORY){
            return AREA_OUT_OF_MEMORY;
        }  
    }
    return AREA_SUCCESS;
}

//==================================================================================================

struct data_t {
    int area_id;
    char* area_name;
};

Data dataAdd(int id , const char* name){
    Data data = malloc(sizeof(*data));
    if ( data == NULL ) {
        return NULL;
    }
    char *copy = malloc(strlen(name)+1);
    if ( copy == NULL ) {
        free(data);
        return NULL;
    }
    strcpy(copy, name);
    data->area_name = copy;
    data->area_id = id;
    return data;
}

void dataDestroy(Data data){
    if ( data == NULL ){
        return ;
    }
    free(data->area_name);
    free(data);
}

int dataGetId(Data data){
    if ( data == NULL ){
        return NO_DATA;
    }
    return data->area_id;
}

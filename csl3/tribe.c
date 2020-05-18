#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "tribe.h"


#define BUFFER_SIZE 256

#define INT_TO_STR(str, num , result)  ( (result) = sprintf( (str) , "%d" , (num) ) ) 
#define STR_TO_INT(num , str) ( sscanf(str , "%d" , &num) ) 

/*-------------------------------------------------------------------------------------*/
struct tribe_t { 
    Map tribe_map;
    // לעתיד אם נרצה אפשר להוסיף עוד שדות כמו לאיזב גןש הם שייכים (ימין שמאל ) ועוד פונקציות בהתאם 
};

Tribe tribeCreat(){
   Tribe new_tribe = malloc(sizeof(*new_tribe));
    if ( new_tribe == NULL ) { 
        return NULL;
    }
    new_tribe->tribe_map = mapCreate();
    if( new_tribe->tribe_map == NULL){
        free(new_tribe);
        return NULL;
    }
    return new_tribe;
}

void tribeDestroy(Tribe tribe){
    if( tribe == NULL ){
        return;
    }
    mapDestroy( tribe->tribe_map );
    free(tribe);
}

TribeResult tribeAdd (Tribe tribe,int new_tribe_id , const char* tripe_name){

   int result = TRIBE_SUCCESS;
   char new_tribe[BUFFER_SIZE]=" ";
   INT_TO_STR(new_tribe ,new_tribe_id ,result);
   if ( result < 0 ) {
       return TRIBE_ERROR;
   }
   result = mapPut(tribe->tribe_map ,new_tribe,tripe_name);
   if ( result == MAP_OUT_OF_MEMORY ) {
        return TRIBE_OUT_OF_MEMORY;
   }
   return TRIBE_SUCCESS;
} 

TribeResult tribeDelete(Tribe head,int id){
    int result = TRIBE_SUCCESS;
    char tribe_id[BUFFER_SIZE]="";
    INT_TO_STR(tribe_id,id ,result);
    if ( result < 0 ) {
       return TRIBE_ERROR;
    }
    mapRemove(head->tribe_map,tribe_id);
    return TRIBE_SUCCESS;
}

char* tribeGetName(Tribe tribe,int get_id ){
    int result = TRIBE_SUCCESS;
    char* tmp=NULL;
    char str_get_id[BUFFER_SIZE] = "";
    INT_TO_STR(str_get_id ,get_id ,result);
    if ( result < 0 ) {
       return NULL;
    }
    tmp = mapGet(tribe->tribe_map , str_get_id);
    
    if ( tmp == NULL) {
        return NULL;
    }
    char* target_name = malloc( strlen(tmp)+1);
    if ( target_name == NULL ){
        return NULL;
    }
    return strcpy(target_name , tmp);;
}

TribeResult tribeChangeName(Tribe tribe ,int tribe_id,const char* name){
    assert(tribe && name && tribe_id>=0);

    int result = TRIBE_SUCCESS;
    char new_tripe_id[BUFFER_SIZE]="";
    INT_TO_STR(new_tripe_id ,tribe_id ,result);
    if ( result < 0 ) {
       return TRIBE_ERROR;
    }
    result = mapPut(tribe->tribe_map,new_tripe_id,name);
    if (result == MAP_OUT_OF_MEMORY) {
        return TRIBE_OUT_OF_MEMORY;
    }
    return TRIBE_SUCCESS;
}

int tribeGetSmallestId(Tribe tribe){
    
    int min_tmp=0,tmp_tribe_id=0;
    STR_TO_INT(min_tmp ,mapGetFirst(tribe->tribe_map));

    MAP_FOREACH(tmp_tribe , tribe->tribe_map ){
        STR_TO_INT(tmp_tribe_id ,tmp_tribe);

        if ( min_tmp > tmp_tribe_id ){
            min_tmp = tmp_tribe_id ;
        }
    }
    return min_tmp;
}

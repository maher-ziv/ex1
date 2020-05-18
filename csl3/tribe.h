
#ifndef TRIBE_H
#define TRIBE_H
#include "mtm_map/map.h"
/**
* Tribe Container
*
* Implements a Tribe container type. //  TODO vote take as inpus int and sting 
*
* The following functions are available:
*   tribeCreat	    	 - Creates a new empty tribes map
*   tribeDestroy    	 - Deletes an existing tribes map and frees all resources
*   tribeAdd	    	 - add a new tribe 
*   tribeGetName 	     - returns tribe name which matches the given id .
*   tribeChangeName 	 - Gives a specific tribe a new name.
*   tribeDelete  	    - remove specific tribe from the map.			
*   tribeGetSmallestId	- returns the smallest tribe id .

*/

/** Type for defining the tribe */
typedef struct tribe_t *Tribe;

/** Type used for returning error codes from tribe functions */
typedef enum tribeResult_t {
    TRIBE_OUT_OF_MEMORY,
    TRIBE_NULL_ARGUMENT,
    TRIBE_SUCCESS,
    TRIBE_INVALID_ID,
    TRIBE_TRIBE_ALREADY_EXIST,
    TRIBE_INVALID_NAME,
    TRIBE_ERROR
} TribeResult;

/**
* tribeCreat: Allocates a new empty tribe map.
*
* @return
* 	NULL - if allocations failed.
* 	A new Tribe map in case of success.
*/
Tribe tribeCreat();
/**
* tribeDestroy: Deallocates an existing tribe map. Clears all elements.
*
* @param tribe - Target tribe to be deallocated. If tribe is NULL nothing will be
* 	           	done
*/
void tribeDestroy(Tribe tribe);

/**
* tribeAdd: add a new tribe.
* 
* @param tribe - The tribe for which to assign the new tribe.
* @param new_tripe_id - The tribe id element which need to be assigned.
* @param tripe_name - The new tribe name  to associate with the given id.
*                     A copy of the tribe name will be inserted.
* @return
* 	TRIBE_OUT_OF_MEMORY if an allocation failed 
* 	TRIBE_SUCCESS the paired elements had been inserted successfully
*/
TribeResult tribeAdd (Tribe tribe,int new_tripe_id ,const char* tripe_name);

/**
*	tribeGetName: Returns a copy of the tribe name associated with a specific tribe id 
*                  in the tribe map.
*
* @param tribe - The tribe map for which to get the tribe name element from.
* @param get_id - The tribe id element which need to be found and who's name
*                   we want to get.
* @return
*  NULL if a NULL pointer was sent or if the tribe does not contain the requested id .
* 	         copy of the tribe name associated with a the tribe id  otherwise.
*/
char* tribeGetName(Tribe tribe,int get_id );

/**
* tribeChangeName: give a exists tribe new name.
* 
* @param tribe - The tribe for which to assign the new tribe name .
* @param new_tripe_id - The tribe id element which need to be modefied.
* @param tripe_name - The new tribe name  to associate with the given id.
*                     A copy of the tribe name will be inserted.
* @return
* 	TRIBE_OUT_OF_MEMORY if an allocation failed 
* 	TRIBE_SUCCESS the paired elements had been inserted successfully
*/
TribeResult tribeChangeName(Tribe tribe ,int tribe_id,const char* name);

/**
* 	tribeDelete: Removes a pair of id and name  elements from the tribe map. The elements
*   are found using the Map ADT. Once found,
*   the elements are removed and deallocated.
*
* @param tribe -
* 	The tribe map to remove the elements from.
* @param tribe_id
* 	The tribe id element to find and remove from the tribe map. The tribe name element will be freed.
* @return
* 	TRIBE_SUCCESS the paired elements had been removed successfully
*/
TribeResult tribeDelete(Tribe tribe,int tribe_id);

/**
* tribeGetSmallestId: returns the smallest tribe id.
* 
* @param tribe - The tribe for which to get the tribe id .
* @return
*   	the smallest tribe id.
*/
int tribeGetSmallestId(Tribe tribe);


#endif

#ifndef AREA_H
#define AREA_H

#include <stdbool.h>
#include <string.h>
#include "mtm_map/map.h"

/**
* Area Container
*
* Implements a Area container type.
* the Area container store the data of the area , id (int) and name (char*) 
* also for each area can store inforamtion of votes //TODO

* The following functions are available:
*   areaCreat - Creates a new empty area list
*   areaDestroy - Deletes an existing area list and frees all resources
*   areaAdd	- add a new area to the list 
*   areaRemoveAllVotesfromTripe - removes all votest from specific tribe 
*   areaRemove - removes specific area from the list with all votes from this area
*   areaAddVoteToTribe - adds votes to tribe 			
*   areaRemoveVoteFromTribe	- removes votes from tribe .
*   areaIsExists -  return true if area exists in the list , else return false
*   areaGetWinnerToMap - return map of areas as keys and most voted tribe as data.
*/

/** Type for defining the Area */
typedef struct area_t *Area;

/** Type used for returning error codes from map functions */
typedef enum areaResult_t {
    AREA_OUT_OF_MEMORY,
    AREA_NULL_ARGUMENT,
    AREA_SUCCESS,
    AREA_ERROR
} AreaResult;

typedef bool (*ConditionFunction) (int);

/**
* areaCreat - Allocates a new empty area list
*
* @return
* 	NULL - if allocations failed.
* 	A new area list in case of success.
*/
Area areaCreat();

/**
* areaDestroy: Deallocates an existing area list.
*
* @param area - Target area list to be deallocated. If area is NULL nothing will be
* 	           	done.
*/
void areaDestroy(Area area);

/**
* areaAdd: add a new area to the list .
* 
* @param area - The area list for which to assign the new area.
* @param new_area_id - The area id element which need to be assigned.
* @param area_name - The new area name  to associate with the given id.
*                     A copy of the area name will be inserted.
* @return
* 	AREA_OUT_OF_MEMORY if an allocation failed 
* 	AREA_SUCCESS the paired elements had been inserted successfully
*/
Area areaAdd (Area area,int new_area_id ,const char* area_name);

/**
* areaRemoveAllVotesfromTribe: removes all votest from specific tribe .
* 
* @param area - The area list for which to remove all votes .
* @param tribe_id - The tribe id element which need to be removed.

* @return
* 	AREA_ERROR if error occurred 
* 	AREA_SUCCESS the tribe votes removed successfully
*/
AreaResult areaRemoveAllVotesfromTribe(Area area, int tribe_id);

/**
* areaRemove: removes a specific area from the list with all votes from this area.
* 
* @param area - The area list for which to remove an area .
* @param should_delete_area - function to tell how to remove 

* @return
* 	updated area
*/
Area areaRemove(Area area,ConditionFunction should_delete_area);

/**
* areaAddVoteToTribe - adds votes to tribe.
* 
* @param area - The area list for which to add votes .
* @param area_id - The area id element which to give votes.
* @param tripe_id - the elected tribe
* @param num_votes - number of votes to add.
* @return
*   AREA_OUT_OF_MEMORY if an allocation failed 
* 	AREA_ERROR if other error occurred 
* 	AREA_SUCCESS the tribe votes removed successfully
*/
AreaResult areaAddVoteToTribe(Area area,int area_id,int tripe_id,int num_votes);

/**
*  areaRemoveVoteFromTribe	- removes votes from tribe .
* 
* @param area - The area list for which to remove  votes .
* @param area_id - The area id element which to remove votes. // להחסיר
* @param tripe_id - the elected tribe
* @param num_votes - number of votes to add te remove.
* @return
*   AREA_OUT_OF_MEMORY if an allocation failed 
* 	AREA_ERROR if other error occurred 
* 	AREA_SUCCESS the tribe votes removed successfully
*/
AreaResult areaRemoveVoteFromTribe(Area area,int area_id,int tripe_id,int votes_to_remove);

/**
*  areaIsExists -  return true if area exists in the list , else return false .
* 
* @param area - The area list for which to check .
* @param get_id - The area id to look for
* @return
*   true if area exists in the list , else return false . 
*/
bool areaIsExists(Area area ,int get_id );
AreaResult areaChangeTribeName(Area area, int tribe_id ,  const char* new_tribe_name);

/**
*  areaGetWinnerToMap - return map of areas as keys and most voted tribe as data.
* 
* @param area - The area list for which to map the elected tribes  .
* @param default_tribe_winner - defult elected tribe in case there is no votes 
* @param map - the map to but the elected tribe for each area
* @return
*   AREA_OUT_OF_MEMORY if an allocation failed 
* 	AREA_ERROR if other error occurred 
* 	AREA_SUCCESS the tribe votes removed successfully
*/
AreaResult areaGetWinnerToMap(Area area, int default_tribe_winner, Map map);




#endif //AREA_H//

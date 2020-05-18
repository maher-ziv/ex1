#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "map.h"


typedef struct elements_node {
    char* key;
    char* data;
    struct elements_node* next;
} *Node;


struct Map_t {
    Node head;
    Node iterator;
    int size;
};


Map mapCreate() {
    Map map = malloc(sizeof(*map));
    if (map == NULL) {
        return NULL;
    }
    map->head = NULL;
    map->iterator = NULL;
    map->size = 0;
    return map;
}

void mapDestroy(Map map) {
    if (map == NULL) {
        return;
    }
    Node to_delete = map->head;
    while (to_delete != NULL) {
        Node tmp = to_delete->next;
        free(to_delete->key);
        free(to_delete->data);
        free(to_delete);
        to_delete = tmp;
    }
    free(map);
}

static Node makeNewElement(const char* key, const char* data) {
    if  (key == NULL || data == NULL) {
        return NULL;
    }
    Node newElement = malloc(sizeof(*newElement));
    if (newElement == NULL) {
        return NULL;
    }
    newElement->key = malloc((strlen(key)+1)*sizeof(char));
    newElement->data = malloc((strlen(data)+1)*sizeof(char));
    if (newElement->key == NULL || newElement->data == NULL) {
        free(newElement->key);
        free(newElement->data);
        free(newElement);
        return NULL;
    }
    strcpy(newElement->key,key);
    strcpy(newElement->data,data);
    newElement->next=NULL;
    return newElement;
}


Map mapCopy(Map map) {
    if (map == NULL) {
        return NULL;
    }
    Map map_new = (mapCreate());
    if (map_new == NULL) {
        return NULL;
    }
    Node ptr = map->head;
    while (map->size!=(map_new->size)) {   //copying the elements from the original map
        if (mapPut(map_new , ptr->key , ptr->data) == MAP_OUT_OF_MEMORY) {
            mapDestroy(map_new);
            return NULL;
        }
        ptr = ptr->next;
    }
    return map_new;
}

int mapGetSize(Map map) {
    if (map == NULL) {
        return -1;
    }
    return map->size;
}

//searching for an element that includes the given key
static Node findElement(Map map, const char* key) { 
    assert(map!=NULL && key!=NULL);
    Node element = map->head;
    while (element!=NULL && strcmp(element->key,key)) { //checking if the strings are identical
        element = element->next;
        }
        return element; //NULL if the search ended with nothing or an element that includes the given key
}

bool mapContains(Map map, const char* key) {
    if (map == NULL || key == NULL || map->size == 0) {
        return false;
    }
    return (findElement(map,key));
}



MapResult mapPut(Map map, const char* key, const char* data) {
    if (map == NULL || key == NULL || data == NULL) {
        return MAP_NULL_ARGUMENT;
    }
    Node element = findElement(map,key);  //if finds the element the data will be replaced, else new element
    if (element != NULL) {
        char* newData = realloc(element->data, sizeof(char)*(strlen(data)+1));
        if (newData == NULL) {
            return MAP_OUT_OF_MEMORY;
        }
        strcpy(newData,data);
        element->data = newData;
    }
    else {
        Node newElement = makeNewElement(key,data);
        if(newElement == NULL) {
            return MAP_OUT_OF_MEMORY; 
        }
        newElement->next = map->head;
        map->head = newElement;
        map->size++;
    }
    return MAP_SUCCESS;
    }
    
char* mapGet(Map map, const char* key) {
    if (map == NULL || key == NULL) {
        return NULL;
    }
    Node element = findElement(map,key);
    if (element == NULL) {
        return NULL;
    }
    return element->data;
}

MapResult mapRemove(Map map, const char* key) {
    if (map == NULL || key == NULL) {
        return MAP_NULL_ARGUMENT;
    }
    if (!mapContains(map,key)) {
        return MAP_ITEM_DOES_NOT_EXIST;
    }
    Node to_delete = NULL;
    if (!strcmp(map->head->key,key)) {
        to_delete = map->head;
        map->head = map->head->next;   
    }
    for (Node tmp = map->head ; to_delete == NULL ; tmp = tmp->next) {
        if (!strcmp(tmp->next->key,key)) {
            to_delete = tmp->next;
            tmp->next = tmp->next->next;
            continue;
        }
    }
    free(to_delete->key);
    free(to_delete->data);
    free(to_delete);
    map->size--;
    return MAP_SUCCESS; 
}

char* mapGetFirst(Map map) {
    if (map == NULL || map->size == 0){
        return NULL;
    }
    map->iterator = map->head;
    return map->iterator->key;
}

char* mapGetNext(Map map){
    if (map == NULL || map->iterator == NULL || map->iterator->next ==NULL ) {
        return NULL;
    }
    map->iterator = map->iterator->next;
    return map->iterator->key;
}

MapResult mapClear(Map map) {
    if (map == NULL) {
        return MAP_NULL_ARGUMENT;
    }
    while (map->size) {
        Node to_delete = map->head;
        free (to_delete->key);
        free (to_delete->data);
        map->head = to_delete->next;
        free(to_delete);
        map->size--;
    }
    return MAP_SUCCESS;
        
}

//
// Created by bardv on 3/19/2020.
//


#include "mapTests.h"
#include "../test_utilities.h"
#include "../mtm_map/map.h"



bool testMapCreate() {
    Map map = mapCreate();
    ASSERT_TEST(map != NULL);
    mapDestroy(map);
    return true;
}

bool testMapDestroy() {
    Map map = mapCreate();
    ASSERT_TEST(map != NULL);
    char *arr[] = {"paris", "madrid", "london", "jerusalem", "rome"};
    mapPut(map, "0", arr[0]);
    mapPut(map, "1", arr[1]);
    mapPut(map, "2", arr[2]);
    mapPut(map, "3", arr[3]);
    mapPut(map, "4", arr[4]);
    mapDestroy(map);
    return true;
}

bool testMapCopy() {
    Map map = mapCreate();

    Map map_copy1 = mapCopy(NULL);
    ASSERT_TEST(map_copy1 == NULL);
    mapDestroy(map_copy1);


    char *arr[] = {"paris", "madrid", "london", "jerusalem", "rome"};
    mapPut(map, "0", arr[0]);
    mapPut(map, "1", arr[1]);
    mapPut(map, "2", arr[2]);
    mapPut(map, "3", arr[3]);
    mapPut(map, "4", arr[4]);


    Map map_copy = mapCopy(map);
    ASSERT_TEST(map_copy != NULL);
    ASSERT_TEST(mapGetSize(map_copy) == 5);
    char *zero = "0";
    ASSERT_TEST(mapContains(map_copy, zero));
    char *a = "3";
    char *map_value_of_three = mapGet(map_copy, a);
    ASSERT_TEST(strcmp(map_value_of_three, "jerusalem") == 0);


    a = "2";
    mapRemove(map_copy, a);
    ASSERT_TEST(mapGetSize(map_copy) == 4);
    ASSERT_TEST(mapGetSize(map) == 5);
    ASSERT_TEST(!mapContains(map_copy, a));
    ASSERT_TEST(mapContains(map, a));

    mapDestroy(map);
    mapDestroy(map_copy);
    return true;
}


bool testMapGetSize() {
    Map map = mapCreate();
    char *arr[] = {"paris", "madrid", "london", "jerusalem", "rome"};
    ASSERT_TEST(map != NULL);
    ASSERT_TEST(mapGetSize(map) == 0);
    ASSERT_TEST(mapGetSize(NULL) == -1);
    mapPut(map, "0", arr[0]);
    mapPut(map, "1", arr[1]);
    mapPut(map, "2", arr[2]);
    mapPut(map, "3", arr[3]);
    mapPut(map, "4", arr[4]);
    ASSERT_TEST(mapGetSize(map) == 5);
    char *a = "2";
    mapPut(map,a,arr[2]);
    ASSERT_TEST(mapGetSize(map) == 5);
    mapRemove(map, a);
    ASSERT_TEST(mapGetSize(map) == 4);
    mapDestroy(map);
    return true;
}

bool testMapContains() {
    Map map = mapCreate();
    char *arr[] = {"paris", "madrid", "london", "jerusalem", "rome"};
    mapPut(map, "0", arr[0]);
    mapPut(map, "1", arr[1]);
    mapPut(map, "2", arr[2]);
    mapPut(map, "3", arr[3]);
    mapPut(map, "4", arr[4]);
    char *a = "1";

    ASSERT_TEST(mapContains(NULL, a) == false);
    ASSERT_TEST(mapContains(map, NULL) == false);

    ASSERT_TEST(mapContains(map, a) == true);
    mapRemove(map, a);
    ASSERT_TEST(mapContains(map, a) == false);
    mapDestroy(map);
    return true;
}


bool testMapPut() {
    Map map = mapCreate();
    char *arr[] = {"paris", "madrid", "london", "jerusalem", "rome", "borma"};
    char *a = "1";

    ASSERT_TEST(mapPut(map, NULL, arr[1]) == MAP_NULL_ARGUMENT);
    ASSERT_TEST(mapPut(map, a, NULL) == MAP_NULL_ARGUMENT);
    ASSERT_TEST(mapPut(NULL, a, arr[1]) == MAP_NULL_ARGUMENT);

    ASSERT_TEST(mapPut(map, "0", arr[0]) == MAP_SUCCESS);
    ASSERT_TEST(mapPut(map, "1", arr[1]) == MAP_SUCCESS);
    ASSERT_TEST(mapPut(map, "2", arr[2]) == MAP_SUCCESS);
    ASSERT_TEST(mapPut(map, "3", arr[3]) == MAP_SUCCESS);
    ASSERT_TEST(mapPut(map, "4", arr[4]) == MAP_SUCCESS);
    ASSERT_TEST(mapGetSize(map) == 5);

    a = "2";
    char* current_value = mapGet(map, a);
    ASSERT_TEST(strcmp(current_value, "london") == 0);
    ASSERT_TEST(mapPut(map, a, arr[5]) == MAP_SUCCESS);
    current_value = mapGet(map, a);
    ASSERT_TEST(strcmp(current_value, "borma") == 0);
    ASSERT_TEST(mapContains(map, a) == true);
    ASSERT_TEST(mapGetSize(map) == 5);

    mapDestroy(map);
    return true;
}

bool testMapGet() {
    Map map = mapCreate();
    char *arr[] = {"paris", "madrid", "london", "jerusalem", "rome"};
    char *a = "1";

    ASSERT_TEST(mapGet(NULL, a) == NULL);
    ASSERT_TEST(mapGet(map, NULL) == NULL);

    mapPut(map, "0", arr[0]);
    char* current_get = mapGet(map, "0");
    ASSERT_TEST(strcmp(current_get, arr[0]) == 0);
    mapPut(map, "1", arr[1]);
    current_get = mapGet(map, "1");
    ASSERT_TEST(strcmp(current_get, arr[1]) == 0);
    mapPut(map, "2", arr[2]);
    current_get = mapGet(map, "2");
    ASSERT_TEST(strcmp(current_get, arr[2]) == 0);
    mapPut(map, "3", arr[3]);
    current_get = mapGet(map, "3");
    ASSERT_TEST(strcmp(current_get, arr[3]) == 0);
    mapPut(map, "4", arr[4]);
    current_get = mapGet(map, "4");
    ASSERT_TEST(strcmp(current_get, arr[4]) == 0);

    a = "6";
    ASSERT_TEST(mapGet(map, a) == NULL);

    //checking its return real value and not copy
    current_get[0] = 'g';
    current_get = mapGet(map, "4");
    ASSERT_TEST(strcmp(current_get, arr[4]) != 0);
    ASSERT_TEST(strcmp(current_get, "gome") == 0);


    mapDestroy(map);
    return true;
}

bool testMapRemove() {
    Map map = mapCreate();
    char *arr[] = {"paris", "madrid", "london", "jerusalem", "rome"};
    char *a = "1";
    ASSERT_TEST(mapRemove(NULL, a) == MAP_NULL_ARGUMENT);
    ASSERT_TEST(mapRemove(map, NULL) == MAP_NULL_ARGUMENT);

    mapPut(map, "0", arr[0]);
    mapPut(map, "1", arr[1]);
    mapPut(map, "2", arr[2]);
    mapPut(map, "3", arr[3]);
    mapPut(map, "4", arr[4]);

    ASSERT_TEST(mapGetSize(map) == 5);
    ASSERT_TEST(mapRemove(map, a) == MAP_SUCCESS);
    ASSERT_TEST(mapGetSize(map) == 4);
    ASSERT_TEST(mapRemove(map, a) == MAP_ITEM_DOES_NOT_EXIST);
    ASSERT_TEST(mapGetSize(map) == 4);
    mapDestroy(map);
    return true;
}

bool testMapGetFirst() {
    Map map = mapCreate();
    char *arr[] = {"paris", "madrid", "london", "jerusalem", "rome"};

    ASSERT_TEST(mapGetFirst(NULL) == NULL);
    ASSERT_TEST(mapGetFirst(map) == NULL);

    mapPut(map, "0", arr[0]);
    mapPut(map, "1", arr[1]);
    mapPut(map, "2", arr[2]);
    mapPut(map, "3", arr[3]);
    mapPut(map, "4", arr[4]);

    char *a = mapGetFirst(map);
    ASSERT_TEST(mapContains(map, a) == true);

    mapDestroy(map);
    return true;
}

bool testMapGetNext() {
    Map map = mapCreate();
    char *arr[] = {"paris", "madrid", "london", "jerusalem", "rome"};
    ASSERT_TEST(mapGetFirst(map) == NULL);
    ASSERT_TEST(mapGetNext(map) == NULL);


    mapPut(map, "0", arr[0]);
    ASSERT_TEST(strcmp(mapGetFirst(map), "0") == 0);
    mapPut(map, "1", arr[1]);
    ASSERT_TEST(mapGetFirst(map) != NULL);
    mapPut(map, "2", arr[2]);
    ASSERT_TEST(mapGetFirst(map) != NULL);
    mapPut(map, "3", arr[3]);
    ASSERT_TEST(mapGetFirst(map) != NULL);
    mapPut(map, "4", arr[4]);
    ASSERT_TEST(mapGetFirst(map) != NULL);

    char *second_key = mapGetNext(map);
    char *third_key = mapGetNext(map);
    char *fourth_key = mapGetNext(map);
    char *fifth_key = mapGetNext(map);
    char *sixth_key = mapGetNext(map);


    ASSERT_TEST(second_key != third_key);
    ASSERT_TEST(second_key != fourth_key);
    ASSERT_TEST(second_key != fifth_key);
    ASSERT_TEST(second_key != sixth_key);

    ASSERT_TEST(third_key != fourth_key);
    ASSERT_TEST(third_key != fifth_key);
    ASSERT_TEST(third_key != sixth_key);

    ASSERT_TEST(fourth_key != fifth_key);
    ASSERT_TEST(fourth_key != sixth_key);

    ASSERT_TEST(fifth_key != sixth_key);

    ASSERT_TEST(sixth_key == NULL);

    mapDestroy(map);
    return true;
}

bool testMapClear() {
    Map map = mapCreate();
    char *arr[] = {"paris", "madrid", "london", "jerusalem", "rome"};
    char *a = "1";
    ASSERT_TEST(mapClear(NULL) == MAP_NULL_ARGUMENT);

    mapPut(map, "0", arr[0]);
    mapPut(map, "1", arr[1]);
    mapPut(map, "2", arr[2]);
    mapPut(map, "3", arr[3]);
    mapPut(map, "4", arr[4]);

    ASSERT_TEST(mapGetSize(map) == 5);
    ASSERT_TEST(mapClear(map) == MAP_SUCCESS);
    ASSERT_TEST(map != NULL);
    ASSERT_TEST(mapGetSize(map) == 0);
    ASSERT_TEST(mapGetFirst(map) == NULL);
    ASSERT_TEST(mapRemove(map, a) == MAP_ITEM_DOES_NOT_EXIST);
    mapDestroy(map);
    return true;
}

#include "map_test.h"
#include "map.h"
#include "stdio.h"

void printMap(map *map)
{
    printf("Info on map of capacity %d.\n", map->size);
    int i, j;
    for (i = 0; i < map->size; i++)
    {
        struct _map_bucket buck = map->buckets[i];
        if (buck.key == NULL)
        {
            if (buck.next != NULL)
            {
                printf("Error on hash %d. Was null, but had next.\n", i);
            }
        }
        else
        {
            j = 1;
            printf("%s", buck.key);
            while (buck.next != NULL)
            {
                buck = *(buck.next);
                printf(" %s ", buck.key);
            }
            printf(" | Hash %d has %d items.\n", i, j);
        }
    }
}

void testMap(CuTest *tc)
{
    map *nmap = NewMap(100);
    int val = 55;
    Map_Set(nmap, "age", &val);
    Map_Set(nmap, "age", &val);
    Map_Set(nmap, "age2", &val);
    map_result result = Map_Get(nmap, "age");
    CuAssertIntEquals_Msg(tc, "Found should be 1", 1, result.found);
    CuAssertIntEquals_Msg(tc, "Returned value should be correct", val, *(int *)(result.data));
    int override = 65;
    Map_Set(nmap, "age", &override);
    result = Map_Get(nmap, "age");
    CuAssertIntEquals_Msg(tc, "Should override values.", override, *(int *)(result.data));
    result = Map_Delete(nmap, "age", 0);
    CuAssertIntEquals_Msg(tc, "Should have found", 1, result.found);
    CuAssertIntEquals_Msg(tc, "Should have data", override, *(int *)(result.data));
    result = Map_Get(nmap, "age");
    CuAssertIntEquals_Msg(tc, "Should have not found", 0, result.found);
}

CuSuite *mapTestGetSuite()
{
    CuSuite *suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, testMap);
    return suite;
}
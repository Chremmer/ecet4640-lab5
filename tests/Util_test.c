#include "Util_test.h"

void testRandoms(CuTest *tc)
{
    int i, random;
    int min = 10;
    int max = 15;
    for (i = 0; i < 1000; i++)
    {
        random = RandomInteger(min, max);
        CuAssertIntEquals_Msg(tc, "random is between min and max", 1, random <= max && random >= min);
    }
    float minf = 2.5;
    float maxf = 4.0;
    float randomf;
    for (i = 0; i < 1000; i++)
    {
        randomf = RandomFloat(minf, maxf);
        CuAssertIntEquals_Msg(tc, "random float is between min and max", 1, randomf <= max && randomf >= minf);
    }
}

CuSuite *utilTestGetSuite()
{
    CuSuite *suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, testRandoms);
    return suite;
}

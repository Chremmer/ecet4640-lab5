#include <stdio.h>

#include "map_test.h"
#include "Util_test.h"

void RunAllTests(void)
{
    CuString *output = CuStringNew();
    CuSuite *suite = CuSuiteNew();
    
    CuSuiteAddSuite(suite, mapTestGetSuite());
    CuSuiteAddSuite(suite, utilTestGetSuite());

    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);
    printf("%s\n", output->buffer);
}

int main() {
    RunAllTests();
    return 0;
}
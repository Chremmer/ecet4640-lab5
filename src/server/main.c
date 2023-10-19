#include <stdio.h>
#include "Util.h"
#include "Process.h"
#include <string.h>


/**
 * @brief Accepts command line arguments. 
 * @note no argument - run in terminal
 * @note help - Print help
 * @note headless - Run headlessly using nohup
 * @note stop - Stops running server with a signal.
 * @note invalid option - Print help
 */
int main(int argc, char **argv) {

    if(argc <= 1 )
    {
        RunCommand();
    } 
    else if (strcmp(argv[1], "headless") == 0)
    {
        RunHeadless(argv[0]);
    } 
    else if (strcmp(argv[1], "help") == 0)
    {
        // HelpCommand()
    } 
    else if (strcmp(argv[1], "stop") == 0) 
    {
        StopCommand();
    } 
    else if (argc > 2) 
    {
        // Print command not found
        // HelpCommand()
    } 
    else 
    {
        RunCommand();
    }
    return 0;
}
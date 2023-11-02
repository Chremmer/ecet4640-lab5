#include <stdio.h>
#include "Util.h"
#include "Process.h"
#include <string.h>


/**
 * 
 * \mainpage 
 * 
 * # Intro
 * 
 * This program starts a Ipv4 server that listens on a port for incomming connections. For each new connection, it starts a thread. Connecting users send strings to execute various actions on the server.
 * 
 * # Contributions
 * 
 * - On 9/29, Christian made the initial repository.
 * - On 9/29, Karl made the Makefile, copied the example client and server, created the Build, Data, File, Process, Util, and map modules.
 * - On 9/30, Karl added logic for reading the registered users file into a map and initializing/binding the server socket; created the Server and Connection modules.
 * - On 10/1, Paul and Karl did work on reading the server-settings.txt file, and creating a new thread for a client connection.
 * - On 10/2, Christian added a Log module and added some functionality to support various log levels. 
 * - On 10/5, Paul, Karl, and Christian began adding different command line arguments the server could take.
 * - On 10/5, Karl and Paul fixed some bugs related to the multithreading.
 * - On 10/5, Paul added the myinfo and register commands and fixed typos.
 * - On 10/8, Christian added features for initializing the logger and fixed a bug related to disconnects.
 * - On 10/12, Karl, Paul and Christian changed the message format to the current format, set commands to be lowercase, and improved the messages sent to users.
 * - On 10/16, Christian added features to the _rand_age function. 
 * - On 10/18, Karl fixed a segfault bug that occurred when users entered an invalid ID.
 * - On 10/19, Christian added the call to updating the registered file as users register
 * - On 10/19, Christian, Karl, and Paul added more command line arguments to the server.
 * - On 10/21, Christian implemented the advertisement feature and removed debug prints. 
 * 
 * # Overview
 * 
 *  When the program runs, it reads from server-settings.txt to determine how it should be configured. As users register, they are added to a registered.txt file. The Users data structure has a dirty flag and a mutex that indicates when user data has been changed. A separate cleanup thread checks this dirty flag every few seconds to see if the registered.txt file should be updated. 
 * 
 * Only one server process should be running at a given time. To that end, a running server creates a lockfile in the /tmp folder and deletes the lockfile when it is done. New servers will not be started if a lockfile exists, but the running server can be stopped by passing the command line argument 'stop' to the binary. There are other command line arguments available, as detailed below.
 * 
 * | Argument | Description | Calls |
 * |----------|--------|-------|
 * | none     | Defaults to RunCommand; runs server attached to terminal | RunCommand() |
 * | headless    | Runs the server with .nohup, as a background process. | RunHeadless() |
 *  | stop     | Stops an existing server process if it is running. | StopCommand() |
 **
 * \author Karl Miller
 * \author Paul Shriner
 * \author Christian Messmer
 * 
*/

/**
 * 
 * \page Compilation
 * 
 * # Compilation Pipelines
 * 
 * There are several compilation pipelines, which are described in more detail in the Makefile comments. 
 * 
 *  The first is for making and running the regular server process. Calling `make` executes this. It uses the files in `src/server` to generate the binary and runs it. This will run the binary after it is built, and the default command will cause it to run in the server. Executing `make server` will make the server binary without running it.  
 * 
 * The second is for making the test binary. This compiles the files in `tests` and the files in `src/server`, but excludes `src/main.c` so that `tests/main_test.c` will be the program entry point instead. The tests use [CuTest](https://cutest.sourceforge.net/). The tests are not documented here in order to not inflate the documentation size any further.

 * 
 * # Compiling and running
 * 
 * 1. Copy the .zip file to the server.
 * 2. Extract the zip file.
 * 3. Enter the unzipped folder.
 * 4. Run `make server`
 * 5. Run `./server` to run the server attached to the shell.
 * 6. Press ctrl+c to exit and close the server.
 * 7. Run `./server headless` to run the server headlessly.
 * 8. Run `./server stop` to stop the headless server.
 * 9. If a better client is not available, you can use the example client to connect.
 * 10. cd into the /example folder
 * 11. run gcc client.c
 * 12. run ./a.out and input '3000' as the port.
 * 
 * # Screenshot of Compilation
 * 
 * ![Compiling on draco1](compilation-001.png)
 * 
 * # Cleaning
 * 
 * `make clean` will clean all .o files and binaries.
*/


/**
 * @brief Accepts command line arguments. 
 * @note no argument - run in terminal
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
    else if (strcmp(argv[1], "stop") == 0) 
    {
        StopCommand();
    } 
    else 
    {
        RunCommand();
    }
    return 0;
}
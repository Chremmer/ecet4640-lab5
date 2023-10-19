#ifndef Process_h
#define Process_h
/***
 * \defgroup Process
 * \brief This module holds functions that realize the primary business logic of the program.
 * \details When command line arguments are parsed by main, functions in this module are called.
 * @{
*/

/***
    Handles an interrupt or quit signal.

    Send server to shutdown mode, resulting in graceful deletion of lockfile.
*/
void SignalHandle(int signo);

/***
 * Performs initializing activities which must occur prior to a server loop starting.
 * 
 * Will print errors if there are problems initializing.
 * 
 * @returns 1 on success, otherwise 0.
*/
int Initialize();

/***
 * Runs the server.
 * @returns 1 on success, otherwise 0.
*/
int RunCommand();

/***
    Finds the process ID of a running server using the lockfile, and calls kill on it, sending a SIGTERM.
    @returns -1 if the file doesn't exist, -2 if no valid process ID existed in the file, 1 if sending the kill signal failed, or 0 on success. 
    
*/
int TerminateExistingServer();

/**
 * @brief Stops the server that is running headlessly and prints the results of running the command
 */
void StopCommand();

/**
    Uses nohup `./{processName} run` to run the process headlessly.
    @param processName The name of the currently running process, by default, 'server'.
*/
void RunHeadless(char *processName);

/**
 * @}
*/

#endif 
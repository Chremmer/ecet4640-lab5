#ifndef Process_h
#define Process_h
/**
 * \defgroup Process
 * \brief This module holds functions that realize the primary business logic of the program.
 * \details When command line arguments are parsed by main, functions in this module are called.
 * @{
*/

/**
 * Performs initializing activities which must occur prior to a server loop starting.
 * 
 * Will print errors if there are problems initializing.
 * 
 * @returns 1 on success, otherwise 0.
*/
int Initialize();

/**
 * Runs the server.
 * @returns 1 on success, otherwise 0.
*/
int RunCommand();

/**
 * @}
*/

#endif 
# ecet4640-lab5

Instructions for this assignment are located [at this link](https://students.calu.edu/calupa/chen/ecet4640/lab/phase2cs.html)

# Protocol

Send to Client:

~Message~ A text message
~Error~ A server-side error, accompanying a connection close?

Send to Server:
userID as a string... 
REGISTER - register your user
HELP - get a list of available commands
EXIT - disconnect from the server
MYINFO - get info about yourself

ADVERTISEMENT - send ascii art //define size of ascii art

Their buffer size should be at least 11k bytes to accomodate Advertisements

Bonus Possibilities:

1. ADVERTISEMENT - send them ascii art of the software development club
2. let them change GPA / age / etc
 SET GPA 2.5
 SET AGE 99
 RANDOM GPA
 RANDOM AGE
3. Messages
 SEND MESSAGE <userid> <msg>
 READ MESSAGES
4. WHO (who is online?) - Unregistered?????


# Some high-priority Todos

- Line 83 ish of Connection.c:
    - write a function to handle ClientState_Accessing
- Line 86 of Connection.c
    - write a function to handle ClientState_Registered

- Line 31 of Connection.c
    - finish writing the update thread (writes registered file if data is dirty)

- Logger improvements
    - seperate server-settings.txt parameters for console log level and logfile log leve
    - printing logs to console in the appropriate color
    - prepending text indicating the log type, eg [ERROR]
    - prepending a date string
    - actually using the logging functionality around the program
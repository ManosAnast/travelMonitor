# include "TMInterface.h"

void nothing();

char * FrontTrack(char * src, char * Next);

char * BackTrack(char * src);

void BreakString(char *** Array, char * str, const char * s, int Num);

/* Takes a file and insert it's contents in hash, also starts the TTY that the program needs.
 *
 * text: Name of file that is needed in order to start the program.
 * 
*/
void Start(char * text, int monitorId, int buffer);

/* Takes a virus list and a country list and starts the TTY
 * that gives us the interface that we need in order to use some commands.
 * 
 * Vlist: Virus list. (See Virus.h)
 * CList: Country list. (See Country.h)
 * 
*/
void TTY(Virus * Vlist, Country * Clist);

/* Takes the virus list and the country list that the program has created and destroys them
 * by calling the appropriate functions.
 * 
 * Vlist: Virus list. (See Virus.h)
 * CList: Country list. (See Country.h)
 * 
*/
void Destroy(Virus * Vlist, Country * CList);

void TTYMonitor(Virus * Vlist, int id, int buffer);
# include "signal.h"

/* It adds to the file path, the file/directory that we want to go.
 *
 * src: File path that we want to change.
 * Next: File or directory that we want to go to.
 *
 * Return:
 *      In case of success it returns the new path.
*/
char * FrontTrack(char * src, char * Next);

/* It mimics the "cp .." command. Takes a path and takes one step back.
 *
 * src: File path that we want to count the entries that it has.
 *
 * Return:
 *      In case of success it returns the new path.
*/
char * BackTrack(char * src);

void BreakString(char *** Array, char * str, const char * s, int Num);

/* Counts the number of entries that each country has.
 *
 * text: File path that we want to change.
 *
 * Return:
 *      If everything goes right, return size.
 *      If something goes wrong, return -1.
*/
int CountSize(char * text);
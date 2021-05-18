# include "General.h"

extern int monitorId;

/* Takes a file and insert it's contents in hash, also starts the TTY that the program needs.
 *
 * text: Name of file that is needed in order to start the program.
 * 
*/
void Start(char * text);

void TTYMonitor(Virus * Vlist, char * text);

void travelRequestMonitor(char * Id, char * VirusName);

void searchVaccinationStatusMonitor(char * Id);

int addVaccinationRecords(Virus * Vlist, char * text);
# include "General.h"

extern int monitorId;

/* Takes a file and insert it's contents in hash, also starts the TTY that the program needs.
 *
 * text: Name of file that is needed in order to start the program.
 * 
*/
void Start(char * text);

/* Takes a virus list and a country list and starts the TTY
 * that gives us the interface that we need in order to use some commands.
 * 
 * Vlist: Virus list. (See Virus.h)
 * text: input directory.
 * 
*/
void TTYMonitor(Virus * Vlist, char * text);

/* Implemention of the travelRequest command, for the monitors.
 * travelRequestMonitor: Takes an id and a virus. Checks at the has table if the id has been inserted and prints the suitable message.
 * 
 * Id: Citizens Id that we want to see hers/his vaccination status. In string form
 * VirusName: Name of the virus that we want to check.
 * 
*/
void travelRequestMonitor(char * Id, char * VirusName);

/* Implemention of the searchVaccinationStatus command, for the monitors.
 * searchVaccinationStatusmonitor: Sends to travelmonitor the informations that the monitor has about the citizen.
 * 
 * Id: Citizen id (string).
 * 
*/
void searchVaccinationStatusMonitor(char * Id);


/* Implemention of the addVaccinationRecords command, for the monitors.
 * addVaccinationRecords: Inserts the new information citizens that the monitor was given.
 * 
 * Vlist: Virus list.
 * text: input dir.
 * 
*/
int addVaccinationRecords(Virus * Vlist, char * text);
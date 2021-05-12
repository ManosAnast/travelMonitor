# include "Commands.h"

void travelRequestMonitor(int monitorId, char * Id, char * VirusName);

void searchVaccinationStatusMonitor(int monitorId, char * Id);

void addVaccinationRecords(Virus * Vlist, char * text);

/* It checks if the 2 files are modified at the same time.
 * 
 * src_fd: file descriptor of the source file.
 * dst_fd: file descriptor of the destination file.
 *
 * Return:
 *      If the files are modified at the same time, it returns the 1.
 *      If the files aren't modified at the same time, it returns the 0.
*/
int SameDate(int src_fd, int dst_fd);
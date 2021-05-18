# include "Virus.h"

int serialize_bloom(bloom filter, char * VirusName, int count, void ** output);

bloom unserialize_bloom(void * input, char ** VirusName, int * count, int *size);

/* Takes a virus list and an id(monitorId) and receives the bloom filter.
 *
 *  id: monitor id.
 *  Vlist: virus list.
 * 
*/
int receive_bloom(int id, Virus * Vlist);

/* Helpes receive_bloom to receive a bloom filter.
 * Calculates how many times a fifo should be read based on the buffer and opens it. 
 * Whatever it receives, it gives it to the unserialize function and then makes a virus list.
 *
 *  id: monitor id.
 *  Vlist: pointer to virus list where there will be inserted the new bloomfilter.
 *  input: what we have got from the fifo.
 *  fd: fifo's file descriptor.
 *  count: pointer to int that when the function returns will have the number of bloomfilters that the monitor id has.
 * 
*/
int receive_bloomHelper(int id, Virus * Vlist, void * input, int fd, int * count);

/* Takes a virus list and sends it through a pipe at the travelmonitor.
 * 
 * monitorId: monitor id.
 * Vlist: virus list.
 * 
*/
int send_bloom(int monitorId, Virus * Vlist);

/* Returns the number of bytes that can be read. It takes into consideration the number of bytes.
 * It is used by receive_bloom function.
 * 
 * size: total bytes of the bloom.
 * times: times that we have read the pipe.
 * 
*/
int BytestoRead(int size, int times);

/* Takes an array of strings and convert it into a fifo's input .
 * 
 * Array: array of strings.
 * Length: pointer to int. When it exit this pointer has the number of bytes that the inputs is.
 * 
*/
void * serialize_commands(char ** Array, int * Length);

/* Takes a fifo's input and makes it into an array of strings.
 * 
 * input: fifo's input.
 * 
*/
char ** unserialize_commands(void * input);
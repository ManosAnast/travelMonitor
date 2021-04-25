# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <error.h>
# include <stdbool.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <fcntl.h>
# define Parent "travelMonitor"

/* Makes the name of the fifo. 
 *
 * id: Number of the monitor that we want to make a fifo for.
 * name: Fifo name.
 * name_max: Max size of fifo name.
 * 
 * return: true if everything goes fine. Otherwise, it returns false.
*/
bool make_fifo_name(int id, char * name, size_t name_max);

/* Initialiaze a fifo.
 *
 * Num: Number of monitor that the fifo is initialized for. It is given to the name.
 *
 * return: 0 if everything goes fine. Otherwise, it returns -1.
*/
int Fifo_init(int Num);

/* Write to fifo.
 *
 * Num: Number of monitor that the fifo is for.
 * Input: Data that we want to write to fifo.
 * 
 * return: 0 if everything goes fine. Otherwise, it returns -1.
*/
int Fifo_write(int Num, void * Input);

/* Read from fifo.
 *
 * Num: Number of monitor that the fifo is for.
 * 
 * return: The fifo's data if everything goes fine. Otherwise, it returns NULL.
*/
void * Fifo_read(int Num);
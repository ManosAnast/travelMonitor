# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <string.h>
# include <dirent.h>
# include <errno.h>
# include <stdbool.h>
# include <time.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/select.h>
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
 * Read: Type of fifo. If read=true then it is a read fifo. If it is false then it is a write.
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
int Fifo_write(int Num, void * Input, int size, int *fd);

/* Read from fifo.
 *
 * Num: Number of monitor that the fifo is for.
 * 
 * return: The fifo's data if everything goes fine. Otherwise, it returns NULL.
*/
void * Fifo_read(int Num, int buffer, int * fd);

/* Write to fifo.
 *
 * Num: Number of monitor that the fifo is for.
 * Input: Data that we want to write to fifo.
 * 
 * return: 0 if everything goes fine. Otherwise, it returns -1.
*/
int Fifo_writeCommands(int Num, void * Input, int size, int * fd);

/* Read from fifo.
 *
 * Num: Number of monitor that the fifo is for.
 * 
 * return: The fifo's data if everything goes fine. Otherwise, it returns NULL.
*/
void * Fifo_readCommands(int Num, int buffer, int * fd);
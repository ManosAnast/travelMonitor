# include "Country.h"

extern volatile sig_atomic_t interrupt_flag_usr, interrupt_flag_iq, interrupt_flag_kill  ;


/* Takes a country node and a signal and send's it to the correct process.
 * 
 * CTemp: Country node.
 * signo: signal.
 * 
*/
void SendSignal(Country * CTemp, int signo);

/* SIGUSR handler.
 * 
*/
void signal_usr(int signo);

/* SIGINT and SIGQUIT handler.
 * 
*/
void signal_iq(int signo);

/* SIGCHLD handler.
 * 
*/
void signal_chld(int signo);

/* SIGKILL handler.
 * 
*/
void signal_kill(int signo);

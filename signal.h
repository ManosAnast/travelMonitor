# include "TMInterface.h"

extern volatile sig_atomic_t interrupt_flag_usr ;

void SendSignal(Country * CTemp, int signo);

void signal_usr(int signo);

void signal_int(int signo);

void signal_chld(int signo);

void signal_quit(int signo);

void signal_kill(int signo);

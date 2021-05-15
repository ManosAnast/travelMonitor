# include "TMInterface.h"

extern volatile sig_atomic_t interrupt_flag_usr, interrupt_flag_iq, interrupt_flag_kill  ;

void SendSignal(Country * CTemp, int signo);

void signal_usr(int signo);

void signal_iq(int signo);

void signal_chld(int signo);

void signal_kill(int signo);

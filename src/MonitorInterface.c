# include "MonitorInterface.h"

void Start(char * text)
{
    int ch,Size=CountSize(text);

    Virus * Vlist=VirusInit();
    HTCreate(Size);
    FILE * fp;

    DIR * dir=opendir(text);
    struct dirent * ent;
    int i=0;
    while ((ent = readdir(dir)) != NULL){
        text=FrontTrack(text, ent->d_name);
        if ( strcmp(ent->d_name, ".") && strcmp(ent->d_name, "..")){
            struct stat path_stat;
            if( lstat(text, &path_stat) == -1){
                perror("monitor Stat");
                return;
            }
            else if( (path_stat.st_mode & S_IFMT) == S_IFREG){ //File
                
                fp=fopen(text , "r");   

                // Create the hash table and a string that takes each line
                char *str;
                str=(char *)calloc(150,sizeof(char));
                
                char **Array=(char**)malloc(8*sizeof(char *)); // make an arry of strings with 50 characters each string.
                for(int i=0 ; i< 8 ; i++){
                    Array[i]=(char*)malloc(50*sizeof(char));
                }
                const char * s=" "; // Char that indicates the breaking point of the string
                while (1){
                    int i=0;
                    while ((ch = getc(fp)) != EOF) // Read each line of the entries
                    {
                        if(ch == '\n'){
                            break;
                        }
                        str[i]=ch;  //str is a string that keeps each line that we get from the file.
                        i++;
                    }
                    str[i]='\0';

                    if( feof(fp) ) { 
                        break ;
                    }
                    
                    BreakString(&Array, str, s, 8);
                    /* If the citizen has been vaccinated, yes, insert true. Otherwise insert false*/
                    if (!strcmp(Array[6],"NO")){
                        if(HTSearch(atoi(Array[0]), Array[5])!=NULL){ // Check for duplication
                            printf("Citizen with %d has already been added for %s\n", atoi(Array[0]), Array[5]);
                        }
                        else{
                            int Flag=HTInsert(atoi(Array[0]), Array[1], Array[2], Array[3], atoi(Array[4]), Array[5], false, Array[7]);
                            if (Flag){ // If the hash table insertion had an error, don't insert to the other structs.
                                VirusInsert(&Vlist, Array[0], Array[5], false , Array[7]);
                            }
                        }
                    }
                    else if (!strcmp(Array[6],"YES")){
                        if(HTSearch(atoi(Array[0]), Array[5])!=NULL){ // Check for duplication
                            printf("Citizen with %d has already been added for %s\n", atoi(Array[0]), Array[5]);
                        }
                        else{
                            int Flag=HTInsert(atoi(Array[0]), Array[1], Array[2], Array[3], atoi(Array[4]), Array[5], true, Array[7]);
                            if(Flag){  // If the hash table insertion had an error, don't insert to the other structs.
                                VirusInsert(&Vlist, Array[0], Array[5], true, Array[7]);
                            }
                        }
                    }  
                }
                fclose(fp);
                for (int i = 0; i < 8; i++){
                    free(Array[i]);
                }
                free(Array);
                free(str);
            
            }
        }
        text=BackTrack(text);
    }
    free(dir); 

    int flag=send_bloom(monitorId, Vlist);
    if (flag<0){ // If something goes wrong with fifo_write, free all the allocated memory and return.
        VirusDestroy(&Vlist);
        exit(1);
    }
    
    TTYMonitor(Vlist, text);

    VirusDestroy(&Vlist);
    HTDestroy();
    return;
}

void TTYMonitor(Virus * Vlist, char * text)
{
    int fd;
    char fifo_name[100];
    if(snprintf(fifo_name, sizeof(fifo_name), "./fifo/TravelMonitor%d", monitorId)<0){
        return;
    }
    mkfifo(fifo_name, 0666);
    
    void (*oldhandler)(int);
    oldhandler = signal(SIGUSR1, signal_usr);
    oldhandler = signal(SIGINT, signal_iq);
    oldhandler = signal(SIGQUIT, signal_iq);
    
    while (1){
        void * Input=calloc(buffer, sizeof(void)); 

        fd=open(fifo_name, O_RDONLY);
        if(fd<0){
            perror("open failed:");
            return;
        }

        struct timeval  timeout;
        fd_set fds;
        int maxfd=fd;
        int res=0, s;
        char buf[256];

        FD_ZERO(&fds); // Clear FD set for select
        FD_SET(fd, &fds);
        timeout.tv_sec = 60; /* One minute */
        timeout.tv_usec = 0; /* and no millionths of seconds */

        s=select(maxfd + 1, &fds, NULL, NULL, &timeout);
        if (s<0 && errno==EINTR){ // Check for select error and signal.
            continue;
        }
        else if (FD_ISSET(fd, &fds)){
            res=read(fd, Input, buffer);
            if(res<0){
                perror("read failed");
                close(fd);
                return;
            }
            if (res == 0){
                continue;
            }
        }
        else{
            continue;
        }
        
        close(fd);

        // Check for signals.
        if(interrupt_flag_usr){
            addVaccinationRecords(Vlist, text);
            interrupt_flag_usr=0;
        }
        if(interrupt_flag_iq){
            break;
        }
        
        // Unserialize the commands.
        char ** Array=unserialize_commands(Input);

        if(!strcmp(Array[0], "exit")){
            for (int i = 0; i < 6; i++){
                free(Array[i]);
            }
            free(Array); free(Input);
            break;
        }
        if (!strcmp(Array[0], "travelRequest")){
            travelRequestMonitor(Array[1], Array[5]);
        }
        else if ( !strcmp(Array[0], "searchVaccinationStatus") ){
            searchVaccinationStatusMonitor(Array[1]);
        }
        
        for (int i = 0; i < 6; i++){
            free(Array[i]);
        }
        free(Array); free(Input);
    }
    return;
}
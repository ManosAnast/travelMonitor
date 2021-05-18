# include "Interface.h"

int buffer, BloomNum;
volatile sig_atomic_t interrupt_flag_usr, interrupt_flag_iq, interrupt_flag_kill ;


int main(int argc, char * argv[])
{
    if (argc < 9){
        printf("Call format: ./travelMonitor –m numMonitors -b bufferSize -s sizeOfBloom -i input_dir\n");
        return 1;
    }
    int numMonitors;
    char * input;
    for (int i = 1; i < argc; i++){
        if ( !strcmp(argv[i],"-m") ){
            numMonitors=atoi(argv[++i]);
        }
        else if ( !strcmp(argv[i],"-b") ){
            buffer=atoi(argv[++i]);
        }
        else if ( !strcmp(argv[i],"-s") ){
            BloomNum=atoi(argv[++i]);
        }
        else if ( !strcmp(argv[i],"-i") ){
            input=(char *)calloc(strlen(argv[++i])+10, sizeof(char));
            strcpy(input, argv[i]);
        }
    }

    
    char * buf=(char *)malloc(4*sizeof(char));
    sprintf(buf, "%d", buffer);
    char * filternum=(char *)malloc(7*sizeof(char));
    sprintf(filternum, "%d", BloomNum);
    
    Country * Clist=CountryCreate();
    for(int i=0;i<numMonitors;i++) // loop will run n times (n=5)
    {
        Fifo_init(i);
        pid_t pid=fork();
        if(pid == 0){
            char * id=(char *)malloc(2*sizeof(char));
            sprintf(id, "%d", i);
            execlp("./monitor", "monitor", id, buf, filternum, NULL);
            perror("exec failed");
            exit(0);
        }
        else{
            CountryInsert(&Clist, input, i, pid);
        }
    }
    free(buf); free(filternum);

    // Traverse the input directory and assign the countries to the monitors
    DIR * dir=opendir(input);
    struct dirent * ent;
    int i=0, fd;
    while ((ent = readdir(dir)) != NULL){
        input=FrontTrack(input, ent->d_name);
        if ( strcmp(ent->d_name, ".") && strcmp(ent->d_name, "..")){
            struct stat path_stat;
            if( lstat(input, &path_stat) == -1){
                perror("Stat");
                return -1;
            }
            if( (path_stat.st_mode & S_IFMT) == S_IFDIR){ //Directory
                CountryInsert(&Clist, input, i, 0);
                Fifo_write(i, input, (strlen(input)+1)*sizeof(char), &fd); 
                close(fd);
            }
            i+=1;
        }
        input=BackTrack(input);
    }
    free(dir);
    
    // Take bloomfilters
    Virus * Vlist = VirusInit();
    char * country;
    for (int i = 0; i < numMonitors; i++){
        int flag = receive_bloom(i, Vlist);
    }

    TTY(Vlist, Clist);

    // Unlink all the pipes and wait for the processes.
    for(int i=0;i<numMonitors;i++){ // loop will run n times (n=5)
        char fifo_name[100];
        if(!make_fifo_name(i, fifo_name, sizeof(fifo_name))){
            return -1;
        }
        unlink(fifo_name);
        if(snprintf(fifo_name, sizeof(fifo_name), "./fifo/TravelMonitor%d", i)<0){
            return -1;
        }
        unlink(fifo_name);
    }
    wait(NULL);
    

    VirusDestroy(&Vlist);
    CountryDestroy(&Clist);
    free(input);

    return 0;
}
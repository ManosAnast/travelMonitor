# include "Interface.h"

int Level, BloomNum;

int main(int argc, char * argv[])
{
    if (argc < 9){
        printf("Call format: ./travelMonitor –m numMonitors -b bufferSize -s sizeOfBloom -i input_dir\n");
        return 1;
    }
    int numMonitors, buffer;
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
            input=(char *)calloc(strlen(argv[++i])+1, sizeof(char));
            strcpy(input,argv[i]);
        }
    }

    
    char * buf=(char *)malloc(sizeof(char));
    sprintf(buf, "%d", buffer);
    char * filternum=(char *)malloc(sizeof(char));
    sprintf(filternum, "%d", BloomNum);
    
    for(int i=0;i<numMonitors;i++) // loop will run n times (n=5)
    {
        Fifo_init(i);
        pid_t pid=fork();
        if(pid == 0){
            char * id=(char *)malloc(sizeof(char));
            sprintf(id, "%d", i);
            execlp("./monitor", "monitor", id, buf, filternum, NULL);
            perror("exec failed");
            exit(0);
        }
    }

    // Traverse the input directory and assign the countries to the monitors
    DIR * dir=opendir(input);
    struct dirent * ent;
    int i=0;
    while ((ent = readdir(dir)) != NULL){
        input=FrontTrack(input, ent->d_name);
        if ( strcmp(ent->d_name, ".") && strcmp(ent->d_name, "..")){
            struct stat path_stat;
            if( lstat(input, &path_stat) == -1){
                perror("Stat");
                return -1;
            }
            if( (path_stat.st_mode & S_IFMT) == S_IFDIR){ //Directory
                Fifo_write(i, input, (strlen(input)+1)*sizeof(char)); 
            }
            i+=1;
        }
        input=BackTrack(input);
    }

    // Unlink all the pipes and wait for the processes.
    for(int i=0;i<numMonitors;i++){ // loop will run n times (n=5)
        char fifo_name[100];
        if(!make_fifo_name(i, fifo_name, sizeof(fifo_name))){
            return -1;
        }
        unlink(fifo_name);
    }
    wait(NULL);
    
    // Start(File);

    free(input);

    return 0;
}
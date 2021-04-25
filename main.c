# include "Fifo.h"

int BloomNum;


int main(int argc, char * argv[])
{
    // if (argc < 9){
    //     printf("Call format: ./travelMonitor –m numMonitors -b bufferSize -s sizeOfBloom -i input_dir\n");
    //     return 1;
    // }
    // int numMonitors, buffer;
    // char * input;
    // for (int i = 1; i < argc; i++){
    //     if ( !strcmp(argv[i],"-m") ){
    //         numMonitors=atoi(argv[++i]);
    //     }
    //     else if ( !strcmp(argv[i],"-b") ){
    //         buffer=atoi(argv[++i]);
    //     }
    //     else if ( !strcmp(argv[i],"-s") ){
    //         BloomNum=atoi(argv[++i]);
    //     }
    //     else if ( !strcmp(argv[i],"-i") ){
    //         input=(char *)calloc(strlen(argv[++i])+1, sizeof(char));
    //         strcpy(input,argv[i]);
    //     }
    // }
    // 
    // // If the input_dir doesn't exist, make it.
    // if(open(input, O_RDONLY) == -1){
    //     mkdir(input, 0700);
    // }

    for(int i=0;i<1;i++) // loop will run n times (n=5)
    {
        if(fork() == 0)
        {
            printf("[son] pid %d from [parent] pid %d\n",getpid(),getppid());
            exit(0);
        }
    }
    for(int i=0;i<1;i++){ // loop will run n times (n=5)
    wait(NULL);

    // free(input);
    return 0;
}
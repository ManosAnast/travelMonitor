# include "Interface.h"

int Level, BloomNum;

char * BackTrack(char * src)
{
    int i, length=strlen(src);
    for (i = length; i > 0 ; i--){
        if (src[i] == '/' && i !=length-1 ){ // Find the last directory/file and remove it
            src[i+1]='\0';
            break;
        }
    }
    return src;
}


char * FrontTrack(char * src, char * Next)
{
    int length=strlen(src);
    if( src[length-1] != '/'){ // If the path doesn't have / at the end, add it and the add the new directory/file.
        strcat(src, "/");
        strcat(src, Next);
    }
    else{
        strcat(src, Next);
    }
    return src;
}



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
    
    for(int i=0;i<numMonitors;i++) // loop will run n times (n=5)
    {
        Fifo_init(i);
        pid_t pid=fork();
        if(pid == 0)
        {
            char * id=(char *)malloc(sizeof(char));
            sprintf(id, "%d", i);
            char * buf=(char *)malloc(sizeof(char));
            sprintf(buf, "%d", buffer);
            execlp("./a.out", "a.out", id, buf, NULL);
            perror("exec failed");
            exit(0);
        }
        Fifo_write(i, "Greece", 7);
    }
    for(int i=0;i<numMonitors;i++) // loop will run n times (n=5)
    wait(NULL);

    // DIR * dir=opendir(input);
    // struct dirent * ent;
    // int i=0;
    // while ((ent = readdir(dir)) != NULL){
    //     input=FrontTrack(input, ent->d_name);
    //     struct stat path_stat;
    //     if( lstat(input, &path_stat) == -1){
    //         perror("Stat");
    //         return -1;
    //     }
    //     if( (path_stat.st_mode & S_IFMT) == S_IFDIR){ //Directory
    //        Fifo_write(i, input); 
    //     }
    //     i+=1;
    // }

    
    // Start(File);

    // free(input);
    
    return 0;
}
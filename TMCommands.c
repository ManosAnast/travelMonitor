# include "Interface.h"

void travelRequestMonitor(int monitorId, char * Id, char * VirusName)
{
    Date * VaccDate;
    int fd;

    Citizens * Rec=HTSearch(atoi(Id), VirusName); 
    if (Rec->Vaccinated){
        VaccDate=Rec->Timing;
    }
    else{
        VaccDate=NULL;
    }

    int Length;
    if (VaccDate==NULL){
        char ** Answer=(char **)calloc(2, sizeof(char *));
        Answer[0]=(char *)calloc(2, sizeof(char));
        strcpy(Answer[0], "NO");
        Answer[1]=(char *)calloc(strlen(NULLstring), sizeof(char));strcpy(Answer[3], NULLstring);
        void * input=serialize_commands(Answer, &Length);
        Fifo_writeCommands(monitorId, input, Length, &fd);
        free(Answer[0]); free(Answer[1]); free(Answer);
    }
    else{
        char ** Answer=(char **)calloc(4, sizeof(char *));
        Answer[0]=(char *)calloc(2, sizeof(char));sprintf(Answer[0], "%d", VaccDate->Days);
        Answer[1]=(char *)calloc(2, sizeof(char));sprintf(Answer[1], "%d", VaccDate->Month);
        Answer[2]=(char *)calloc(4, sizeof(char));sprintf(Answer[2], "%d", VaccDate->Year);
        Answer[3]=(char *)calloc(strlen(NULLstring), sizeof(char));strcpy(Answer[3], NULLstring);
        void * input=serialize_commands(Answer, &Length);
        Fifo_writeCommands(monitorId, input, Length, &fd);
        free(Answer[0]); free(Answer[1]); free(Answer[2]); free(Answer);
    }
    close(fd);
}

void searchVaccinationStatusMonitor(int monitorId, char * Id)
{
    Citizens * Rec = HTSearchID(atoi(Id));
    int Length, fd;
    if (Rec == NULL){
        char ** Answer=(char **)calloc(2, sizeof(char *));
        Answer[0]=(char *)calloc(2, sizeof(char));
        strcpy(Answer[0], "NO");
        Answer[1]=(char *)calloc(strlen(NULLstring), sizeof(char));strcpy(Answer[1], NULLstring);
        void * input=serialize_commands(Answer, &Length);
        Fifo_writeCommands(monitorId, input, Length, &fd);
        free(Answer[0]); free(Answer[1]); free(Answer);
        return;
    }
    char ** Answer=(char **)calloc(6, sizeof(char *));
    Answer[0]=(char *)calloc(4, sizeof(char));sprintf(Answer[0], "%d", Rec->citizenId);
    Answer[1]=(char *)calloc(strlen(Rec->FirstName), sizeof(char));strcpy(Answer[1], Rec->FirstName);
    Answer[2]=(char *)calloc(strlen(Rec->LastName), sizeof(char));strcpy(Answer[2], Rec->LastName);
    Answer[3]=(char *)calloc(2, sizeof(char));sprintf(Answer[3], "%d", Rec->Age);
    Answer[4]=(char *)calloc(strlen(Rec->Virus), sizeof(char));strcpy(Answer[4], Rec->Virus);
    if(Rec->Timing != NULL){
        Answer[5]=(char *)calloc(10, sizeof(char));sprintf(Answer[5], "%d-%d-%d", Rec->Timing->Days, Rec->Timing->Month, Rec->Timing->Year);
    }
    else{
        Answer[5]=(char *)calloc(19, sizeof(char));strcpy(Answer[5], "NOT YET VACCINATED");
    }
    void * input=serialize_commands(Answer, &Length);
    Fifo_writeCommands(monitorId, input, Length, &fd);
    free(Answer[0]); free(Answer[1]); free(Answer[2]); free(Answer[3]); free(Answer[4]); free(Answer[5]); free(Answer);
    return;
}

void addVaccinationRecords(Virus * Vlist, char * text)
{
    int ch,Size=0;
    FILE * fp;

    DIR * dir=opendir(text);
    struct dirent * ent;
    int i=0;
    printf("1 entered addVaccinationRecords\n");
    while ((ent = readdir(dir)) != NULL){
        printf("2 entered addVaccinationRecords\n");
        text=FrontTrack(text, ent->d_name);
        printf("3 entered addVaccinationRecords %s\n", ent->d_name);
        if ( strcmp(ent->d_name, ".") && strcmp(ent->d_name, "..")){
            struct stat path_stat;
            printf("4 entered addVaccinationRecords\n");
            
            if( lstat(text, &path_stat) == -1){
                perror("monitor Stat");
                return;
            }
            else if( (path_stat.st_mode & S_IFMT) == S_IFREG){ //File
            
                fp=fopen(text , "r");   
                // Find the number of entries that the given file has
                while(1) {
                    ch = getc(fp);
                    if( feof(fp) ) { 
                        break ;
                    }
                    if(ch == '\n'){  //Finds how many students are added.
                        Size+=1;
                    }
                }
                fclose(fp);
                fp=fopen(text , "r");   

                // Create the hash table and a string that takes each line
                char *str;
                str=(char *)calloc(150,sizeof(char));
                
                char **Array=(char**)malloc(8*sizeof(char *)); // make an arry of strings with 50 characters each string.
                for(int i=0 ; i< 8 ; i++){
                    Array[i]=(char*)malloc(50*sizeof(char));
                }
                const char * s=" "; // Char that indicates the breaking point of the string
                printf("5 entered addVaccinationRecords\n");
                while (1){
                    int i=0;
                    printf("6 entered addVaccinationRecords\n");
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
                    
                    printf("7 entered addVaccinationRecords\n");
                    BreakString(&Array, str, s, 8);
                    /* If the citizen has been vaccinated, yes, insert true. Otherwise insert false*/
                    printf("8 entered addVaccinationRecords\n");
                    if (!strcmp(Array[6],"NO")){
                        printf("9 entered addVaccinationRecords %d, %s\n", atoi(Array[0]), Array[5]);
                        if(HTSearch2(atoi(Array[0]), Array[5])!=NULL){ // Check for duplication
                            printf("Citizen with %d has already been added for %s\n", atoi(Array[0]), Array[5]);
                        }
                        else{
                            int Flag=HTInsert(atoi(Array[0]), Array[1], Array[2], Array[3], atoi(Array[4]), Array[5], false, Array[7]);
                            if (Flag){ // If the hash table insertion had an error, don't insert to the other structs.
                                printf("10 entered addVaccinationRecords\n");
                                VirusInsert(&Vlist, Array[0], Array[5], false , Array[7]);
                                printf("10.5 entered addVaccinationRecords\n");
                            }
                        }
                    }
                    else if (!strcmp(Array[6],"YES")){
                        printf("11 entered addVaccinationRecords\n");
                        if(HTSearch2(atoi(Array[0]), Array[5])!=NULL){ // Check for duplication
                            printf("Citizen with %d has already been added for %s\n", atoi(Array[0]), Array[5]);
                        }
                        else{
                            printf("12 entered addVaccinationRecords\n");
                            int Flag=HTInsert(atoi(Array[0]), Array[1], Array[2], Array[3], atoi(Array[4]), Array[5], true, Array[7]);
                            if(Flag){  // If the hash table insertion had an error, don't insert to the other structs.
                                VirusInsert(&Vlist, Array[0], Array[5], true, Array[7]);
                            }
                        }
                    }  
                }
                printf("13 entered addVaccinationRecords\n");
                fclose(fp);
                Level=Log(Size);
                for (int i = 0; i < 8; i++){
                    free(Array[i]);
                }
                free(Array);
                free(str);
            
            }
        }
        printf("14 entered addVaccinationRecords\n");
        text=BackTrack(text);
    }
    int size, fd;
    char ** Array=(char **)calloc(1, sizeof(char *));
    Array[0]=(char *)calloc(strlen(NULLstring), sizeof(char)); strcpy(Array[0], NULLstring);
    void * Input=serialize_commands(Array, &size);
    Fifo_writeCommands(0, Input, size, &fd); close(fd);
    free(Array[0]); free(Array);
}

int SameDate(int fd1, int fd2)
{
    struct stat buff1;
    struct stat buff2;

    //Take inode information for the source file
    if (fstat(fd1, &buff1) == -1){
        perror("Fstat"); return 1;
    }

    //Take inode information for the destination file
    if (fstat(fd2, &buff2) == -1){
        perror("Fstat"); return 1;
    }

    //Check if files are the same size
    if (buff1.st_mtime > buff2.st_mtime){
        return 1;
    }
    else if(buff1.st_mtime <= buff2.st_mtime){
        return 0;
    }
    return -1;
}

// char ** InsertFile(char ** Array, char * NewFile)
// {
//     int size;

// }
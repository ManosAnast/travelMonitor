# include "MonitorInterface.h"

void travelRequestMonitor(char * Id, char * VirusName)
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
        Answer[0]=(char *)calloc(3, sizeof(char));
        strcpy(Answer[0], "NO");
        Answer[1]=(char *)calloc(strlen(NULLstring)+1, sizeof(char));strcpy(Answer[3], NULLstring);
        void * input=serialize_commands(Answer, &Length);
        Fifo_writeCommands(monitorId, input, Length, &fd);
        free(Answer[0]); free(Answer[1]); free(Answer); free(input);
    }
    else{
        char ** Answer=(char **)calloc(4, sizeof(char *));
        Answer[0]=(char *)calloc(3, sizeof(char));sprintf(Answer[0], "%d", VaccDate->Days);
        Answer[1]=(char *)calloc(3, sizeof(char));sprintf(Answer[1], "%d", VaccDate->Month);
        Answer[2]=(char *)calloc(5, sizeof(char));sprintf(Answer[2], "%d", VaccDate->Year);
        Answer[3]=(char *)calloc(strlen(NULLstring)+1, sizeof(char));strcpy(Answer[3], NULLstring);
        void * input=serialize_commands(Answer, &Length);
        Fifo_writeCommands(monitorId, input, Length, &fd);
        free(Answer[0]); free(Answer[1]); free(Answer[2]); free(Answer[3]); free(Answer); free(input);
    }
    close(fd);
}

void searchVaccinationStatusMonitor(char * Id)
{
    Citizens * Rec = HTSearchID(atoi(Id));
    int Length, fd;
    if (Rec == NULL){
        char ** Answer=(char **)calloc(2, sizeof(char *));
        Answer[0]=(char *)calloc(3, sizeof(char));
        strcpy(Answer[0], "NO");
        Answer[1]=(char *)calloc(strlen(NULLstring)+1, sizeof(char));strcpy(Answer[1], NULLstring);
        void * input=serialize_commands(Answer, &Length);
        Fifo_writeCommands(monitorId, input, Length, &fd);
        free(Answer[0]); free(Answer[1]); free(Answer);
        free(input);
        return;
    }
    char ** Answer=(char **)calloc(6, sizeof(char *));
    Answer[0]=(char *)calloc(4, sizeof(char));sprintf(Answer[0], "%d", Rec->citizenId);
    Answer[1]=(char *)calloc(strlen(Rec->FirstName)+1, sizeof(char));strcpy(Answer[1], Rec->FirstName);
    Answer[2]=(char *)calloc(strlen(Rec->LastName)+1, sizeof(char));strcpy(Answer[2], Rec->LastName);
    Answer[3]=(char *)calloc(4, sizeof(char));sprintf(Answer[3], "%d", Rec->Age);
    Answer[4]=(char *)calloc(strlen(Rec->Virus)+1, sizeof(char));strcpy(Answer[4], Rec->Virus);
    if(Rec->Timing != NULL){
        Answer[5]=(char *)calloc(11, sizeof(char));sprintf(Answer[5], "%d-%d-%d", Rec->Timing->Days, Rec->Timing->Month, Rec->Timing->Year);
    }
    else{
        Answer[5]=(char *)calloc(19, sizeof(char));strcpy(Answer[5], "NOT YET VACCINATED");
    }
    void * input=serialize_commands(Answer, &Length);
    Fifo_writeCommands(monitorId, input, Length, &fd);
    free(Answer[0]); free(Answer[1]); free(Answer[2]); free(Answer[3]); free(Answer[4]); free(Answer[5]); free(Answer);
    free(input);
    return;
}

int addVaccinationRecords(Virus * Vlist, char * text)
{
    int ch,Size=0;
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
                return -1;
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
                        if(HTSearch(atoi(Array[0]), Array[5])==NULL){
                            int Flag=HTInsert(atoi(Array[0]), Array[1], Array[2], Array[3], atoi(Array[4]), Array[5], false, Array[7]);
                            if (Flag){ // If the hash table insertion had an error, don't insert to the other structs.
                                VirusInsert(&(Vlist->Next), Array[0], Array[5], false , Array[7]);
                            }
                        }
                    }
                    else if (!strcmp(Array[6],"YES")){
                        if(HTSearch(atoi(Array[0]), Array[5])==NULL){
                            int Flag=HTInsert(atoi(Array[0]), Array[1], Array[2], Array[3], atoi(Array[4]), Array[5], true, Array[7]);
                            if(Flag){  // If the hash table insertion had an error, don't insert to the other structs.
                                VirusInsert(&(Vlist->Next), Array[0], Array[5], true, Array[7]);
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
    int size, fd;
    char ** Array=(char **)calloc(1, sizeof(char *));
    Array[0]=(char *)calloc(strlen(NULLstring)+1, sizeof(char)); strcpy(Array[0], NULLstring);
    void * Input=serialize_commands(Array, &size);
    Fifo_writeCommands(monitorId, Input, size, &fd);
    free(Array[0]); free(Array); free(Input); close(fd);
    return 0;
}
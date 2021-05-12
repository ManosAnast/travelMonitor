# include "Interface.h"

void nothing()
{
    return;
}

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


void BreakString(char *** Array, char * str, const char * s, int Num)
{
    int index=0;
    char ** Temp=*Array;
    char * Break=(char *)calloc(strlen(str)+1, sizeof(char)); 
    strcpy(Break, str); // I use a new string variable because i want the str variable intacted
    // Use of strtok in order to break the string.
    char * token = strtok(Break, s);
    while (token != NULL && index < Num)   //this loop gives the variables that we need to the array from the token(strtok)
    {
        strcpy(Temp[index],token);
        token = strtok(NULL, s);
        index++;
    }
    // If the array isn't "filled" add the nullstring to those that are empty.
    for (int i = index; i < Num; i++){
        strcpy(Temp[i], NULLstring);
    }
    free(Break);
    return;
}

void Start(char * text, int monitorId, int buffer)
{
    int ch,Size=0;

    Virus * Vlist=VirusInit();
    Country * CList=CountryCreate();
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
                HTCreate(Size);
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
                Level=Log(Size);
                for (int i = 0; i < 8; i++){
                    free(Array[i]);
                }
                free(Array);
                free(str);
            
            }
        }
        text=BackTrack(text);
    }

    VirusSkipList(&Vlist);
    
    printf("%d virus[%p]\n", monitorId,  Vlist);
    Virus * VTemp=Vlist;
    int flag=send_bloom(monitorId, buffer, VTemp);
    free(VTemp);
    printf("%d virus[%p]\n", monitorId, Vlist);
    // VirusPrint(Vlist);
    if (flag<0){ // If something goes wrong with fifo_write, free all the allocated memory and return.
        Destroy(Vlist, CList);
        exit(1);
    }
    
    TTYMonitor(Vlist, monitorId, buffer, text);


    Destroy(Vlist, CList);
    return;
}

void Destroy(Virus * Vlist, Country * CList)
{
    VirusDestroy(&Vlist);
    CountryDestroy(&CList);
    HTDestroy();
}

MonitorCheck *  MCInit()
{
    MonitorCheck * MonitorList=(MonitorCheck *)calloc(1, sizeof(MonitorCheck));
    MonitorList->VirusName=(char *)calloc(strlen(NULLstring), sizeof(char));
    strcpy(MonitorList->VirusName, NULLstring);
    MonitorList->CountryName=(char *)calloc(strlen(NULLstring), sizeof(char));
    strcpy(MonitorList->CountryName, NULLstring);
    MonitorList->Accepted=0; MonitorList->Rejected=0;
    MonitorList->RequestDate=(Date *)calloc(1, sizeof(Date));
    MonitorList->RequestDate=NULL;
    MonitorList->Next=NULL;
    return MonitorList;
}

void MCInsert(MonitorCheck * MonitorList, char * VName, char * CName, bool Accepted, bool Rejected, Date * RequestDate)
{
    MonitorCheck * Temp=MonitorList;
    while (Temp->Next!=NULL){
        if ( !strcmp(Temp->VirusName, VName) && !strcmp(Temp->CountryName, CName) && CheckDateEq(RequestDate, Temp->RequestDate)){
            Temp->Accepted+=(int)Accepted; Temp->Rejected+=(int)Rejected;
            return;
        }
        Temp=Temp->Next;
    }
    MonitorCheck * NewNode=(MonitorCheck *)calloc(1, sizeof(MonitorCheck));
    NewNode->VirusName=(char *)calloc(strlen(VName), sizeof(char));
    strcpy(NewNode->VirusName, VName);
    NewNode->CountryName=(char *)calloc(strlen(CName), sizeof(char));
    strcpy(NewNode->CountryName, CName);
    NewNode->Accepted=(int)Accepted; NewNode->Rejected=(int)Rejected;
    NewNode->RequestDate=RequestDate;
    NewNode->Next=NULL;
    Temp->Next=NewNode;
    return;
}

void MCPrint(MonitorCheck * MonitorList, char * VName, char * CName)
{
    MonitorCheck * Temp=MonitorList;
    while (Temp->Next!=NULL){
        if ( !strcmp(Temp->VirusName, VName) ){
            printf("TOTAL REQUESTS %d\nACCEPTED %d\nREJECTED %d\n", Temp->Accepted+Temp->Rejected, Temp->Accepted, Temp->Rejected);
            return;
        }
        Temp=Temp->Next;
    }
    printf("There is no %s virus at this country\n", VName);
    return;
}

void MCDestroy(MonitorCheck * MonitorList)
{
    MonitorCheck * Current=MonitorList, *Next;
    while (Current!= NULL){
        Next=Current->Next;
        free(Current->CountryName); free(Current->VirusName); free(Current);
        Current=Next;
    }
    return;
}
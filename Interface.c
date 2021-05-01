# include "serialize.h"

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
            printf("%s\n", text);
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
                                CountryInsert(&CList, Array[3]);
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
                                CountryInsert(&CList, Array[3]);
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

    nothing();

    Virus * VTemp=Vlist->Next;

    int exist=bloomBitExist(&(VTemp->filter), "10");
    if (exist)
    {
        printf("MAYBE\n");
    }
    else{
        printf("NOT VACCINATED\n");
    }
    
    int flag=send_bloom(monitorId, buffer, VTemp->filter, VTemp->VirusName);
    if (flag<0){ // If something goes wrong with fifo_write, free all the allocated memory and return.
        Destroy(Vlist, CList);
        exit(1);
    }
    
    // while (VTemp !=NULL){
    //     break;
    //     VTemp=VTemp->Next;
    // }
    

    //TTY();    


    return;
}

void Destroy(Virus * Vlist, Country * CList)
{
    VirusDestroy(&Vlist);
    CountryDestroy(&CList);
    HTDestroy();
}
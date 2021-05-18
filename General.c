# include "General.h"

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
        // src=(char *)realloc(src, (length+strlen(Next)+2)*sizeof(char));
        strcat(src, "/");
        strcat(src, Next);
    }
    else{
        // src=(char *)realloc(src, (length+strlen(Next)+1)*sizeof(char));
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

int CountSize(char * text)
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
            }
        }
        text=BackTrack(text);
    }
    free(dir);
    return Size;
}
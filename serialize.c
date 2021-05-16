# include "Interface.h"

int serialize_bloom(bloom filter, char * VirusName, int count, void ** output)
{
    int Bytes=filter.NumofBytes;
    int size=Bytes+3*sizeof(int)+strlen(VirusName);
    int Length=0;

    *output=(void *)calloc(size, sizeof(void));
    //Keeps the number of bytes that the fifo is going to send.
    memcpy(*output+Length, &count, sizeof(int));

    Length+=sizeof(int);
    memcpy(*output+Length, &size, sizeof(int));

    Length+=sizeof(int);
    memcpy(*output+Length, &Bytes, sizeof(int));

    Length+=sizeof(int);
    memcpy(*output+Length, VirusName, strlen(VirusName));

    char * Arr=filter.bits;
    Length+=strlen(VirusName);
    memcpy(*output+Length, filter.bits, Bytes);

    return size;
}

bloom unserialize_bloom(void * input, char ** VirusName, int * count, int buffer, int *size)
{
    int Length=0, filterbytes;
    
    bloom filter;
    memcpy(count, input+Length, sizeof(int));

    Length+=sizeof(int);    
    memcpy(size, input+Length, sizeof(int));

    Length+=sizeof(int);
    memcpy(&(filter.NumofBytes), input+Length, sizeof(int));

    int Namesize=*size-3*sizeof(int)-filter.NumofBytes;
    if (*size > buffer){
        filterbytes=buffer-3*sizeof(int)-Namesize;
    }
    else{
        filterbytes=filter.NumofBytes;
    }
    
    filter.bits=(char *)calloc(filterbytes, sizeof(char));
    Length+=sizeof(int);
    *VirusName=(char *)calloc(Namesize+1, sizeof(char));
    memcpy(*VirusName, input+Length, Namesize);

    Length+=Namesize;
    memcpy(filter.bits, input+Length, filterbytes);

    return filter;
}

int receivebloomtest(int id, Virus * Vlist, int buffer)
{
    int size=0, /*i=1,*/ fd, count=1;
    char * VirusName;
    for (int i = 0; i < count; i++){
        void * input =Fifo_read(id, buffer, &fd);
        receive_bloom(id, Vlist, buffer, input, fd, &count);
        free(input);
    }

}

int receive_bloom(int id, Virus * Vlist, int buffer, void * input, int fd, int * count)
{
    int size=0, i=1;
    char * VirusName;

    bloom bloomfilter = unserialize_bloom(input+size, &VirusName, count, buffer, &size); 
    int filterbytes=buffer-3*sizeof(int)-strlen(VirusName);
    char * filterbits=(char *)calloc(buffer, sizeof(char));

    int num= size/buffer;
    if (size % buffer != 0){
        num+=1;
    }
    while (i < num){
        int readbytes=BytestoRead(size, i, buffer);
        if(read(fd, input, readbytes)<0){
            perror("read failed");
            close(fd);
            return -1;
        }
        memcpy(filterbits, input, readbytes);
        bloomfilter.bits=(char *)realloc(bloomfilter.bits, filterbytes+readbytes);
        for (int i = 0; i < readbytes; i++){
            bloomfilter.bits[filterbytes+i]=filterbits[i];
        }
        filterbytes+=readbytes;
        
        i+=1;
    }
    
    free(filterbits);
    VirusInsertBloom(&Vlist, VirusName, bloomfilter);
    
    free(VirusName);
    close(fd);
    return 0;
}

int send_bloom(int monitorId, int buffer, Virus * VTemp)
{
    int size=0, fd;
    // printf("1 virus[%p]\n", Vlist);
    // Virus * VTemp=Vlist;
    // printf("2 virus[%p]\n", Vlist);
    int count=VirusCount(VTemp);
    VTemp=VTemp->Next;
    void * output;
    while (VTemp != NULL){
        size = serialize_bloom(VTemp->filter, (VTemp->VirusName), count, &output);
        int flag=Fifo_write(monitorId, output, size, &fd);
        if (flag < 0){
            return -1;
        }
        VTemp=VTemp->Next;
        free(output);
    }
    // printf("3 virus[%p]\n", Vlist);
    close(fd);
    free(VTemp); 
    return 0;
}

int BytestoRead(int size, int times, int buffer)
{
    int Bytes=size-times*buffer;
    if (Bytes > buffer){
        return buffer;
    }
    return Bytes;
}

void * serialize_commands(char ** Array, int * Length)
{
    void * output=malloc(100*sizeof(void));
    *Length=0;
    for (int i = 0; i < 6; i++){
        int size=strlen(Array[i]);
        memcpy(output+*Length, &size, sizeof(int));
        *Length+=sizeof(int);
        
        memcpy(output+*Length, Array[i], size);
        *Length+=size;

        if (!strcmp(Array[i], NULLstring)){
            break;
        }
        
    }
    return output;
}

char ** unserialize_commands(void * input)
{
    int Length=0;
    char ** Array=(char **)calloc(6, sizeof(char *));
    int size; 
    for (int i = 0; i < 6; i++){
        memcpy(&size, input+Length, sizeof(int));
        Array[i]=(char *)calloc(size+1, sizeof(char));

        Length+=sizeof(int);
        memcpy(Array[i], input+Length, size);
        Length+=size;
        if (!strcmp(Array[i], NULLstring)){
            break;
        }
    }
    return Array;
}

void * serialize_citizen(Citizens * Rec, int * Length, int buffer)
{
    void * output=malloc(buffer*sizeof(void));
    *Length=0;
    
    memcpy(output+*Length, &(Rec->citizenId), sizeof(int));

    *Length+=sizeof(int);
    // memcpy
}

Citizens * unserialize_citizen(void * input)
{

}
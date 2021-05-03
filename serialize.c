# include "serialize.h"

int serialize_bloom(bloom filter, char ** VirusName, int count, void ** output)
{
    int Bytes=filter.NumofBytes;
    int size=Bytes+3*sizeof(int)+strlen(*VirusName);
    int Length=0;

    //Keeps the number of bytes that the fifo is going to send.
    memcpy(*output+Length, &count, sizeof(int));

    Length+=sizeof(int);
    memcpy(*output+Length, &size, sizeof(int));

    Length+=sizeof(int);
    memcpy(*output+Length, &Bytes, sizeof(int));

    Length+=sizeof(int);
    memcpy(*output+Length, *VirusName, strlen(*VirusName));

    char * Arr=filter.bits;
    Length+=strlen(*VirusName);
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
    *VirusName=(char *)calloc(Namesize, sizeof(char));
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
    
    close(fd);
    return 0;
}

int send_bloom(int monitorId, int buffer, Virus * Vlist)
{
    int size=0;
    Virus * VTemp=Vlist->Next;
    int count=VirusCount(Vlist);
    void * output;
    while (VTemp != NULL){
        size = serialize_bloom(VTemp->filter, &(VTemp->VirusName), count, &output);
        int flag=Fifo_write(monitorId, output, size);
        if (flag < 0){
            return -1;
        }
        
        VTemp=VTemp->Next;
    }
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
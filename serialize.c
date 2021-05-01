# include "serialize.h"

int serialize_bloom(bloom filter, char ** CountryName, void ** output)
{
    int Bytes=filter.NumofBytes;
    int size=Bytes+2*sizeof(int)+strlen(*CountryName);
    int Length=0;

    //Keeps the number of bytes that the fifo is going to send.
    memcpy(*output+Length, &size, sizeof(int));

    Length+=sizeof(int);
    memcpy(*output+Length, &Bytes, sizeof(int));

    Length+=sizeof(int);
    memcpy(*output+Length, *CountryName, strlen(*CountryName));

    char * Arr=filter.bits;
    Length+=strlen(*CountryName);
    memcpy(*output+Length, filter.bits, Bytes);

    return size;
}

bloom unserialize_bloom(void * input, char ** CountryName, int buffer, int *size)
{
    int Length=0, filterbytes;
    memcpy(size, input+Length, sizeof(int));
    
    bloom filter;
    Length+=sizeof(int);
    memcpy(&(filter.NumofBytes), input+Length, sizeof(int));

    int Namesize=*size-2*sizeof(int)-filter.NumofBytes;
    if (*size > buffer){
        filterbytes=buffer-2*sizeof(int)-Namesize;
    }
    else{
        filterbytes=filter.NumofBytes;
    }
    
    filter.bits=(char *)calloc(filterbytes, sizeof(char));
    Length+=sizeof(int);
    *CountryName=(char *)calloc(Namesize, sizeof(char));
    memcpy(*CountryName, input+Length, Namesize);

    Length+=Namesize;
    memcpy(filter.bits, input+Length, filterbytes);

    return filter;
}

bloom receive_bloom(int id, char ** CountryName, int buffer)
{
    int size=0, i=1, fd;
    void * input =Fifo_read(id, buffer, &fd);
    bloom bloomfilter = unserialize_bloom(input, CountryName, buffer, &size);
    int filterbytes=buffer-2*sizeof(int)-strlen(*CountryName);
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
            break;
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
    close(fd);
    return bloomfilter;
}

int send_bloom(int monitorId, int buffer, bloom filter, char * CountryName)
{
    void * output;
    int size = serialize_bloom(filter, &CountryName, &output);
    int flag=Fifo_write(monitorId, output, size);
    return flag;
}

int BytestoRead(int size, int times, int buffer)
{
    int Bytes=size-times*buffer;
    if (Bytes > buffer){
        return buffer;
    }
    return Bytes;
}
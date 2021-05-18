# include "General.h"

int serialize_bloom(bloom filter, char * VirusName, int count, void ** output)
{
    int Bytes=filter.NumofBytes;
    int size=Bytes+3*sizeof(int)+strlen(VirusName);
    int Length=0;

    *output=(void *)calloc(size, sizeof(void));

    //Keeps the number of bloomfilters that the fifo is going to send.
    memcpy(*output+Length, &count, sizeof(int));

    //Keeps the number of bytes that the fifo is going to send.
    Length+=sizeof(int);
    memcpy(*output+Length, &size, sizeof(int));

    //Keeps the number of bytes that the filter.bits is.
    Length+=sizeof(int);
    memcpy(*output+Length, &Bytes, sizeof(int));

    //Keeps the name of virus that the bloom filter is used for.
    Length+=sizeof(int);
    memcpy(*output+Length, VirusName, strlen(VirusName));

    //Keeps the filter.bits.
    Length+=strlen(VirusName);
    memcpy(*output+Length, filter.bits, Bytes);

    return size;
}

bloom unserialize_bloom(void * input, char ** VirusName, int * count, int *size)
{
    int Length=0, filterbytes;
    
    bloom filter;
    // Number of bloomfilter that are send.
    memcpy(count, input+Length, sizeof(int));

    // Number of bytes that the fifo has send(bloomfilter + virusname + ints).
    Length+=sizeof(int);    
    memcpy(size, input+Length, sizeof(int));

    // Number of bytes that the filter.bits is going to be.
    Length+=sizeof(int);
    memcpy(&(filter.NumofBytes), input+Length, sizeof(int));

    // Calculation of the virusname length.
    int Namesize=*size-3*sizeof(int)-filter.NumofBytes;
    if (*size > buffer){
        filterbytes=buffer-3*sizeof(int)-Namesize;
    }
    else{
        filterbytes=filter.NumofBytes;
    }
    
    // VirusName.
    Length+=sizeof(int);
    *VirusName=(char *)calloc(Namesize+1, sizeof(char));
    memcpy(*VirusName, input+Length, Namesize);

    // filter.bits.
    Length+=Namesize;
    filter.bits=(char *)calloc(filterbytes, sizeof(char));
    memcpy(filter.bits, input+Length, filterbytes);

    return filter;
}

int receive_bloom(int id, Virus * Vlist)
{
    int size=0, fd, count=1;
    char * VirusName;
    for (int i = 0; i < count; i++){  // Loop as many time as bloomfilter has been send from the monitor.
        // Read the first buffer bytes of the fifo and the call helper.
        void * input =Fifo_read(id, &fd);
        receive_bloomHelper(id, Vlist, input, fd, &count);
        free(input);
    }

}

// This function is called from the receive_bloom after it has read the first buffer bytes.
// Calculates if there are going to be needed more read and executes them.
// At the end it receives the bloomfilter and enteres it to a Vlist node which is "held" by the travelmonitor.
int receive_bloomHelper(int id, Virus * Vlist, void * input, int fd, int * count)
{
    int size=0, i=1;
    char * VirusName;

    bloom bloomfilter = unserialize_bloom(input+size, &VirusName, count, &size); 
    
    int filterbytes=buffer-3*sizeof(int)-strlen(VirusName);
    char * filterbits=(char *)calloc(buffer, sizeof(char));

    // size has take the number of bytes from the unserialize_bloom.
    // Calculation of the number of loops that has to be done.
    int num= size/buffer; 
    if (size % buffer != 0){
        num+=1;
    }

    // The loop doesn't use the unserialize function because now we don't have the ints, only the bloomfilter.bits
    while (i < num){
        int readbytes=BytestoRead(size, i);
        if(read(fd, input, readbytes)<0){
            perror("read failed");
            close(fd);
            return -1;
        }

        memcpy(filterbits, input, readbytes);

        // Realloc the bloomfilter.bits and insert at the last allocated bytes the new bits that we have take from fifo.
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

int send_bloom(int monitorId, Virus * VTemp)
{
    int size=0, fd;
    int count=VirusCount(VTemp);

    VTemp=VTemp->Next;
    void * output;
    while (VTemp != NULL){ // Traverse the virus list and send all the bloomfilters that the monitor has.
        // Send all the bytes.
        size = serialize_bloom(VTemp->filter, (VTemp->VirusName), count, &output);
        int flag=Fifo_write(monitorId, output, size, &fd);
        if (flag < 0){
            return -1;
        }
        VTemp=VTemp->Next;
        free(output);
    }
    close(fd);
    free(VTemp); 
    return 0;
}

int BytestoRead(int size, int times)
{
    int Bytes=size-times*buffer;
    if (Bytes > buffer){
        return buffer;
    }
    return Bytes;
}

void * serialize_commands(char ** Array, int * Length)
{
    void * output=malloc(buffer*sizeof(void));
    *Length=0;
    // All the commands at max are at size of 6. Threrefore, I have made a loop that serializes at max 6 array items.
    // If there is a nullstring exit.
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
    // Working with the same logic as previous.
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
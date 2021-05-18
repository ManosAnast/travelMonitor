# include "bloomfilter.h"

bloom bloomInit(int NumOfBytes)
{
    bloom filter;
    filter.NumofBytes=NumOfBytes; filter.bits=(char *)calloc(NumOfBytes, sizeof(char));
    return filter;
}

void bloomSetBit(bloom * bloom, char * Id)
{   
    int num1, num2, num3;
    num1=djb2(Id)%(BloomNum*8); num2=sdbm(Id)%(BloomNum*8); num3=hash_i(Id, 3)%(BloomNum*8);

    /*Set the bit that djb2 returned*/
    int Byte=num1/8, x=1;
    int N=num1 - 8*Byte;
    int Number=bloom->bits[Byte];
    Number ^= (-x ^ Number) & (1UL << N); // Change of the Nth bit
    bloom->bits[Byte]=Number;
    
    /*Set the bit that sdbm returned*/
    Byte=num2/8; N=num2 - 8*Byte; Number=bloom->bits[Byte];
    Number ^= (-x ^ Number) & (1UL << N); // Change of the Nth bit
    bloom->bits[Byte]=Number;
    
    /*Set the bit that hash_i returned*/
    Byte=num3/8; N=num3 - 8*Byte; Number=bloom->bits[Byte];
    Number ^= (-x ^ Number) & (1UL << N); // Change of the Nth bit
    bloom->bits[Byte]=Number;
    return;
}

int bloomBitExist(bloom * bloom, char * Id)
{
    int num1, num2, num3;
    num1=djb2(Id)%(BloomNum*8); num2=sdbm(Id)%(BloomNum*8); num3=hash_i(Id, 3)%(BloomNum*8);
    int Byte=num1/8;
    int N=num1 - 8*Byte;
    int Number=bloom->bits[Byte];
    int bit1=(Number >> N) & 1U; // Save of the Nth bit
    
    Byte=num2/8; N=num2 - 8*Byte; Number=bloom->bits[Byte];
    int bit2=(Number >> N) & 1U; // Save of the Nth bit
    
    Byte=num3/8; N=num3 - 8*Byte; Number=bloom->bits[Byte];
    int bit3=(Number >> N) & 1U;
    return (bit1 && bit2 && bit3); // Save of the Nth bit
}

void bloomDestroy(bloom * bloom)
{
    free(bloom->bits);
    return;
}

/*
This algorithm (k=33) was first reported by dan bernstein many years 
ago in comp.lang.c. 
The magic of number 33 (why it works better than many other constants, 
prime or not) has never been adequately explained.
*/
unsigned long djb2(unsigned char *str) 
{
	unsigned long hash = 5381;
	int c; 
	while (c = *str++) {
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	}
	return hash;
}


/*
This algorithm was created for sdbm (a public-domain reimplementation of ndbm) 
database library. it was found to do well in scrambling bits, causing better 
distribution of the keys and fewer splits. it also happens to be a good 
general hashing function with good distribution. The actual function 
is hash(i) = hash(i - 1) * 65599 + str[i]; what is included below 
is the faster version used in gawk. There is even a faster, duff-device 
version. The magic constant 65599 was picked out of thin air while experimenting 
with different constants, and turns out to be a prime. this is one of the 
algorithms used in berkeley db (see sleepycat) and elsewhere.
*/
unsigned long sdbm(unsigned char *str) 
{
	unsigned long hash = 0;
	int c;

	while (c = *str++) {
		hash = c + (hash << 6) + (hash << 16) - hash;
	}

	return hash;
}


/* 
Return the result of the Kth hash funcation. This function uses djb2 and sdbm.
None of the functions used here is strong for cryptography purposes but they
are good enough for the purpose of the class.

The approach in this function is based on the paper:
https://www.eecs.harvard.edu/~michaelm/postscripts/rsa2008.pdf
*/
unsigned long hash_i(unsigned char *str, unsigned int i) 
{
	return djb2(str) + i*sdbm(str) + i*i;
}



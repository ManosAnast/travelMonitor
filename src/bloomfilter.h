# include "hash.h"

/* Struct that implements a bloom filter.
 * 
 * NumOfBytes: Number of bytes that the bloom filter has.
 * bits: array of bits.
 * 
*/
typedef struct bloomfilter{
    int NumofBytes;
    char * bits;
}bloom;
extern int BloomNum;

/*Initialize a new bloom filter
 *
 * NumofBytes: Number of bytes that you want it to have.
 * 
*/
bloom bloomInit(int NumofBytes);

/* Based on the Id, sets the correct bits of the bloom filter to 1.
 * If the bits that have to be changed are 0, it changes them to 1. Otherwise, it returns.
 *
 * bloom: Bloom filter.
 * Id: Id that we want to insert(type of string).
 *
*/
void bloomSetBit(bloom * bloom, char * Id);

/* Searches for the bits based on the Id.
 * If the bits are 1 it returns 1.
 * If the bits are 0 it returns 0.
 *
 * bloom: Bloom filter.
 * Id: Id that we want to insert(type of string).
 *
*/
int bloomBitExist(bloom * bloom, char * Id);

/* It frees the array of bits.
 *
 * bloom: Bloom filter.
 *
*/
void bloomDestroy(bloom * bloom);

/*--------------Hash functions that you gave.--------------*/

unsigned long djb2(unsigned char *str);

unsigned long sdbm(unsigned char *str);

unsigned long hash_i(unsigned char *str, unsigned int i);
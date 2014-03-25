#ifndef _SHA_H_
#define _SHA_H_


//#include <stdint.h>
#include <linux/types.h>

#ifndef _SHA_enum_
#define _SHA_enum_
/*
 *  All SHA functions return one of these values.
 */
enum {
    shaSuccess = 0,
    shaNull,            /* Null pointer parameter */
    shaInputTooLong,    /* input data too long */
    shaStateError,      /* called Input after FinalBits or Result */
    shaBadParam         /* passed a bad parameter */
};
#endif /* _SHA_enum_ */

/*
 *  These constants hold size information for each of the SHA
 *  hashing operations
 */
enum {
    
    SHA256_Message_Block_Size = 64,

    SHA256HashSize = 32,

    SHA256HashSizeBits = 256
};



#ifndef USE_MODIFIED_MACROS
#define SHA_Ch(x,y,z)        (((x) & (y)) ^ ((~(x)) & (z)))
#define SHA_Maj(x,y,z)       (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#else 
#define SHA_Ch(x, y, z)      (((x) & ((y) ^ (z))) ^ (z))
#define SHA_Maj(x, y, z)     (((x) & ((y) | (z))) | ((y) & (z)))
#endif 



/*
 *  This structure will hold context information for the SHA-256
 *  hashing operation.
 */
typedef struct SHA256Context {
    uint32_t Intermediate_Hash[SHA256HashSize/4]; /* Message Digest */

    uint32_t Length_Low;                /* Message length in bits */
    uint32_t Length_High;               /* Message length in bits */

    int Message_Block_Index;  /* Message_Block array index */
                                        /* 512-bit message blocks */
    uint8_t Message_Block[SHA256_Message_Block_Size];

    int Computed;                       /* Is the digest computed? */
    int Corrupted;                      /* Is the digest corrupted? */
} SHA256Context;


/* SHA-256 */
int SHA256Reset(SHA256Context *);
int SHA256Input(SHA256Context *, const uint8_t *bytes,
                       unsigned int bytecount);
int SHA256Result(SHA256Context *,
                        uint8_t Message_Digest[SHA256HashSize]);


#endif /* _SHA_H_ */

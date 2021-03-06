/*
    Mark Gill
    G00348787@gmit.ie
    this code was adapted from https://gist.github.com/creationix/4710780#file-md5-c-L4
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>

// function used to left rotate
#define LEFTROTATE(x, c) (((x) << (c)) | ((x) >> (32 - (c))))

// variables to hold vaues of the hash
uint32_t hash0, hash1, hash2, hash3;

// md5 takes the parameters of initial message and initial length
void md5(uint8_t *initial_message, size_t initial_length) {

    // initialising the empty message
    uint8_t *message = NULL;

    // r holds values of shifts in each round
    uint32_t r[] = {7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
                    5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
                    4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
                    6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};

    // pre-defined heash values obtained by: K[i] = floor(2^32 * abs(sin(i+1)))
    uint32_t k[] = {
        0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
        0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
        0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
        0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
        0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
        0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
        0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
        0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
        0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
        0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
        0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
        0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
        0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
        0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
        0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
        0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};

    // giving variables some values
    hash0 = 0x67452301;
    hash1 = 0xefcdab89;
    hash2 = 0x98badcfe;
    hash3 = 0x10325476;

    // appending "1" to input
    int new_length = ((((initial_length + 8) / 64) + 1) * 64) - 8;
 
    // calloc is a memory allocator
    // add "0" until congruent with 448, modulo 512
    message = calloc(new_length + 64, 1);

    // creating copy of memory block
    memcpy(message, initial_message, initial_length);

    // apending "1" to message
    message[initial_length] = 128;

    // adding 64 bit representation to equate to 512 bits
    uint32_t bits_length = 8*initial_length;
    memcpy(message + new_length, &bits_length, 4); 

    // put message in blocks, seperated every 512 bits
    int offset;
    for(offset=0; offset<new_length; offset += (512/8)) {
        uint32_t *w = (uint32_t *) (message + offset);

        uint32_t a = hash0;
        uint32_t b = hash1;
        uint32_t c = hash2;
        uint32_t d = hash3;
 
        uint32_t i;
        for(i = 0; i<64; i++) {
            uint32_t f, g;
 
            // using aux functions from RFC doc
             if (i < 16) {
                f = (b & c) | ((~b) & d);
                g = i;
            } else if (i < 32) {
                f = (d & b) | ((~d) & c);
                g = (5*i + 1) % 16;
            } else if (i < 48) {
                f = b ^ c ^ d;
                g = (3*i + 5) % 16;          
            } else {
                f = c ^ (b | (~d));
                g = (7*i) % 16;
            }

            // puts hold on d
            uint32_t temp = d;
            d = c;
            c = b;
            //printf("rotateLeft(%x + %x + %x + %x, %d)\n", a, f, k[i], w[g], r[i]);
            b = b + LEFTROTATE((a + f + k[i] + w[g]), r[i]);
            a = temp;
 
        }
        // adding hash to output
        hash0 += a;
        hash1 += b;
        hash2 += c;
        hash3 += d;
    }
    // cleanup
    free(message);

}

int main(int argcount, char **argv){

    uint8_t *hash;

    // messaged to be hashed will be the 3rd argument passed
    char *message = argv[2];
    size_t length = strlen(message);
        

    int i;
    while ((i = getopt (argcount, argv, ":s:t:h")) != -1){

        switch(i){
            case 's':  
                md5(message, length); 
                
                // display result
                printf("Your message is: %s\n", message);
            
                hash=(uint8_t *)&hash0;
                printf("%2.2x%2.2x%2.2x%2.2x", hash[0], hash[1], hash[2], hash[3], hash0);
            
                hash=(uint8_t *)&hash1;
                printf("%2.2x%2.2x%2.2x%2.2x", hash[0], hash[1], hash[2], hash[3], hash1);
            
                hash=(uint8_t *)&hash2;
                printf("%2.2x%2.2x%2.2x%2.2x", hash[0], hash[1], hash[2], hash[3], hash2);
            
                hash=(uint8_t *)&hash3;
                printf("%2.2x%2.2x%2.2x%2.2x", hash[0], hash[1], hash[2], hash[3], hash0);
                puts("");

                hash0 = 0x67452301;
                hash1 = 0xefcdab89;
                hash2 = 0x98badcfe;
                hash3 = 0x10325476;                
            break;
            case 't':

                /*
                    I had to hardcode the test value along with the length of the test value. I couln't get it to work with variables, unfortunately.
                */
                
                md5("", 0); 
            
                printf("Your message is: \nThe result should be: d41d8cd98f00b204e9800998ecf8427e\n");
                printf("Actual result: ");

                hash=(uint8_t *)&hash0;
                printf("%2.2x%2.2x%2.2x%2.2x", hash[0], hash[1], hash[2], hash[3], hash0);
            
                hash=(uint8_t *)&hash1;
                printf("%2.2x%2.2x%2.2x%2.2x", hash[0], hash[1], hash[2], hash[3], hash1);
            
                hash=(uint8_t *)&hash2;
                printf("%2.2x%2.2x%2.2x%2.2x", hash[0], hash[1], hash[2], hash[3], hash2);
            
                hash=(uint8_t *)&hash3;
                printf("%2.2x%2.2x%2.2x%2.2x", hash[0], hash[1], hash[2], hash[3], hash0);
                puts("");

                // resetting hash codes 
                hash0 = 0x67452301;
                hash1 = 0xefcdab89;
                hash2 = 0x98badcfe;
                hash3 = 0x10325476;

                printf("\n================================================================\n");


                md5("a", 1); 
            
                printf("Your message is: a\nThe result should be: 0cc175b9c0f1b6a831c399e269772661\n");
                printf("Actual result: ");


                hash=(uint8_t *)&hash0;
                printf("%2.2x%2.2x%2.2x%2.2x", hash[0], hash[1], hash[2], hash[3], hash0);
            
                hash=(uint8_t *)&hash1;
                printf("%2.2x%2.2x%2.2x%2.2x", hash[0], hash[1], hash[2], hash[3], hash1);
            
                hash=(uint8_t *)&hash2;
                printf("%2.2x%2.2x%2.2x%2.2x", hash[0], hash[1], hash[2], hash[3], hash2);
            
                hash=(uint8_t *)&hash3;
                printf("%2.2x%2.2x%2.2x%2.2x", hash[0], hash[1], hash[2], hash[3], hash0);
                puts("");

                hash0 = 0x67452301;
                hash1 = 0xefcdab89;
                hash2 = 0x98badcfe;
                hash3 = 0x10325476;

                printf("\n================================================================\n");


                md5("abc", 3); 
            
                printf("Your message is: abc\nThe result should be: 900150983cd24fb0d6963f7d28e17f72\n");
                printf("Actual result: ");

                hash=(uint8_t *)&hash0;
                printf("%2.2x%2.2x%2.2x%2.2x", hash[0], hash[1], hash[2], hash[3], hash0);
            
                hash=(uint8_t *)&hash1;
                printf("%2.2x%2.2x%2.2x%2.2x", hash[0], hash[1], hash[2], hash[3], hash1);
            
                hash=(uint8_t *)&hash2;
                printf("%2.2x%2.2x%2.2x%2.2x", hash[0], hash[1], hash[2], hash[3], hash2);
            
                hash=(uint8_t *)&hash3;
                printf("%2.2x%2.2x%2.2x%2.2x", hash[0], hash[1], hash[2], hash[3], hash0);
                puts("");

                hash0 = 0x67452301;
                hash1 = 0xefcdab89;
                hash2 = 0x98badcfe;
                hash3 = 0x10325476;

                printf("\n================================================================\n");


                md5("message digest", 14); 
            
                printf("Your message is: message digest\nThe result should be: f96b697d7cb7938d525a2f31aaf161d0\n");
                printf("Actual result: ");

                hash=(uint8_t *)&hash0;
                printf("%2.2x%2.2x%2.2x%2.2x", hash[0], hash[1], hash[2], hash[3], hash0);
            
                hash=(uint8_t *)&hash1;
                printf("%2.2x%2.2x%2.2x%2.2x", hash[0], hash[1], hash[2], hash[3], hash1);
            
                hash=(uint8_t *)&hash2;
                printf("%2.2x%2.2x%2.2x%2.2x", hash[0], hash[1], hash[2], hash[3], hash2);
            
                hash=(uint8_t *)&hash3;
                printf("%2.2x%2.2x%2.2x%2.2x", hash[0], hash[1], hash[2], hash[3], hash0);
                puts("");

                hash0 = 0x67452301;
                hash1 = 0xefcdab89;
                hash2 = 0x98badcfe;
                hash3 = 0x10325476;

                printf("\n================================================================\n");


                md5("abcdefghijklmnopqrstuvwxyz", 26); 
            
                printf("Your message is: abcdefghijklmnopqrstuvwxyz\nThe result should be: c3fcd3d76192e4007dfb496cca67e13b\n");
                printf("Actual result: ");

                hash=(uint8_t *)&hash0;
                printf("%2.2x%2.2x%2.2x%2.2x", hash[0], hash[1], hash[2], hash[3], hash0);
            
                hash=(uint8_t *)&hash1;
                printf("%2.2x%2.2x%2.2x%2.2x", hash[0], hash[1], hash[2], hash[3], hash1);
            
                hash=(uint8_t *)&hash2;
                printf("%2.2x%2.2x%2.2x%2.2x", hash[0], hash[1], hash[2], hash[3], hash2);
            
                hash=(uint8_t *)&hash3;
                printf("%2.2x%2.2x%2.2x%2.2x", hash[0], hash[1], hash[2], hash[3], hash0);
                puts("");

                hash0 = 0x67452301;
                hash1 = 0xefcdab89;
                hash2 = 0x98badcfe;
                hash3 = 0x10325476;

                printf("\n================================================================\n");

            break;
            case 'h':
                printf("-s 'string here'    run md5 hash on string\n");
                printf("-t     run tests\n");
                printf("-h     provides help\n");
                break;
        }
    }
    return 0;
}
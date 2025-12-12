#include<stdio.h>

void print_bin(void *vp, int size);

int main()
{
    printf("Hello world..\n");

    char ch = 10;                 // Example 1-byte variable
    print_bin(&ch, sizeof(ch));   // Print its binary

    unsigned int n1 = 0xFAFA;     // 4-byte variable
    print_bin(&n1, sizeof(n1));   // Print binary of each byte

    return 0;
}

/*
 * print_bin:
 * ----------
 * vp   : pointer to the variable whose binary we want to print
 * size : size (in bytes) of the variable
 *
 * Idea:
 * - Treat the variable as a sequence of bytes in memory.
 * - Start from the highest byte (size-1) → print down to byte 0.
 * - For every byte, use a mask (0x80 = 1000 0000) to test each bit.
 */
void print_bin(void *vp, int size)
{
    int i;

    // Loop from the most significant byte to the least.
    for(i = size - 1; i >= 0; i--)
    {
        unsigned char mask = 0x80;      // Start checking from MSB (bit-7)

        // Check all 8 bits of the selected byte
        while(mask)
        {
            /*
             * (char *)vp + i :
             *      - Cast vp to a byte pointer
             *      - Move to the i-th byte of the variable
             *
             * *((char *)vp + i) :
             *      - Read that byte value
             *
             * & mask :
             *      - Check whether the current bit is 1 or 0
             */
            if( *((char *)vp + i) & mask )
                printf("1");
            else
                printf("0");

            mask >>= 1;  // Move mask to next lower bit
        }

        printf(" ");     // Space between bytes
    }

    printf("\n");
}

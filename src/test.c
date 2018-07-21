#include "shared.h"
#include "snowflake.h"

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)       \
    (byte & 0x80 ? '1' : '0'),     \
        (byte & 0x40 ? '1' : '0'), \
        (byte & 0x20 ? '1' : '0'), \
        (byte & 0x10 ? '1' : '0'), \
        (byte & 0x08 ? '1' : '0'), \
        (byte & 0x04 ? '1' : '0'), \
        (byte & 0x02 ? '1' : '0'), \
        (byte & 0x01 ? '1' : '0')

time_t timeb_milliseconds_test(struct timeb time)
{
    // convert seconds component of timeb to milliseconds
    // append milliseconds component of timbe to total ms
    // then convert into milliseconds.
    return (time_t)(1000.0 * time.time + time.millitm);
}

void print_bits(void *p, size_t len)
{
    long int i;
    printf("(");
    for (i = len - 1; i >= 0; i--)
        printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(((unsigned char *)p)[i]));
    printf(")");
}

void print_uint64_bits(uint64_t x)
{
    print_bits(&x, sizeof(x));
}

int main()
{
    uint64_t snowflake;
    if (next_snowflake(&snowflake))
    {
        // error
        printf("failure");
    }
    else
    {
        // good!
        printf("%lli: ", snowflake);
        print_uint64_bits(snowflake);
    }
    return 0;
}
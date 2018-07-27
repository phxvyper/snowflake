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

#define MAX_WORKERID (2 << (SNOWFLAKE_WORKERID_BITS - 1))
#define SNOWFLAKE_SEQUENCE_MAX (2 << (SNOWFLAKE_SEQUENCE_BITS - 1))

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
    for (int wid = 0; wid < MAX_WORKERID; wid++)
    {
        for (int sid = 0; sid < SNOWFLAKE_SEQUENCE_MAX; sid++)
        {
            uint64_t snowflake;
            if (next_snowflake(wid, &snowflake))
            {
                // error
                printf("%4i.%3i: failed\n", wid, sid);
                return 1;
            }
            else
            {
                // good!
                printf("%4i.%3i: %lli ", wid, sid, snowflake);
                print_uint64_bits(snowflake);
                printf("\n");
            }
        }
    }
    
    return 0;
}

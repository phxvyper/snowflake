# Snowflake
Snowflakes are 64-bit unsigned integers that are guaranteed to be unique, as long as no more than
\f$(2^b)-1\f$ snowflakes are generated in a millisecond. If a snowflake is attempted to be organized
when this limit is reached, no snowflake will be generated.

The uniqueness of the snowflake is dependent on the worker_id being unique to all other workers when passed in.

## Building

```sh
$ ./build
```

## Usage

```c
#include "snowflake.h"
#include <stdlib.h>

int main(void)
{
    // example worker_id, should be unique in your implementation of 
    // workers/servers that create snowflakes.
    long int worker_id = 32;

    // initialize the snowflake state
    snowflake_init(worker_id);

    // obtain a snowflake
    uint64_t snowflake;
    if (next_snowflake(&snowflake))
    {
        // if a truthy value is returned, there was an error
        // and no snowflake is made.
    }

    printf("%lli", snowflake);

    return 0;
}
```

## Entropy
If the default values are used, then...

There are 256 (2^8) sequences that can be created every millisecond, with a maximum of 1024 (2^10) workers,
for a total of 262,144 (2^18) snowflakes every millisecond, along the timespan of (2^46) milliseconds, which
is approximately 2231.37824003 years.

If will achieve maximum entropy per millisecond by using a round-robin distribution of workers with a maximum
number of workers, resulting in an entropy of (2^18) per millisecond.


## The Snowflake ID
```
                 milliseconds since custom epoch       worker   sequence
|                                      143621749 |         26 |       37 |
| ---------------------------------------------- | ---------- | -------- |
| 0000000000000000001000100011110111111001110101 | 0000011010 | 00100101 |
| 63                                          18 | 17       8 | 7      0 |
```
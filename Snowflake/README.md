## Building

Requires gcc with support for C99.

```sh
$ ./build
```

The build script is very simple, as this implementation is very simple, so it should be a breeze to change to something like clang or msvc if desired.

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
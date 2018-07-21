#include "shared.h"
#include "snowflake.h"

#define SNOWFLAKE_SEQUENCE_MAX (SNOWFLAKE_SEQUENCE_BITS * SNOWFLAKE_SEQUENCE_BITS) - 1

struct _snowflake_state
{
    // time since unix epoch
    struct timeb time;
    // the default worker id
    long int worker_id;
    // sequence of the last snowflake made
    long int sequence;
} global_state;

time_t timeb_milliseconds(struct timeb time)
{
    // convert seconds component of timeb to milliseconds
    // append milliseconds component of timbe to total ms
    // then convert into milliseconds.
    return (time_t)(1000.0 * time.time + time.millitm);
}

uint64_t get_snowflake_id()
{
    uint64_t id = 0;

    // ensure we organize our snowflake to use our epoch, not the unix epoch.
    time_t milliseconds = timeb_milliseconds(global_state.time) - SNOWFLAKE_EPOCH;

    id = milliseconds << SNOWFLAKE_WORKERID_BITS;
    id = (id + global_state.worker_id) << SNOWFLAKE_SEQUENCE_BITS;
    id = (id + global_state.sequence);

    return id;
}

int next_snowflake(uint64_t *snowflake)
{
    struct timeb time;
    long int milliseconds;

    ftime(&time);
    milliseconds = timeb_milliseconds(time);

    // reset sequence if we're on a new timestamp or we've hit the max sequence value
    if (milliseconds != timeb_milliseconds(global_state.time))
    {
        global_state.sequence = 0;
    }
    else if (global_state.sequence == SNOWFLAKE_SEQUENCE_MAX)
    {
        // cannot create a UNIQUE snowflake, so we should fail.
        return 1;
    }

    global_state.time = time;

    (*snowflake) = get_snowflake_id();
    return 0;
}

int snowflake_init(long int worker_id)
{
    ftime(&global_state.time);
    global_state.worker_id = worker_id;
    global_state.sequence = SNOWFLAKE_SEQUENCE_MAX;

    return 0;
}
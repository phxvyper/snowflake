#include "shared.h"
#include "snowflake.h"

#define SNOWFLAKE_SEQUENCE_MAX (2 << (SNOWFLAKE_SEQUENCE_BITS - 1)) - 1
#define SNOWFLAKE_WORKER_MAX (2 << (SNOWFLAKE_WORKERID_BITS - 1)) - 1

struct _snowflake_state
{
    // time since unix epoch
    struct timeb time;
    // the default worker id
    long int worker_id;
    // sequence of the last snowflake made
    long int sequence;
    int first;
} states[SNOWFLAKE_WORKER_MAX];

time_t timeb_milliseconds(struct timeb time)
{
    // convert seconds component of timeb to milliseconds
    // append milliseconds component of timbe to total ms
    // then convert into milliseconds.
    return (time_t)(1000 * time.time + time.millitm);
}

uint64_t get_snowflake_id(unsigned int worker_id)
{
    uint64_t id = 0;

    // ensure we organize our snowflake to use our epoch, not the unix epoch.
    time_t milliseconds = timeb_milliseconds(states[worker_id].time) - SNOWFLAKE_EPOCH;

    id = milliseconds << SNOWFLAKE_WORKERID_BITS;
    id = (id + worker_id) << SNOWFLAKE_SEQUENCE_BITS;
    id = (id + states[worker_id].sequence);

    return id;
}

int next_snowflake(unsigned int worker_id, uint64_t *snowflake)
{
    struct timeb time;
    long int milliseconds;
    struct _snowflake_state state = states[worker_id];

    ftime(&time);
    milliseconds = timeb_milliseconds(time);

    // reset sequence if we're on a new timestamp or we've hit the max sequence value
    if (milliseconds != timeb_milliseconds(state.time))
    {
        state.sequence = 0;
        state.first = 1;
    }
    else
    {
        if (state.sequence == SNOWFLAKE_SEQUENCE_MAX)
        {
            // cannot create a UNIQUE snowflake, so we should fail.
            return 1;
        }

        if (state.first)
        {
            state.first = 0;
        }
        else
        {
            state.sequence++;
        }
    }

    state.time = time;
    states[worker_id] = state;

    (*snowflake) = get_snowflake_id(worker_id);
    return 0;
}

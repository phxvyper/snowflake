/**
 * @brief Implementation of snowflakes inspired by Twitter's Snowflake system, and later TwitterServer.
 * 
 * ## Snowflake
 * Snowflakes are 64-bit unsigned integers that are guaranteed to be unique, as long as no more than
 * \f(2^b)-1\f snowflakes are generated in a millisecond. If a snowflake is attempted to be organized
 * when this limit is reached, no snowflake will be generated.
 * 
 * The uniqueness of the snowflake is dependent on the worker_id being unique to all other workers when passed in.
 * 
 * ### Entropy
 * If the default values are used, then...
 * 
 * There are 256 (2^8) sequences that can be created every millisecond, with a maximum of 1024 (2^10) workers,
 * for a total of 262,144 (2^18) snowflakes every millisecond, along the timespan of (2^46) milliseconds, which
 * is approximately 2231.37824003 years.
 * 
 * If will achieve maximum entropy per millisecond by using a round-robin distribution of workers with a maximum
 * number of workers, resulting in an entropy of (2^18) per millisecond.
 * 
 * ### The Snowflake ID
 * 
 * ```
 *                  milliseconds since custom epoch       worker   sequence
 * |                                      143621749 |         26 |       37 |
 * | ---------------------------------------------- | ---------- | -------- |
 * | 0000000000000000001000100011110111111001110101 | 0000011010 | 00100101 |
 * | 63                                          18 | 17       8 | 7      0 |
 * ```
 * 
 * @file snowflake.h
 * @author Tristen Horton
 * @date 2018-07-20
 * @license MIT
 */

#ifndef SNOWFLAKE_H
#define SNOWFLAKE_H

#include "shared.h"

#ifdef __cplusplus
extern "C"
{
#endif

#if defined(_MSC_VER)
#define SF_EXPORT __declspec(dllexport)
#else
#define SF_EXPORT
#endif

/**
 * @brief The program's epoch
 * 
 * 1388534400000 milliseconds is Jan 1st, 2014 at 00:00:00 +0000 UTC
 */
#define SNOWFLAKE_EPOCH 1388534400000 // Midnight of January 1st, 2014
/**
 * @brief Bit width of snowflake's sequence segment
 */
#define SNOWFLAKE_SEQUENCE_BITS 8
/**
 * @brief Bit width of snowflake's workerid segment
 */
#define SNOWFLAKE_WORKERID_BITS 10
    /**
     * @brief Obtain a new snowflake at the current timestamp.
     * 
     * If a call to this function occurs on the same milisecond, the sequence will automatically increment.
     * If the snowflake generated is not unique, no snowflake will be made.
     * 
     * @param[out] snowflake the snowflake that was created
     * @return int 0 if the snowflake was created
     */
    SF_EXPORT int next_snowflake(uint64_t *snowflake);
    /**
     * @brief Initialize the snowflake state with a worker id.
     * 
     * @param worker_id The id associated with the worker that is generating snowflakes.
     * @return int 0 if the initialization is successful
     */
    SF_EXPORT int snowflake_init(long int worker_id);

#ifdef __cplusplus
}
#endif

#endif
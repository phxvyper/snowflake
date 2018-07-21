# Snowflake

Snowflake is a specification for a loosely defined unsigned integer with any width greater than 1 bit, where:
 - at least 1 bit represents the time since an epoch
 - at least 1 bit represents the 0-indexed count of snowflakes that have been made since the last time the time value has changed

For more details on what a snowflake is, see the [informal Snowflake standard.](https://github.com/phxvyper/snowflake/blob/master/STANDARD.md)

## These Snowflakes

In all of these implementations, Snowflakes are 64-bit unsigned integers that are guaranteed to be unique, as long as no more than
256 snowflakes are generated in a millisecond. If a snowflake is attempted to be organized
when this limit is reached, no snowflake will be generated.

The uniqueness of the snowflake is dependent on the worker_id being unique to all other workers when it is provided to the snowflake controller.

## Using

Please see the individual documentation for utilizing this specification's implementations in whatever language fits your needs:

 - [C](https://github.com/phxvyper/snowflake/tree/master/Snowflake)
 - [C#/.NET](https://github.com/phxvyper/snowflake/tree/master/Snowflake.NET)
 - [Python 3.5+](https://github.com/phxvyper/snowflake/tree/master/SnowflakePy)
 - More coming soon!

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
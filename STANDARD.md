# Information technology - The Snowflake unique identifier specification

## Foreward

This document is an informal ECMA/ISO-style specification for Snowflake. It has not been submitted to ISO for standardization, as of July 21st, 2018.

## Introduction

This document was developed in response to the rising adoption of systems of unique object identification numbers that are based off of a non-standardized model, known as a "snowflake". Middle line implementations of these models are inconsistent to eachother, and this document aims to bridge the gaps of inconsistency with a shared pattern.

## 1. Scope

Snowflake is a simple and intentionally loosely defined format for a categorized and sortable identifier. It was originally implemented in a social network to provide all messages on the platform with a unique and sortable identifier, but since has been used by other organizations and platforms for general identification of objects represented in a database or other backend. Snowflake defines a small set of rules about the structure of object identifiers, as an effort towards consistency and familiarity between different data management systems.

The goal of this specification is only to define the minimal viable structure of a Snowflake, as well as how it can be extended for more specific scenarios in data management. It intentionally defines only the most minimum requirements for a Snowflake to be valid, such that it may act as a common container for more specific categorization of objects in a data mangement system. Implementations and the representation of a Snowflake internal to an implementation are not defined in this specification, and the semantics of those implementations are not specified upon in this document.

## 2. Conformance

A conforming Snowflake value is a sequence of digital bits that conforms to the minimal requirements for a valid Snowflake as defined in this specification.

## 3. Snowflake

A Snowflake is a number of any width greater than 1 bit, where:
 1. at least 1 bit represents the time since an epoch as an integer, and
 2. at least 1 bit represents the 0-indexed count of snowflakes that have been created since the last time the time segment has changed
 3. each evaluated segment is continuous such that a segment's determined value can only be obtained from a continuous section of bits that is not split into by another segment's bits.

Additionally, the expected values within a Snowflake must exist in certain locations within the numeral's binary representation:
 1. the sequence segment must end at the HEAD of the numeral.
 2. the time segment must begin at the TRUNK of the numeral.

The uniqueness of a snowflake only pertains to the system in which it is being utilized in, and therefore should be unique to all other snowflakes in the same data system, but does not need to be unique with any snowflake from another data system it is not apart of.

### 3.1 Example

```r
# custom epoch is Jan 1st, 2014 at Midnight +0000 UTC
# in a 64-bit wide Snowflake model, with an additional 'worker' segment:
                 milliseconds since custom epoch       worker   sequence
|                                      143621749 |         26 |       37 |
| ---------------------------------------------- | ---------- | -------- |
| 0000000000000000001000100011110111111001110101 | 0000011010 | 00100101 |
| 63                                          18 | 17       8 | 7      0 |
```

where *HEAD* is at index 63 because it is the most signficant bit, and *TRUNK* is at index 0 because it is the least significant bit.

Note the additional, previously undefined, segment called 'worker'. This is permitted by this specification because this document only defines the *minimum* level of validity for a Snowflake.

### 3.2 Comparing Snowflakes

In comparisons between two Snowflakes of the same structure, there are some interesting implications:

| comparison | notes |
|  :------:  |  ---  |
|  `a == b`  | The snowflakes were created at the same time and have the same sequence, implying equality. |
|  `a > b`   | `a` could be newer than `b`. Inequality is implied. |

The assumption that `a` is newer than `b` when `a > b` is only accurate in systems where the Snowflake structure has a consistently direct and unqiue correlation to the time of creation. The default structure for a snowflake only contains data for time and sequence, and the sequence bits still correlate uniquely to the time of creation as they increment over time.

This assumption of `a` being newer than `b` when `a > b` continue to be *mostly true* in systems with high snowflake creation rates, but low rate of comparisons between snowflakes that are very near eachother.

Snowflakes are useful for quickly sorting, but if a true sort is desired then the components of the Snowflake must be disassembled.

## 4. Epoch Times

An epoch time is a system-specific offset from the Unix Epoch Time. Every implementation of Snowflake must have a single Epoch time, but that eopch time does not have to be constant and can be any offset from the Unix Epoch Time.

The implementation's epoch time must be an integer.

The time segment of a Snowflake is the amount of time that has passed since the implementation's epoch time, at the moment the Snowflake was created.

## 5. Sequences

A sequence is a counting value. When a new Snowflake is created, and the time segment of the Snowflake hasn't changed since the last time a Snowflake was created, the sequence segment should be incremented once. If the time segment has changed, then the sequence segment is reset to 0. If the number of iterations made exceeds the max value of the sequence segment, then the Snowflake should fail to be generated.

## 6. Implementation Dependent

There are various attributes about a Snowflake that are dependent upon the implementation that is deriving their use from this specification. Most notably are:

1. the width of the numeral.
    - this can be 2 bits, or 200 bits. Its up to the implementation.
    - it still mustbe at least 2 bits, however.

2. extra segments.
    - the implementation may provide additional segments in between the TRUNK sequence segment and the HEAD time segment, with no limit.

3. the units of measurement for time.
    - the time segment can be anywhere from nanoseconds to years, or any other unit of time - even nonstandardized units.
    - the implementation must still base its epoch time off of the Unix Epoch Time.

using System;
using System.Diagnostics;
using System.Runtime.InteropServices;

namespace Snowflake
{
    /// <summary>
    /// An instance of a worker that generates snowflakes.
    /// </summary>
    public class SnowflakeWorker
    {
        private readonly ulong _epoch = (ulong)new DateTimeOffset(2014, 1, 1, 0, 0, 0, 0, TimeSpan.Zero).ToUnixTimeMilliseconds();
        private const int _sequence_bits = 8;
        private const int _workerid_bits = 10;
        private const ulong _max_sequence_value = (2 << (_sequence_bits - 1)) - 1;

        private ulong _lastTime;
        private ulong _workerId;
        private ulong _sequence;

        private bool _first = true;

        /// <summary>
        /// Construct and initialize the state of the snowflake worker.
        /// </summary>
        /// <param name="workerId">The ID to assign to this worker. Should be as unique as possible within a close system of workers.</param>
        public SnowflakeWorker(ulong workerId)
        {
            _lastTime = (ulong)DateTimeOffset.UtcNow.ToUnixTimeMilliseconds();
            _workerId = workerId;
            _sequence = 0;
        }

        /// <summary>
        /// Try to generate a new snowflake.
        /// </summary>
        /// <param name="snowflake">The snowflake generated, or 0 if no snowflake was generated.</param>
        /// <returns><see cref="SnowflakeError.None"/> if a snowflake was generated. <see cref="SnowflakeError.NotUnique"/> if it couldnt generate a unique snowflake.</returns>
        public SnowflakeError Next(out ulong snowflake)
        {
            var newTime = (ulong)DateTimeOffset.UtcNow.ToUnixTimeMilliseconds();

            if (newTime != _lastTime)
            {
                Console.WriteLine("Theyre different!");
                _sequence = 0;
                _lastTime = newTime;
                _first = true;
            }
            else
            {
                if (_sequence == _max_sequence_value)
                {
                    snowflake = 0;
                    return SnowflakeError.NotUnique;
                }

                if (_first)
                {
                    _first = false;
                }
                else
                {
                    _sequence++;
                }
            }

            snowflake = GetSnowflake();
            return SnowflakeError.None;
        }

        private ulong GetSnowflake()
        {
            var milliseconds = _lastTime - _epoch;
            Console.WriteLine(milliseconds);

            var id = milliseconds << _workerid_bits;
            id = (id + _workerId) << _sequence_bits;
            return id + _sequence;
        }
    }

    public enum SnowflakeError
    {
        None = 0,
        NotUnique = 1,
    }
}

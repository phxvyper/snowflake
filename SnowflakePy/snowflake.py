from ctypes import windll, CDLL, byref, c_ulonglong
import os

if os.name == "nt":
    snowflake_native = windll.LoadLibrary("./snowflake.so")
else:
    snowflake_native = CDLL("./snowflake.so")


def snowflake_init(workerid):
    snowflake_native.snowflake_init(workerid)


def next_snowflake():
    snowflake = c_ulonglong()
    status = snowflake_native.next_snowflake(byref(snowflake))

    return snowflake.value if status == 0 else None

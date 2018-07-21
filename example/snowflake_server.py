from flask import Flask, Response
from ctypes import windll, CDLL, byref, c_ulonglong
import os

APP = Flask(__name__)

snowflakes = []
workerid = 0

if os.name == "nt":
    snowflake_native = windll.LoadLibrary("./snowflake.so")
else:
    snowflake_native = CDLL("./snowflake.so")

snowflake_native.snowflake_init(workerid)


def next_snowflake():
    snowflake = c_ulonglong()
    success = snowflake_native.next_snowflake(byref(snowflake))

    return snowflake.value if success == 0 else None


@APP.route("/snowflake", methods=["POST"])
def post_snowflake():
    snowflake = next_snowflake()
    snowflakes.append(snowflake)

    return Response(str(snowflake), status=200)


@APP.route("/snowflake/<string:snowflake>", methods=["GET"])
def snowflake_verify(snowflake):
    if snowflake in snowflakes:
        return Response("OK", status=200)
    else:
        return Response("Not Found", status=404)

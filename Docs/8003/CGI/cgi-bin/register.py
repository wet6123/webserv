#!/usr/bin/env python3

import cgi
import os
from http import HTTPStatus
from database import register_user

def register():
    form = cgi.FieldStorage()
    username = form.getvalue('username')
    password = form.getvalue('password')

    if register_user(username, password):
        response_body="<html><body><script>\
            alert(\"Register Success!\");\
            window.location.replace(\"/main/login\");\
            </script>Registration successful!</body></html>"

        print(f"HTTP/1.1 200 OK")
        print(f"Connection: {os.environ.get("HTTP_CONNECTION", os.environ.get("CONNECTION", "close"))}")
        print(f"Content-Length: {len(response_body)}")
        print("Content-type: text/html\r\n\r\n")
        print(response_body)
    else:
        response_body = "<html><body><script>\
            alert(\"Register Fail!\");\
            window.location.replace(\"/main/register\");\
            </script>Register Fail!</body></html>"
        print(f"HTTP/1.1 200 OK")
        print(f"Connection: {os.environ.get("HTTP_CONNECTION", os.environ.get("CONNECTION", "close"))}")
        print(f"Content-Length: {len(response_body)}")
        print("Content-type: text/html\r\n\r\n")
        print(response_body)
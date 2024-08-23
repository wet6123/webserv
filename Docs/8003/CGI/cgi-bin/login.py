#!/usr/bin/env python3

import cgi
import uuid
import os
from http import HTTPStatus
from database import authenticate_user

def login_user():
    form = cgi.FieldStorage()
    username = form.getvalue('username')
    password = form.getvalue('password')
    session_id = uuid.uuid4();

    if authenticate_user(username, password):
        response_body = f"<html><body><script>\
            alert(\"Welcome, {username}!\");\
            window.location.replace(\"/main\");\
            </script></body></html>"
        # 로그인 성공, 세션 쿠키 설정
        print(f"HTTP/1.1 200 OK")
        print(f"Connection: {os.environ.get("HTTP_CONNECTION", os.environ.get("CONNECTION", "close"))}")
        print(f"Content-Length: {len(response_body)}")
        print(f"Set-Cookie: session_id={session_id}; HttpOnly")
        print(f"Set-Cookie: username={username}; HttpOnly")
        print("Content-type: text/html\r\n\r\n")
        print(response_body)
    else:
        response_body = "<html><body><script>\
            alert(\"Invalid credentials!\");\
            window.location.replace(\"/main/login\");\
            </script>Invalid credentials!</body></html>"
        print(f"HTTP/1.1 {HTTPStatus.UNAUTHORIZED} Unauthorized")
        print(f"Connection: {os.environ.get("HTTP_CONNECTION", os.environ.get("CONNECTION", "close"))}")
        print(f"Content-Length: {len(response_body)}")
        print("Content-type: text/html\r\n\r\n")
        print(response_body)
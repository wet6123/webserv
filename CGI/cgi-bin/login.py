#!/usr/bin/env python3

import cgi
import uuid
from database import authenticate_user

form = cgi.FieldStorage()
username = form.getvalue('username')
password = form.getvalue('password')
session_id = uuid.uuid4();

if authenticate_user(username, password):
    # 로그인 성공, 세션 쿠키 설정
    print(f"Set-Cookie: session_id={session_id}; HttpOnly")
    print(f"Set-Cookie: username={username}; HttpOnly")
    print("Content-type: text/html\r\n\r\n")
    print(f"<html><body>Welcome, {username}!</body></html>")
else:
    print("Content-type: text/html\r\n\r\n")
    print("<html><body>Invalid credentials!</body></html>")
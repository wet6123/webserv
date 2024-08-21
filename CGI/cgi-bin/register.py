#!/usr/bin/env python3

import cgi
from database import register_user

form = cgi.FieldStorage()
username = form.getvalue('username')
password = form.getvalue('password')

if register_user(username, password):
    print("Content-type: text/html\r\n\r\n")
    print("<html><body>Registration successful!</body></html>")
else:
    print("Content-type: text/html\r\n\r\n")
    print("<html><body>Username already exists!</body></html>")
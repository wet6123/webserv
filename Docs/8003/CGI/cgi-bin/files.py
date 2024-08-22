#!/usr/bin/env python3

import os
import cgi
import cgitb
import sys
from upload import get_user_upload_dir
from http import HTTPStatus

# Enable debugging
cgitb.enable()

UPLOAD_BASE_DIR = "/Users/iyeonjae/Documents/GitHub/webserv/CGI/uploads"

def list_files(username):
    user_dir = get_user_upload_dir(username)
    files = os.listdir(user_dir)
    
    response_body = "<html><body><h1>Uploaded Files</h1><ul>"
    
    for file in files:
        response_body += f"""
            <li>
                {file}
                <a href="/cgi-bin/main.py/list_files?download={file}">Download</a>
                <a href="/cgi-bin/main.py/list_files?delete={file}">Delete</a>
            </li>
        """
    
    response_body += "</ul></body></html>"
    
    return response_body, HTTPStatus.OK

def download_file(username, filename):
    user_dir = get_user_upload_dir(username)
    file_path = os.path.join(user_dir, filename)
    
    try:
        with open(file_path, 'rb') as file:
            content = file.read()
        
        # Set headers for file download
        sys.stdout.buffer.write(f"Content-Type: application/octet-stream\r\n".encode())
        sys.stdout.buffer.write(f"Content-Disposition: attachment; filename=\"{filename}\"\r\n".encode())
        sys.stdout.buffer.write(f"Content-Length: {len(content)}\r\n".encode())
        sys.stdout.buffer.write(b"\r\n")
        sys.stdout.buffer.write(content)
    except Exception as e:
        sys.stdout.buffer.write("Content-Type: text/html\r\n".encode())
        sys.stdout.buffer.write("Status: 500 Internal Server Error\r\n".encode())
        sys.stdout.buffer.write(b"\r\n")
        sys.stdout.buffer.write(f"<html><body><h1>Internal Server Errorssss</h1><p>{e}</p></body></html>".encode())
    sys.stdout.flush()
    sys.exit(0)

def delete_file(username, filename):
    user_dir = get_user_upload_dir(username)
    file_path = os.path.join(user_dir, filename)
    os.remove(file_path)
    
    sys.stdout.buffer.write("Status: 303 See Other\r\n".encode())
    sys.stdout.buffer.write("Location: /cgi-bin/main.py/list_files\r\n".encode())
    sys.stdout.buffer.write(b"\r\n")
    sys.stdout.flush()
    sys.exit(0)

if __name__ == "__main__":
    form = cgi.FieldStorage()
    
    if "download" in form:
        download_file(form.getvalue("download"))
    elif "delete" in form:
        delete_file(form.getvalue("delete"))
    else:
        response_body, status_code = list_files()
        sys.stdout.buffer.write("Content-Type: text/html\r\n".encode())
        sys.stdout.buffer.write(f"Status: {status_code.value} {status_code.phrase}\r\n".encode())
        sys.stdout.buffer.write(b"\r\n")
        sys.stdout.buffer.write(response_body.encode())
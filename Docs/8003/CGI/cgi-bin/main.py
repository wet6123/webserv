#!/usr/bin/env python3

import os
import cgi
import cgitb
import sys
import datetime
from html import escape
from http import cookies, HTTPStatus
from files import list_files, download_file, delete_file, download
from upload import upload_file
from database import load_users
from urllib.parse import parse_qs
from login import login_user
from register import register

# Enable debugging
cgitb.enable()

# Get CGI environment variables
def get_env():
    request_method = os.environ.get("REQUEST_METHOD", "GET")
    query_string = os.environ.get("QUERY_STRING", "")
    content_type = os.environ.get("CONTENT_TYPE", "")
    content_length = os.environ.get("CONTENT_LENGTH", "0")
    cookie_string = os.environ.get("HTTP_COOKIE", "")
    return request_method, query_string, content_type, content_length, cookie_string

def print_response(response_body, status_code=HTTPStatus.OK, content_type='text/html', cookie_string=None):
    # Print the HTTP header
    print(f"HTTP/1.1 {status_code.value} {status_code.phrase}")
    print(f"Connection: {os.environ.get("HTTP_CONNECTION", os.environ.get("CONNECTION", "close"))}")
    print(f"Content-Disposition: inline")
    print(f"Content-Length: {len(response_body)}")
    print(f"Content-Type: {content_type}")
    if cookie_string:
        for cookie in cookie_string:
            print(f"Set-Cookie: {cookie}")
    now = datetime.datetime.now(datetime.timezone.utc)
    print(f"Date: {now.strftime("%a, %d %b %Y %H:%M:%S %Z")}")
    print(f"Server: {os.environ.get("SERVER_SOFTWARE", "Webserv")}")
    print()
    print(response_body)

def load_html(file_path):
    try:
        with open(file_path, 'r') as file:
            return file.read(), HTTPStatus.OK
    except Exception as e:
        return f"<html><body><h1>Error loading {file_path}</h1><p>{e}</p></body></html>", HTTPStatus.INTERNAL_SERVER_ERROR

def handle_dark_mode(cookie_string, query_params):
    dark_mode = query_params.get('dark_mode', [None])[0]
    cookie_set = []

    if dark_mode == "toggle":
        if "dark_mode=on" in cookie_string:
            cookie_set.append("dark_mode=off; Path=/; Max-Age=31536000")
        else:
            cookie_set.append("dark_mode=on; Path=/; Max-Age=31536000")

    return cookie_set


def get_logged_in_user(cookie_string):
    # Parse cookies
    cookie = cookies.SimpleCookie(cookie_string)

    # Check if the session ID exists
    if 'username' in cookie:
        username = cookie['username'].value
        return username
    
    return None


def main():
    try:
        request_method, query_string, content_type, content_length, cookie_string = get_env()
        query_params = parse_qs(query_string)

        # Get the PATH_INFO environment variable
        path_info = os.environ.get("PATH_INFO", "")

        # Absolute paths for HTML files
        base_dir = os.path.dirname(os.path.abspath(__file__))
        login_html_path = os.path.join(base_dir, '../html/login.html')
        register_html_path = os.path.join(base_dir, '../html/register.html')
        calc_html_path = os.path.join(base_dir, '../html/calc.html')
        upload_html_path = os.path.join(base_dir, '../html/upload.html')
        notfound_html_path = os.path.join(base_dir, '../html/404.html')

        # Default status code
        status_code = HTTPStatus.OK

        # Verify if a user is logged in
        username = get_logged_in_user(cookie_string)

        # Handle dark mode toggle
        cookie_set = handle_dark_mode(cookie_string, query_params)

        if request_method == "POST":
            if path_info == "/upload":
                response_body, status_code = upload_file(username)
            elif path_info == "/login":
                login_user()
            elif path_info == "/register":
                register()
            elif path_info == "/files":
                download()
            else:
                response_body, status_code = load_html(notfound_html_path)
                status_code = HTTPStatus.NOT_FOUND
        else:
            if path_info == "/login":
                response_body, status_code = load_html(login_html_path)
            elif path_info == "/register":
                response_body, status_code = load_html(register_html_path)
            elif path_info == "/calculator":
                response_body, status_code = load_html(calc_html_path)
            elif path_info == "/upload":
                if username is not None:
                    response_body, status_code = load_html(upload_html_path)
                else:
                    response_body, status_code = load_html(login_html_path)
            elif "download" in query_string:
                if username is not None:
                    filename = parse_qs(query_string).get("download", [None])[0]
                    if filename:
                        download_file(username, filename)
                        return  # download_file 함수 내부에서 응답을 이미 처리합니다.
                else:
                    response_body, status_code = load_html(login_html_path)
            elif "delete" in query_string:
                if username is not None:
                    filename = parse_qs(query_string).get("delete", [None])[0]
                    if filename:
                        delete_file(username, filename)
                        return  # delete_file 함수 내부에서 응답을 이미 처리합니다.
                else:
                    response_body, status_code = load_html(login_html_path)
            elif path_info == "/files":
                if username is not None:
                    response_body, status_code = list_files(username)
                else:
                    response_body, status_code = load_html(login_html_path)
            elif path_info == "":
                response_body = "<html><body><h1>Hello, CGI!</h1>\
                    <div><button type=\"button\" onclick=\"location.href='/main/login'\">Login</button></div>\
                    <div><button type=\"button\" onclick=\"location.href='/main/register'\">Register</button></div>\
                    <div><button type=\"button\" onclick=\"location.href='/main/calculator'\">Calculator</button></div>\
                    <div><button type=\"button\" onclick=\"location.href='/main/upload'\">Upload</button></div>\
                    <div><button type=\"button\" onclick=\"location.href='/main/files'\">Download</button></div>\
                    <p>{}</p></body></html>".format(escape(query_string))
                status_code = HTTPStatus.OK
            else:
                response_body, status_code = load_html(notfound_html_path)
                status_code = HTTPStatus.NOT_FOUND


        print_response(response_body, status_code, cookie_string=cookie_set)

    except Exception as e:
        sys.stderr.write(f"Error: {e}")
        response_body = f"<html><body><h1>Internal Server Error11111</h1><p>{e}</p></body></html>"
        print_response(response_body, HTTPStatus.INTERNAL_SERVER_ERROR)

if __name__ == "__main__":
    main()
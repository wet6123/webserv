#!/usr/bin/env python3

from html import escape
import os
import cgitb
import sys
from upload import upload_file
from http import HTTPStatus

# Enable debugging
cgitb.enable()

# Get CGI environment variables
def get_env():
    request_method = os.environ.get("REQUEST_METHOD", "GET")
    query_string = os.environ.get("QUERY_STRING", "")
    content_type = os.environ.get("CONTENT_TYPE", "")
    content_length = os.environ.get("CONTENT_LENGTH", "0")
    return request_method, query_string, content_type, content_length

def print_response(response_body, status_code):
    # Print the HTTP header
    print(f"Status: {status_code.value} {status_code.phrase}")
    # Set the content type
    print("Content-Type: text/html")
    print()
    print(response_body)

def load_html(file_path):
    try:
        with open(file_path, 'r') as file:
            return file.read()
    except Exception as e:
        return f"<html><body><h1>Error loading {file_path}</h1><p>{e}</p></body></html>"

def main():
    request_method, query_string, content_type, content_length = get_env()

    # Get the PATH_INFO environment variable
    path_info = os.environ.get("PATH_INFO", "")

    # Absolute paths for HTML files
    base_dir = os.path.dirname(os.path.abspath(__file__))
    calc_html_path = os.path.join(base_dir, '../html/calc.html')
    upload_html_path = os.path.join(base_dir, '../html/upload.html')

    # Default status code
    status_code = HTTPStatus.OK

    # POST 데이터 처리
    if request_method == "POST":
        if path_info == "/upload":
            response_body, status_code = upload_file()
        else:
            content_length = int(content_length)
            post_data = sys.stdin.read(content_length)
            response_body = "<html><body><h1>POST Data</h1><pre>{}</pre></body></html>".format(escape(post_data))
            status_code = HTTPStatus.OK
    else:
        if path_info == "/calculator":
            response_body = load_html(calc_html_path)
        elif path_info == "/upload":
            response_body = load_html(upload_html_path)
        else:
            response_body = "<html><body><h1>Hello, CGI!</h1><p>{}</p></body></html>".format(escape(query_string))
            status_code = HTTPStatus.OK

    print_response(response_body, status_code)

if __name__ == "__main__":
    main()
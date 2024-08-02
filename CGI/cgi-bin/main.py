#!/usr/bin/env python3

from html import escape
import os
import cgi
import cgitb
import sys
from upload import upload_file
from files import list_files, download_file, delete_file
from http import HTTPStatus
from urllib.parse import parse_qs

# Enable debugging
cgitb.enable()

# Get CGI environment variables
def get_env():
    request_method = os.environ.get("REQUEST_METHOD", "GET")
    query_string = os.environ.get("QUERY_STRING", "")
    content_type = os.environ.get("CONTENT_TYPE", "")
    content_length = os.environ.get("CONTENT_LENGTH", "0")
    return request_method, query_string, content_type, content_length

def print_response(response_body, status_code=HTTPStatus.OK, content_type='text/html'):
    # Print the HTTP header
    print(f"Status: {status_code.value} {status_code.phrase}")
    print(f"Content-Type: {content_type}")
    print()
    print(response_body)

def load_html(file_path):
    try:
        with open(file_path, 'r') as file:
            return file.read(), HTTPStatus.OK
    except Exception as e:
        return f"<html><body><h1>Error loading {file_path}</h1><p>{e}</p></body></html>", HTTPStatus.INTERNAL_SERVER_ERROR

def main():
    try:
        request_method, query_string, content_type, content_length = get_env()

        # Get the PATH_INFO environment variable
        path_info = os.environ.get("PATH_INFO", "")

        # Absolute paths for HTML files
        base_dir = os.path.dirname(os.path.abspath(__file__))
        calc_html_path = os.path.join(base_dir, '../html/calc.html')
        upload_html_path = os.path.join(base_dir, '../html/upload.html')

        # Default status code
        status_code = HTTPStatus.OK

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
                response_body, status_code = load_html(calc_html_path)
            elif path_info == "/upload":
                response_body, status_code = load_html(upload_html_path)
            elif "download" in query_string:
                file_name = parse_qs(query_string).get("download", [None])[0]
                if file_name:
                    download_file(file_name)
                    return  # download_file 함수 내부에서 응답을 이미 처리합니다.
            elif "delete" in query_string:
                file_name = parse_qs(query_string).get("delete", [None])[0]
                if file_name:
                    delete_file(file_name)
                    return  # delete_file 함수 내부에서 응답을 이미 처리합니다.
            elif path_info == "/list_files":
                response_body, status_code = list_files()
            else:
                response_body = "<html><body><h1>Hello, CGI!</h1><p>{}</p></body></html>".format(escape(query_string))
                status_code = HTTPStatus.OK

        print_response(response_body, status_code)

    except Exception as e:
        response_body = f"<html><body><h1>Internal Server Error</h1><p>{e}</p></body></html>"
        print_response(response_body, HTTPStatus.INTERNAL_SERVER_ERROR)

if __name__ == "__main__":
    main()
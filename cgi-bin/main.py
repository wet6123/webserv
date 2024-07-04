#!/usr/bin/env python3

import os
import cgi
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

def main():
    request_method, query_string, content_type, content_length = get_env()

    # Default status code
    status_code = HTTPStatus.OK

    # POST 데이터 처리
    if request_method == "POST":
        # TODO: PATH_INFO 환경 변수를 사용해 요청 경로를 확인
        if "multipart/form-data" in content_type:
            response_body, status_code = upload_file()
        else:
            content_length = int(content_length)
            post_data = sys.stdin.read(content_length)
            response_body = "<html><body><h1>POST Data</h1><pre>{}</pre></body></html>".format(cgi.escape(post_data))
            status_code = HTTPStatus.OK
    else:
        response_body = "<html><body><h1>Hello, CGI!</h1><p>{}</p></body></html>".format(cgi.escape(query_string))
        status_code = HTTPStatus.OK

    print_response(response_body, status_code)

if __name__ == "__main__":
    main()
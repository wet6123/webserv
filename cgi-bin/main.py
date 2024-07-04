#! /usr/bin/env python3

import os
import cgi
import cgitb
import sys
from upload import upload_file

# Enable debugging
cgitb.enable()

# Get CGI environment variables
def get_env():
	request_method = os.environ.get("REQUEST_METHOD", "GET")
	query_string = os.environ.get("QUERY_STRING", "")
	content_type = os.environ.get("CONTENT_TYPE", "")
	content_length = os.environ.get("CONTENT_LENGTH", "0")
	return request_method, query_string, content_type, content_length

def main():
	# Set the content type
	print("HTTP/1.1 200 OK")
	print("Content-Type: text/html")
	print()

	request_method, query_string, content_type, content_length = get_env()

	# POST 데이터 처리
	if request_method == "POST":
		# TODO: PATH_INFO 환경 변수를 사용해 요청 경로를 확인
		if ("multipart/form-data" in content_type):
			upload_file()
		content_length = int(content_length)
		post_data = sys.stdin.read(content_length)
		print("<html><body>")
		print("<h1>POST Data</h1>")
		print("<pre>{}</pre>".format(cgi.escape(post_data)))
		print("</body></html>")
	# GET 요청 처리
	else:
		print("<html><body>")
		print("<h1>Hello, CGI!</h1>")
		print("<p>{}</p>".format(cgi.escape(query_string)))
		print("</body></html>")

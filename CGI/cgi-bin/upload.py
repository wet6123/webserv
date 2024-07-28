#!/usr/bin/env python3

import cgi
from http import HTTPStatus
import os
import sys

UPLOAD_DIR = "/Users/iyeonjae/Documents/GitHub/webserv/CGI/uploads"

def increment_filename(filename):
    base, ext = os.path.splitext(filename)
    counter = 1
    result = filename

    while os.path.exists(os.path.join(UPLOAD_DIR, result)):
        result = f"{base}({counter}){ext}"
        counter += 1
    
    return result


def upload_file():
    # FieldStorage instance
    form = cgi.FieldStorage()

    # Get the file item
    file_item = form['file']

    # Check if the file was uploaded
    if file_item.filename:
        # Secure the file path
        if not os.path.exists(UPLOAD_DIR):
            os.makedirs(UPLOAD_DIR)
        
        # Ensure the file path is safe
        original_filename = os.path.basename(file_item.filename)
        new_filename = increment_filename(original_filename)
        file_path = os.path.join(UPLOAD_DIR, new_filename)

        try:
            # Write the file to the specified location
            with open(file_path, 'wb') as f:
                f.write(file_item.file.read())

            # HTML response for successful upload
            response_body = f"<html><body><h2>File {original_filename} uploaded successfully as {new_filename}.</h2></body></html>"
            status_code = HTTPStatus.OK
        except Exception as e:
            response_body = f"<html><body><h2>Error uploading file: {e}</h2></body></html>"
            # Internal server error status code
            status_code = HTTPStatus.INTERNAL_SERVER_ERROR
    else:
        response_body = "<html><body><h2>No file was uploaded.</h2></body></html>"
        # Bad request status code
        status_code = HTTPStatus.BAD_REQUEST
    return response_body, status_code
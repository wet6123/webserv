#!/usr/bin/env python3

import cgi
from http import HTTPStatus
import os
import sys

def upload_file():
    # FieldStorage instance
    form = cgi.FieldStorage()

    # Get the file item
    file_item = form['file']

    # Check if the file was uploaded
    if file_item.filename:
        # Secure the file path
        upload_dir = "/PATH/TO/UPLOAD/DIRECTORY"
        if not os.path.exists(upload_dir):
            os.makedirs(upload_dir)
        
        # Ensure the file path is safe
        filename = os.path.basename(file_item.filename)
        file_path = os.path.join(upload_dir, filename)

        try:
            # Write the file to the specified location
            with open(file_path, 'wb') as f:
                f.write(file_item.file.read())

            # HTML response for successful upload
            response_body = f"<html><body><h2>File {filename} uploaded successfully.</h2></body></html>"
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
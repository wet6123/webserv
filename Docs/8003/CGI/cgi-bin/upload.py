#!/usr/bin/env python3

import cgi
from http import HTTPStatus
import os
import sys
import hashlib

UPLOAD_BASE_DIR = os.path.join(os.path.dirname(__file__), "uploads")

def get_user_upload_dir(username):
    user_hash = hashlib.sha256(username.encode()).hexdigest()
    return os.path.join(UPLOAD_BASE_DIR, user_hash)


def ensure_user_upload_dir(username):
    user_dir = get_user_upload_dir(username)
    if not os.path.exists(user_dir):
        os.makedirs(user_dir)
    return user_dir


def increment_filename(filename, user_dir):
    base, ext = os.path.splitext(filename)
    counter = 1
    result = filename

    while os.path.exists(os.path.join(user_dir, result)):
        result = f"{base}({counter}){ext}"
        counter += 1
    
    return result


def upload_file(username):
    sys.stderr.write(UPLOAD_BASE_DIR)
    # FieldStorage instance
    form = cgi.FieldStorage()
    sys.stderr.write(str(form))

    # Get the file item
    file_item = form['file']

    # Check if the file was uploaded
    if file_item.filename:
        # Secure the file path
        if not os.path.exists(UPLOAD_BASE_DIR):
            os.makedirs(UPLOAD_BASE_DIR)
        
        # Ensure the file path is safe
        user_dir = ensure_user_upload_dir(username)
        original_filename = os.path.basename(file_item.filename)
        new_filename = increment_filename(original_filename, user_dir)
        file_path = os.path.join(user_dir, new_filename)

        try:
            # Write the file to the specified location
            with open(file_path, 'wb') as f:
                f.write(file_item.file.read())

            # HTML response for successful upload
            response_body = f"<html><body><h2>File {original_filename} uploaded successfully in {file_path}.</h2></body></html>"
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
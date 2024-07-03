#!/usr/bin/env python3

import cgi
import os

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
            print(f"<html><body><h2>File {filename} uploaded successfully.</h2></body></html>")
        except Exception as e:
            print(f"<html><body><h2>Error uploading file: {e}</h2></body></html>")
    else:
        print("<html><body><h2>No file was uploaded.</h2></body></html>")
#!/usr/bin/env python3

import cgi
import cgitb
from html import escape
from http import HTTPStatus

# Enable debugging
cgitb.enable()

def calculator():
    form = cgi.FieldStorage()

    # Get values from the form
    num1 = form.getvalue("num1")
    num2 = form.getvalue("num2")
    operation = form.getvalue("operation")
    
    # Initialize result
    result = None
    
    # Check if num1 or num2 is empty
    if not num1 or not num2:
        result = "Error: Both fields are required."
    else:        
        try:
            # Convert inputs to float
            num1 = float(num1)
            num2 = float(num2)
            
            # Perform the selected operation
            if operation == "add":
                result = num1 + num2
            elif operation == "subtract":
                result = num1 - num2
            elif operation == "multiply":
                result = num1 * num2
            elif operation == "divide":
                if num2 != 0:
                    result = num1 / num2
                # FIXME: Handle status code when error occurs
                else:
                    result = "Error: Division by zero"
                    status_code = HTTPStatus.BAD_REQUEST
            else:
                result = "Error: Invalid operation"
                status_code = HTTPStatus.BAD_REQUEST
        except ValueError:
            result = "Error: Invalid input. Please enter numeric values."
            status_code = HTTPStatus.BAD_REQUEST
    
    # Create the HTML response
    response_body = f"<html><body><h1>Calculator Result</h1><p>{escape(result)}</p></body></html>"
    status_code = HTTPStatus.OK

    return response_body, status_code

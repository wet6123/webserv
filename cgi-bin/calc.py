#!/usr/bin/env python3

import cgi
import cgitb
from html import escape

# Enable debugging
cgitb.enable()

def main():
    form = cgi.FieldStorage()
    
    # Get values from the form
    num1 = form.getvalue("num1")
    num2 = form.getvalue("num2")
    operation = form.getvalue("operation")
    
    # Initialize result
    result = None
    
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
            else:
                result = "Error: Division by zero"
        else:
            result = "Error: Invalid operation"
    except ValueError:
        result = "Error: Invalid input. Please enter numeric values."
    
    # Print the HTML response
    print("Content-Type: text/html")
    print()
    print("<html><body>")
    print("<h1>Calculator Result</h1>")
    
    if isinstance(result, str):
        # Error message
        print(f"<p>{escape(result)}</p>")
    else:
        # Calculation result
        print(f"<p>Result: {result}</p>")
    
    print('<a href="/calculator.html">Go back</a>')
    print("</body></html>")

if __name__ == "__main__":
    main()
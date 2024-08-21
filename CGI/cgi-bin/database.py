#!/usr/bin/env python3

import json
import os

USER_DB = "users.json"

def load_users():
    if not os.path.exists(USER_DB):
        return {}
    with open(USER_DB, 'r') as file:
        return json.load(file)

def save_users(users):
    with open(USER_DB, 'w') as file:
        json.dump(users, file)

def register_user(username, password):
    users = load_users()
    if username in users:
        return False
    users[username] = password
    save_users(users)
    return True

def authenticate_user(username, password):
    users = load_users()
    return users.get(username) == password
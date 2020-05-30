import sqlite3
import os
import sys
from pathlib import Path

path = os.path.abspath(os.path.dirname(os.path.dirname(__file__)))
db_path = os.path.join(path,"software/instance/app.sqlite")
conn = None

def get_db():
    conn = sqlite3.connect(db_path) 
    return conn
  
def close_db():
    conn.close()    



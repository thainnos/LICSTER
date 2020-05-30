import os
from db import get_db
from pathlib import Path

filepath = os.path.join(os.path.dirname(os.path.realpath(__file__)), "snort-datei")

with open(filepath, 'r') as f:
    line_array = f.readlines()
    
singleline_array = []

for i in range (-6, -1):
    singleline_array.append(line_array[i].split())
    
Type = singleline_array[0][2] + ' ' + singleline_array[0][3] + ' ' + singleline_array[0][4] + ' ' + singleline_array[0][5]
Classification =  singleline_array[1][1] + ' ' + singleline_array[1][2] + ' ' + singleline_array[1][3].replace("]" , "") 
Priority = singleline_array[1][5].replace("]" , '') 
Datetime = singleline_array[2][0].split(".")[0]

db = get_db()
db.execute('INSERT INTO snort (snort_type, snort_classification, snort_priority, snort_datetime) VALUES (?,?,?,?)', (Type, Classification, Priority, Datetime))
db.commit()

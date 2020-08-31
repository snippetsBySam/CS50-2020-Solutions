import csv
import sys
from cs50 import SQL

# Check for current number of arguments
if len(sys.argv) != 2:
    print("USAGE: import.py house")
    sys.exit(1)

# Open db file
db = SQL("sqlite:///students.db")

house_name = sys.argv[1]

# Find all students in selected house
student_list = db.execute("SELECT first,middle,last,birth from students WHERE students.house = ? ORDER BY students.last, students.first", house_name)

# print required info for each
for student in student_list:
    # middle name logic
    if student['middle']:
        full_name = ' '.join([student['first'], student['middle'], student['last']])
    else:
        full_name = ' '.join([student['first'], student['last']])
    print(f'{full_name}, born {student["birth"]}')

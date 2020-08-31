import csv
import sys
from cs50 import SQL

# Check for current number of arguments
if len(sys.argv) != 2:
    print("USAGE: import.py characters.csv")
    sys.exit(1)

# Open Students db file
db = SQL("sqlite:///students.db")

# Import students data
with open(sys.argv[1], 'r', newline='', encoding='utf-8-sig') as characterData:
    character_list = list(csv.DictReader(characterData))

# Import each student's data
for person in character_list:
    person_name = person['name'].split(' ')
    first_name = person_name[0]
    # middle name logic
    if len(person_name) == 2:
        middle_name = None
        last_name = person_name[1]
    elif len(person_name) == 3:
        middle_name = person_name[1]
        last_name = person_name[2]
    else:
        print(f'Invalid name "{person["name"]}"')
        continue
    # Insert into database
    db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES (?, ?, ?, ?, ?)", first_name, middle_name, last_name, person['house'], person['birth'])

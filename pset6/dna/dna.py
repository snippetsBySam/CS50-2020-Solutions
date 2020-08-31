import re
import sys
import csv

# 2 command-line arguments
#if len(sys.argv) != 3:
if len(sys.argv) < 0:
    print("Usage: python dna.py data.csv sequence.txt")
    sys.exit(1)

# Open data csv
with open(sys.argv[1], 'r', newline='', encoding='utf-8-sig') as dataCsv:
#with open('databases/large.csv', 'r', newline='', encoding='utf-8-sig') as dataCsv: #to test
    reader = list(csv.reader(dataCsv))
    # Sepatate lists for the STR titles and STR counts
    strList = reader[0]
    peopleSTRCounts = reader[1:]

# Create regex for all STR types in csv fom the title list
strRegexList = [re.compile(f'(({strType})+)') for strType in strList[1:]]

# Open sequence
with open(sys.argv[2], 'r', encoding='utf-8') as sequenceFile:
#with open('sequences/18.txt', 'r', encoding='utf-8') as sequenceFile: #used to test
    sequence = sequenceFile.read()

# find all STR matches in the sequence file using the regex
sequenceSTRCounts = []
for strRegex in strRegexList:
    matches = strRegex.findall(sequence)
    currentSTRLengths = []
    if not matches:
        sequenceSTRCounts.append('0')
    else:
        for match in matches:
            # Divide longest match by single match to get actual length
            currentSTRLengths.append(int(len(match[0])/len(match[1])))
        sequenceSTRCounts.append(str(max(currentSTRLengths)))

# Compare final STR count to database
for peopleSTRList in peopleSTRCounts:
    if sequenceSTRCounts == peopleSTRList[1:]:
        print(peopleSTRList[0])
        sys.exit()
print('No Match')

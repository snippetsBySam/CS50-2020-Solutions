from cs50 import get_string
import re

text = get_string("Text: ")
# letters regex
letters = len(re.findall(r'''[a-zA-Z]''', text))
# words regex
words = len(re.findall(r'''[\w"';,.:-]+''', text))
# sentences regex
sentences = len(re.findall(r'''[.!?]+''', text))
index = 0.0588 * ((letters*100)/words) - 0.296 * ((sentences*100)/words) - 15.8
print("Before Grade 1" if index < 1 else "Grade 16+" if index >= 16 else f"Grade {round(index)}")

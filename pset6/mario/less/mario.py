from cs50 import get_int

# No comment required
while True:
    s = get_int("Pyramid height: ")
    if s >= 1 and s <= 8:
        break
for i in range(s):
    print(" " * (s-i-1), "#" * (i+1), sep = "")
    
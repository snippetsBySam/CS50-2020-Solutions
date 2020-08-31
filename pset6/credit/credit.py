from cs50 import get_int

#Input CC number
s = get_int("Number: ")

#0. Multiply every other digit by 2, starting with the number’s second-to-last digit, and then add those products' digits together
# change credit number to string
creditCardString = str(s)
cumulativeSum = 0
# keep track of odd/even positioned digits
isSecond = False
for digit in creditCardString[::-1]:
    dInt = int(digit)
    if isSecond == True:
        dInt *= 2
    # Adds the first digit
    cumulativeSum += dInt % 10
    # adds second digit if greatest than 9 (zero otherwise)
    cumulativeSum += dInt // 10
    isSecond = not isSecond
print(cumulativeSum)

if cumulativeSum % 10 != 0:
    print("INVALID")
else:
    if 51 <= s//100000000000000 <= 55:
        print("MASTERCARD")
    elif s // 10000000000000 == 34 or s // 10000000000000 == 37:
        print("AMEX")
    elif s // 1000000000000000 or s // 1000000000000 == 4:
        print("VISA")
    else:
        print("INVALID")

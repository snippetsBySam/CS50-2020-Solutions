from cs50 import get_float

while True:
    # force valid value
    cash = get_float("Change owed: ")
    if cash >= 0:
        break
# List of coins
coins = [25, 10, 5, 1]
cents = cash * 100
totalcoins = 0
for coin in coins:
    coinsUsed = cents // coin
    # cents remaining
    cents -= coinsUsed * coin
    totalcoins += coinsUsed
    if cents < 1:
        break
print(totalcoins)

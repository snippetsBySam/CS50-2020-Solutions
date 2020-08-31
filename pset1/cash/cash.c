#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int main(void)
{
    float cash;
    // do while to force correct input
    do
    {
        cash = get_float("Change owed: ");
    }
    while (cash < 0);

    //convert to cents
    int coins = round(cash * 100);
    //number of quarters
    int quarters = coins / 25;
    //remainder after quarters
    int quartersRm = coins - (25 * quarters);
    // number of dimes
    int dimes = quartersRm / 10;
    // remainder after dimes
    int dimesRm = quartersRm - (10 * dimes);
    // number of nickles
    int nickels = dimesRm / 5;
    //remainder after nickels
    int nickelsRm = dimesRm - (5 * nickels);
    // number of cents
    int cents = nickelsRm;
    //get the total
    int totalCoins = quarters + dimes + nickels + cents;
    printf("%d\n", totalCoins);

}

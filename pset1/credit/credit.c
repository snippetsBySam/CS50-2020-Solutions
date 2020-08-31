
#include <cs50.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    //credit card number
    long long ccn;
    //current digit being looked it
    long long curnt;
    ccn = get_long_long("Positive Number: ");  // credit card no
    long long ccndt = ccn / 10; // divide cc no by 10 so you get the second last digit when you ask for modulus
    int sumod = 0; //to be used for cummulative sum of numbers multiplied by 2
    // Sum of numbers not multiplied my 2
    int sumnm;
    //ccn value for the loop
    long long ccnl = ccn;

    // Finding the sum of the 2nd to last numbers
    while (ccndt) //while the ccndt value exists
    {
        curnt = (ccndt % 10) * 2; // modulus gives the 2nd last number of the ccn the its multiplied by 2 and added to the sum
        if (curnt > 9) // if greater than 9, the result gives double digits when multiplied by 2
        {
            sumod = sumod + 1 + (curnt % 10); //the first digit is always 1 so that gets added to the second digit (which is found from the modulus)
        }
        else
        {
            sumod = sumod + curnt; //
        }
        ccndt /= 100; // divide by 100 to move two digits to the left
    }
    //printf("The sum of 2nd to last numbers is %d\n", sumod);

    //finding the sum of the other numbers
    int sixt = ccn / 1000000000000000;
    int fift = ccn / 100000000000000;
    int four = ccn / 10000000000000;
    int thir = ccn / 1000000000000;

    //finding the sum of the other numbers
    while (ccnl)
    {
        sumnm = sumnm + (ccnl % 10);
        ccnl /= 100;
    }
    //printf("The sum of unmultiplied numbers is %d\n", sumnm);

    //Sum of both have to be divisible by zero
    if ((sumod + sumnm) % 10 != 0)
    {
        printf("INVALID\n");
    }

    //Mastercard check
    else if (fift >= 51 && fift <= 55)
    {
        printf("MASTERCARD\n");
    }

    //Amex check
    else if ((four == 34) || (four == 37))
    {
        printf("AMEX\n");
    }

    //Visa check
    else if ((thir == 4) || (sixt == 4))
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }


}

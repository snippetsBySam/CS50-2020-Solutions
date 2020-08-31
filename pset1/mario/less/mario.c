#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int n; // declare int variable
    //Reprompt while input is not in required range
    do
    {
        n = get_int("Height: ");
    }
    while (n < 1 || n > 8);

    //outer for-loop for each row if the pyramid
    for (int i = 0; i < n; i++)
    {
        //number of spaces on each row
        for (int j = i + 1; j < n; j++)
        {
            printf(" ");
        }
        //number of hashes
        for (int k = 0; k <= i; k++)
        {
            printf("#");
        }
        //line break after each row
        printf("\n");
    }
}

#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // Prompt user for a positive number
    int n; //declare empty vatiable to be used later
    // do while loop keeps prompting user for a valid value between the specified range
    do
    {
        n = get_int("Positive Number: "); //prompts user for input
    }
    // The range of input values is 1 - 8
    while (n < 1 || n > 8);

    // The main for loop which determines the number of rows
    for (int i = 0; i < n; i++)
    {
        // First secondary forloop which does the whitespace according to what row the loop is currently on
        for (int k = i; n - k > 1; k++)
        {
            printf(" ");
        }

        //Second forloop which builds the blocks on each row
        for (int j = 0; j <= i; j++)
        {
            printf("#");
        }

        // print 2 spaces
        printf("  ");

        //Print the other side of the pyramid (copy of previous for loop)
        for (int j = 0; j <= i; j++)
        {
            printf("#");
        }
        //line break after each row
        printf("\n");
    }
}

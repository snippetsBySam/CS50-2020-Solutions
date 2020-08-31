#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, string argv[])
{
    // enforce 2 arguments
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    // ENSURE key only contains digits
    for (int i = 0, n = strlen(argv[1]); i < n; i++)
    {
        if (!isdigit(argv[1][i]))
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
    }
    // convert key to int
    int k = atoi(argv[1]);

    // get plain text
    string p = get_string("plaintext: ");
    int n = strlen(p);
    // allocate cypher array (1 char larger for null terminator)
    char c[n + 1];
    c[n] = '\0';
    for (int i = 0; i < n; i++)
    {
        // only change alphabet characters
        if (isalpha(p[i]))
        {
            if (isupper(p[i]))
            {
                //uppercasse logic
                c[i] = ((p[i] - 'A' + k) % 26) + 'A';
            }
            else
            {
                //lower case logic
                c[i] = ((p[i] - 'a' + k) % 26) + 'a';
            }
        }
        // non-alpha unchaged
        else
        {
            c[i] = p[i];
        }
    }
    printf("ciphertext: %s\n", c);
    return 0;
}

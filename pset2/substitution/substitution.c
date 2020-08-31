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
        printf("Usage: ./substitution key\n");
        return 1;
    }
    // get key and length
    string key = argv[1];
    int key_length = strlen(key);
    // validate key length
    if (key_length != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    else
    {
        char zero_array[key_length + 1];
        zero_array[key_length] = '\0';
        for (int i = 0; i < key_length; i++)
        {
            zero_array[i] = '0';
        }
        for (int j = 0; j < key_length; j++)
        {
            // Validate key; contains only alphabet characters
            if (isalpha(key[j]))
            {
                // Validate key; contains non-repeated characters
                // create array for each letter[26 length] filled with '0' and populate it with
                // the letters in the key. if 'o' is overwritten then the letter in not unique
                if (zero_array[tolower(key[j]) - 97] == '0')
                {
                    zero_array[tolower(key[j]) - 97] = tolower(key[j]);
                }
                else
                {
                    printf("Key cannot contain repeated characters.\n");
                    return 1;
                }
            }
            else
            {
                printf("Key must only contain alphabet characters.\n");
                return 1;
            }
        }
    }
    // turn key to all lower case for easier manipulation
    int key_lower[key_length + 1];
    key_lower[key_length] = '\0';
    // create substution table according to the key
    for (int i = 0; i < key_length; i++)
    {
        key_lower[i] = tolower(key[i]) - 97;
    }

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
                c[i] = key_lower[(p[i] - 'A') % 26]  + 'A';
            }
            else
            {
                //lower case logic
                c[i] = key_lower[(p[i] - 'a') % 26]  + 'a';
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

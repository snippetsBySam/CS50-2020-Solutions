#include <cs50.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

int main(void)
{
    //declare initial values
    int letter_count = 0, word_count = 0, sentence_count = 0;
    char previous_letter = '\0';
    //get text
    string text = get_string("Text: ");
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        //text only
        if ((text[i] >= 'a' && text[i] <= 'z') || (text[i] >= 'A' && text[i] <= 'Z'))
        {
            letter_count++;
            //for when there are no spaces and only 1 word with no punctuation
            if (word_count == 0)
            {
                word_count++;
            }
        }
        //check for spaces and commas (ignores sinsecutive spaces/commas)
        //TODO check for mutiple spaces
        if ((text[i] == '\t' || text[i] == ' ' || text[i] == ',') && (previous_letter != ' ' && previous_letter != ','))
        {
            word_count++;
        }
        //check for sentence
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentence_count++;
        }
        previous_letter = text[i];
    }
    //calculations
    float L = ((letter_count * 100.00) / word_count);
    float S = ((sentence_count * 100.00) / word_count);
    float index = 0.0588 * ((letter_count * 100.00) / word_count) - 0.296 * ((sentence_count * 100.00) / word_count) - 15.8;
    printf("Letter count: %i\n", letter_count);
    printf("Word count: %i\n", word_count);
    printf("Sentence count: %i\n", sentence_count);
    printf("L: %f\n", L);
    printf("S: %f\n", S);
    printf("index: %f\n", index);
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %.0f\n", index);
    }
}

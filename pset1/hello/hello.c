#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Get name from user input
    string name = get_string("What is your name?\n");
    // Say hello to user
    printf("hello, %s\n", name);
}

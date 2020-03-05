#include <stdio.h>
#include <stdlib.h>


int main(int argc, char **argv)
{
	int val = 0b111100011010;

    int last2 = 0b11 & val;

    printf("%d \n", val);
    printf("%d", last2);


}


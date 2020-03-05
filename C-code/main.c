#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void binprintf(int v)
{
    unsigned int mask=1<<((sizeof(int)<<3)-1);
    while(mask) {
        printf("%d", (v & mask ? 1 : 0));
        mask >>= 1;
    }
}



int main(int argc, char **argv)
{
	int MSB = 0b10000001;

    int LSB = 0b01110001;

    int all_data = (MSB << 8) | LSB ;

    int temp_data = 0b111111111111 & (all_data >> 3); // 111111111111 & all_data (MINUS de tre første bits)
        
    
    
    printf("--------- Binary tests! -----------\n");
    binprintf(MSB);
    printf("\n");
    binprintf(LSB);
    printf("\n");
    binprintf(all_data);
    printf("\n");
    binprintf(temp_data);
    printf("\n--------------------------\n");
    printf("%d",temp_data);
    printf("\n");

    char str[20];
    sprintf(str, "%d", temp_data);
    printf("%s", str);
}


#include <stdio.h>
#include <cs50.h>

int main(void){
    long number;
    long check;
    int bits = 1;
    do{
        number = get_long("Number: ");
    } while (number <= 0);
    check = number;
    do{
        check = check/10;
        if (check != 0)
            bits++;
    } while(check != 0);
    check = number;
    int array[bits];
    int sum1 = 0, sum2 = 0;
    for (int i = 0; i < bits; i++){
        array[i] = check - (check/10)*10;
        check = check/10;
        if (i%2 != 0){
            sum1 += (array[i]*2)%10 + (array[i]*2)/10;
        }
        else {
            sum2 += array[i];
        }
    }
    if ((sum1+sum2)%10 == 0 && bits >= 13){
        int begin = array[bits-1]*10 + array[bits-2];
        if (array[bits-1] == 4){
            printf("VISA\n");
        } else if (begin == 34 || begin == 37) {
            printf("AMEX\n");
        } else if (begin >= 51 && begin <= 55){
            printf("MASTERCARD\n");
        } else {printf("INVALID\n");}   
    } else {printf("INVALID\n");}
    
    printf(" %i %i\n", sum1, sum2);
}

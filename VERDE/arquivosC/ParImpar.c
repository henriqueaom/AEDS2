#include <stdio.h>

int main(){

    int n = 0;

    printf("digite um numero \n" );
    scanf("%i", &n);

    if (n % 2 == 0){
        printf("par");
    }else{ 
        printf("impar");
    }
}
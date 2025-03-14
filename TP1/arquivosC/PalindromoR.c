#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

bool palindromo(char palavra[], int esquerda,int direita){



    if(esquerda>direita){
        return false;
    }


        if(palavra[esquerda]!=palavra[direita]){
            return false;
        }else{
            palindromo(palavra,esquerda+1,direita-1);
        }


    return true;
}



int main(){

    char palavra[1000];

    while(1){

    scanf(" %[^\n]", palavra);
    int tamanho;
    tamanho=strlen(palavra);

    int esquerda=0;
    int direita=tamanho-1;

    if (strcmp(palavra, "FIM") == 0) {
            return 0;
        }

    printf(palindromo(palavra,esquerda,direita)? "SIM\n":"NAO\n");

    }


    return 0;
}
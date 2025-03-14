#include <stdio.h>
#include <stdbool.h>
#include <string.h>


#define max_size 100

 void palindromo(char palavra[]){

    int tamanho =strlen(palavra);
   

    for(int i = 0; i<tamanho/2; i++){
        int j = tamanho - 1 - i;
            if(palavra[i] != palavra[j]){
                printf("NAO\n");
                return;
            }
        
    }


  printf("SIM\n");



 }

bool conferirFim(char palavra[]){

    if(strcmp(palavra,"FIM") == 0){
        return true;
    }else{
        return false;
    }
}

int main(){

    char palavra[max_size];

     while(conferirFim(palavra) != true){

        fgets(palavra,sizeof(palavra),stdin);

        palavra[strcspn(palavra, "\n")] = 0;

        palindromo(palavra);

    }



}
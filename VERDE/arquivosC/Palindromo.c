#include <stdio.h>
#include <string.h>

int main()
{
    char texto[10] = "";
    int result = 1;

    while (1)  // A condição de parada vai ser dentro do loop
    {
        result = 1;
        scanf(" %[^\n]", texto);  // Lê até a quebra de linha

        if (strcmp(texto, "FIM") == 0)  // Compara as strings corretamente
        {
            break;  // Sai do loop se a palavra for "FIM"
        }

        int tamanho = strlen(texto);

        // Verifica se a palavra é um palíndromo
        for (int i = 0; i < tamanho / 2; i++)
        {
            if (texto[i] != texto[tamanho - i - 1])
            {
                result = 0;  // Define como não palíndromo
                break;  // Sai do loop assim que encontrar a diferença
            }
        }

        if (result == 1)
        {
            printf("SIM\n");
        }
        else
        {
            printf("NAO\n");
        }
    }

    return 0;
}

#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *arquivo;
    char caminho[] = "numeros.txt";
    int n;

    // Criando e escrevendo no arquivo
    arquivo = fopen(caminho, "w");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        double numero;
        scanf("%lf", &numero);
        fprintf(arquivo, "%.3f\n", numero); // Altera para 3 casas decimais
    }

    fclose(arquivo);

    // Lendo de trás para frente
    arquivo = fopen(caminho, "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo para leitura");
        return 1;
    }

    fseek(arquivo, 0, SEEK_END);
    long posicao = ftell(arquivo);

    char buffer[50]; // Para armazenar cada número
    long ultimaPosicao = posicao;

    for (long i = posicao - 1; i >= 0; i--) {
        fseek(arquivo, i, SEEK_SET);
        char c = fgetc(arquivo);

        if (c == '\n' && i != posicao - 1) {
            fseek(arquivo, i + 1, SEEK_SET);
            fgets(buffer, sizeof(buffer), arquivo);
            printf("%s", buffer); // Imprime o número
            ultimaPosicao = i;
        }

        if (i == 0) { // Trata o primeiro número do arquivo
            fseek(arquivo, 0, SEEK_SET);
            fgets(buffer, sizeof(buffer), arquivo);
            printf("%s", buffer);
        }
    }

    fclose(arquivo);
    return 0;
}

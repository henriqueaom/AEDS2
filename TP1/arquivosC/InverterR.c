#include <stdio.h>
#include <string.h>

// Função recursiva para inverter a string
void inverter(char *texto, int inicio, int fim) {
    if (inicio >= fim) {
        return;
    }
    // Troca os caracteres
    char temp = texto[inicio];
    texto[inicio] = texto[fim];
    texto[fim] = temp;

    // Chamada recursiva para os próximos caracteres
    inverter(texto, inicio + 1, fim - 1);
}

int main() {
    char texto[100];

    do {
        fgets(texto, sizeof(texto), stdin);
        texto[strcspn(texto, "\n")] = 0; // Remove o '\n' do fgets

        if (strcmp(texto, "FIM") != 0) {
            inverter(texto, 0, strlen(texto) - 1);
            printf("%s\n", texto);
        }

    } while (strcmp(texto, "FIM") != 0);

    return 0;
}
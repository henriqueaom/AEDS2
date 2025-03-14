#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Função recursiva para somar os dígitos da string
int somaDigitos(const char *n, int i) {
    if (i >= strlen(n)) {
        return 0;
    }

    int n2 = n[i] - '0';
    return n2 + somaDigitos(n, i + 1);
}

int main() {
    char n[100];

    do {
        fgets(n, sizeof(n), stdin);
        n[strcspn(n, "\n")] = 0; // Remove o '\n' do fgets

        if (strcmp(n, "FIM") == 0) {
            break;
        }

        int resultado = somaDigitos(n, 0);
        printf("%d\n", resultado);

    } while (1);

    return 0;
}
import java.util.Scanner;

class Matriz {
    private int[][] dados;
    private int linhas;
    private int colunas;

    public Matriz(int linhas, int colunas) {
        this.linhas = linhas;
        this.colunas = colunas;
        this.dados = new int[linhas][colunas];
    }

    public void preencher(Scanner scanner) {
        for (int i = 0; i < linhas; i++) {
            for (int j = 0; j < colunas; j++) {
                dados[i][j] = scanner.nextInt();
            }
        }
    }

    public Matriz soma(Matriz outra) {
        if (this.linhas != outra.linhas || this.colunas != outra.colunas) {
            throw new IllegalArgumentException("Matrizes devem ter as mesmas dimensões para soma");
        }

        Matriz resultado = new Matriz(linhas, colunas);
        for (int i = 0; i < linhas; i++) {
            for (int j = 0; j < colunas; j++) {
                resultado.dados[i][j] = this.dados[i][j] + outra.dados[i][j];
            }
        }
        return resultado;
    }

    public Matriz multiplicacao(Matriz outra) {
        if (this.colunas != outra.linhas) {
            throw new IllegalArgumentException("Número de colunas da primeira matriz deve ser igual ao número de linhas da segunda");
        }

        Matriz resultado = new Matriz(this.linhas, outra.colunas);
        for (int i = 0; i < this.linhas; i++) {
            for (int j = 0; j < outra.colunas; j++) {
                int soma = 0;
                for (int k = 0; k < this.colunas; k++) {
                    soma += this.dados[i][k] * outra.dados[k][j];
                }
                resultado.dados[i][j] = soma;
            }
        }
        return resultado;
    }

    public void mostrarDiagonalPrincipal() {
        int tamanho = Math.min(linhas, colunas);
        for (int i = 0; i < tamanho; i++) {
            System.out.print(dados[i][i] + " ");
        }
        System.out.println();
    }

    public void mostrarDiagonalSecundaria() {
        int tamanho = Math.min(linhas, colunas);
        for (int i = 0; i < tamanho; i++) {
            System.out.print(dados[i][colunas - 1 - i] + " ");
        }
        System.out.println();
    }

    public void imprimir() {
        for (int i = 0; i < linhas; i++) {
            for (int j = 0; j < colunas; j++) {
                System.out.print(dados[i][j] + " ");
            }
            System.out.println();
        }
    }
}

public class java11 {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        int numCasos = scanner.nextInt();

        for (int caso = 0; caso < numCasos; caso++) {
            // Ler primeira matriz
            int linhas1 = scanner.nextInt();
            int colunas1 = scanner.nextInt();
            Matriz m1 = new Matriz(linhas1, colunas1);
            m1.preencher(scanner);

            // Ler segunda matriz
            int linhas2 = scanner.nextInt();
            int colunas2 = scanner.nextInt();
            Matriz m2 = new Matriz(linhas2, colunas2);
            m2.preencher(scanner);

            // Mostrar diagonais da primeira matriz
            m1.mostrarDiagonalPrincipal();
            m1.mostrarDiagonalSecundaria();

            // Soma
            if (linhas1 == linhas2 && colunas1 == colunas2) {
                Matriz soma = m1.soma(m2);
                soma.imprimir();
            }

            // Multiplicação
            if (colunas1 == linhas2) {
                Matriz multiplicacao = m1.multiplicacao(m2);
                multiplicacao.imprimir();
            }
        }
        scanner.close();
    }
}
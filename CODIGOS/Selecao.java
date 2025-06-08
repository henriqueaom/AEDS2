package CODIGOS;

class Selecao {

    static void swap(int[] vetor, int i, int j) {
        int temp = vetor[j];
        vetor[j] = vetor[i];
        vetor[i] = temp;
    }

    public static void main(String[] args) {

        int[] vetor = { 1, 2, 50, 45, 23, 43, 9, 80, 5 };
        int menor;

        for (int i = 0; i < vetor.length - 1; i++) {
            menor = i;
            for (int j = i + 1; j < vetor.length; j++) {
                if (vetor[menor] > vetor[j]) {
                    menor = j;
                }
            }
            if (menor != i) {
                swap(vetor, menor, i);
            }
        }

        for (int i = 0; i < vetor.length; i++) {
            System.out.println(vetor[i]);
        }

    }

}

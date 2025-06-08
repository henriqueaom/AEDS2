package CODIGOS;

public class QuickSort {

    static void swap(int[] vetor, int i, int j) {
        int temp = vetor[j];
        vetor[j] = vetor[i];
        vetor[i] = temp;
    }

    static void Quick(int[] vetor, int esq, int dir) {

        int i = esq, j = dir, pivo = vetor[(esq + dir) / 2];

        while (i <= j) {

            while (vetor[i] < pivo) {
                i++;
            }

            while (vetor[j] > pivo) {
                j--;
            }

            if (i <= j) {
                swap(vetor, i, j);
                i++;
                j--;

            }
        }

        if (esq < j) {
            Quick(vetor, esq, j);
        }

        if (i < dir) {
            Quick(vetor, i, dir);
        }
    }

    

    public static void main(String[] args) {

        int[] vetor = { 5, 7, 1, 45, 23, 43, 9, 80, 10 };

        Quick(vetor, 0, vetor.length-1);

        for (int i = 0; i < vetor.length; i++) {
            System.out.println(vetor[i]);
        }

    }

}

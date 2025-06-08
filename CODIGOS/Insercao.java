package CODIGOS;

public class Insercao {

    public static void main(String[] args) {

        int[] vetor = { 5, 7, 1, 45, 23, 43, 9, 80, 10 };
        int temp;
        int j;

        for (int i = 1; i < vetor.length - 1; i++) {
            j = i - 1;
            temp = vetor[i];

            while (j > 0 && vetor[j] > temp) {
                vetor[j + 1] = vetor[j];
                j--;
            }

            vetor[j+1] = temp;

        }

        for (int i = 0; i < vetor.length; i++) {
            System.out.println(vetor[i]);
        }

    }

}

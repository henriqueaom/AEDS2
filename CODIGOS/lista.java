package CODIGOS;

public class lista {

    private int[] vetor;
    private int n;

    // Construtor que inicializa o vetor com o tamanho informado
    public lista(int tamanho) {
        this.vetor = new int[tamanho];
        this.n = 0; // Inicializa o contador como zero
    }

    public int inserirI(int x) {

        if (n > vetor.length) {
            System.out.println("vetor cheio");
            return n;

        } else {

            for (int i = n; i > 0; i--) {
                vetor[i] = vetor[i - 1];
            }

            vetor[0] = x;
            n++;
            System.out.println("numero inserido = " + x);

        }

        return n;

    }

    public int inserirF(int x) {

        if (n > vetor.length) {
            System.out.println("vetor cheio");
            return n;

        } else {
            vetor[n] = x;
            n++;
            System.out.println("numero inserido = " + x);
        }

        return n;
    }

    public int inserirP(int x, int pos) {

        if (n > vetor.length) {
            System.out.println("vetor cheio");
            return n;

        } else {

            for (int i = n; i > pos; i--) {
                vetor[i] = vetor[i - 1];
            }

            vetor[pos] = x;
            n++;
            System.out.println("numero inserido = " + x);

        }

        return n;

    }

    public void print() {

        for (int i = 0; i < n; i++) {
            System.out.println(vetor[i]);
        }

    }

    public void RemoverI() {
        int temp = vetor[0];

        if (n == 0) {
            System.out.println("vetor vazio");

        } else {
            for (int i = 0; i < n; i++) {
                vetor[i] = vetor[i + 1];
            }

            System.out.println("Numero removido = " + temp);
            n--;

        }
    }

    
    public void RemoverF() {
        int temp = vetor[n -1 ];

        if (n == 0) {
            System.out.println("vetor vazio");

        } else {
          
            System.out.println("Numero removido = " + temp);
            n--;

        }
    }


    public void RemoverP(int pos) {
        int temp = vetor[pos];

        if (n < 0 && pos > n) {
            System.out.println("vetor vazio");

        } else {
            for (int i = pos; i < n; i++) {
                vetor[i] = vetor[i + 1];
            }

            System.out.println("Numero removido = " + temp);
            n--;

        }
    }

    public static void main(String[] args) {

        lista lista = new lista(6);

        System.out.println("inciando...");
        lista.inserirI(2);
        lista.inserirF(6);
        lista.inserirF(9);
        lista.inserirP(10, 1);
        lista.inserirI(12);
        lista.print();
        System.out.println("Removendo...");
        lista.RemoverI();
        lista.print();
        System.out.println("Removendo...");
        lista.RemoverF();
        lista.print();
        System.out.println("Removendo...");
        lista.RemoverP(1);
        lista.print();


    }

}

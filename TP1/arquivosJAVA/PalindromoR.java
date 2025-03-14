import java.util.Scanner;

public class PalindromoR {

    static boolean teste(String palavra, int esquerda, int direita) {

        if (esquerda > direita) {
            return true;
        }

        if (palavra.charAt(esquerda) == palavra.charAt(direita)) {

            teste(palavra, esquerda + 1, direita - 1);

        } else {
            return false;
        }
        return true;
    }

    public static void main(String[] args) {

        Scanner sc = new Scanner(System.in);
        String palavra;

        while (true) {
            palavra = sc.nextLine();
            int esquerda = 0;
            int direita = palavra.length() - 1;

            if (palavra.equals("FIM")) {
                sc.close();
                System.exit(0);
            }

            if (teste(palavra, esquerda, direita) == true) {
                System.out.println("SIM");
            } else {
                System.out.println("NAO");
            }

        }
    }
}
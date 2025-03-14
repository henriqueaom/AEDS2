import java.util.Scanner;

public class CiframentoR {

    static String teste(String palavra, int posicao) {

        char letra;
        if (posicao == palavra.length()) {
            return "";
        }
        if (palavra.charAt(posicao) == 65533) {
            char c = palavra.charAt(posicao);
            letra = (char) (c);
            return letra + teste(palavra, posicao + 1);
        } else {
            letra = (char) (palavra.charAt(posicao) + 3);
            return letra + teste(palavra, posicao + 1);
        }

    }

    public static void main(String[] args) {

        Scanner sc = new Scanner(System.in);

        while (true) {

            String palavra = sc.nextLine();

            if (palavra.equals("FIM")) {
                sc.close();
                System.exit(0);
            }
            String resp = teste(palavra, 0);
            System.out.println(resp);
        }

    }
}
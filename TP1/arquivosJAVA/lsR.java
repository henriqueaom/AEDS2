import java.util.Scanner;

public class lsR {

    static boolean vogal(String palavra, int i) {
        if (i >= palavra.length()) {
            return true;
        }

        if (palavra.charAt(i) == 'a' || palavra.charAt(i) == 'A'
                || palavra.charAt(i) == 'e' || palavra.charAt(i) == 'E'
                || palavra.charAt(i) == 'i' || palavra.charAt(i) == 'I'
                || palavra.charAt(i) == 'o' || palavra.charAt(i) == 'O'
                || palavra.charAt(i) == 'u' || palavra.charAt(i) == 'U') {

            return vogal(palavra, i + 1);

        } else {
            return false;
        }
    }

    static boolean consoante(String palavra, int i) {
        if (i >= palavra.length()) {
            return true;
        }

        if (palavra.charAt(i) != 'a' && palavra.charAt(i) != 'A'
                && palavra.charAt(i) != 'e' && palavra.charAt(i) != 'E'
                && palavra.charAt(i) != 'i' && palavra.charAt(i) != 'I'
                && palavra.charAt(i) != 'o' && palavra.charAt(i) != 'O'
                && palavra.charAt(i) != 'u' && palavra.charAt(i) != 'U'
                && ((palavra.charAt(i) >= 'a' && palavra.charAt(i) <= 'z')
                        || (palavra.charAt(i) >= 'A' && palavra.charAt(i) <= 'Z'))) {

            return consoante(palavra, i + 1);

        } else {
            return false;
        }
    }

    static boolean inteiro(String palavra, int i) {
        if (i >= palavra.length()) {
            return true;
        }

        if (palavra.charAt(i) >= '0' && palavra.charAt(i) <= '9') {
            return inteiro(palavra, i + 1);
        } else {
            return false;
        }
    }

    static boolean real(String palavra, int i, int cont) {
        if (i >= palavra.length()) {
            return cont == 1;
        }

        if (palavra.charAt(i) == '.' || palavra.charAt(i) == ',') {
            cont++;
            if (cont == 2) {
                return false;
            } else {
                return real(palavra, i + 1, cont);
            }
        } else if (Character.isDigit(palavra.charAt(i))) {
            return real(palavra, i + 1, cont);
        } else {
            return false;
        }
    }

    static void escreva(String palavra) {
        if (vogal(palavra, 0)) {
            System.out.print("SIM ");
        } else {
            System.out.print("NAO ");
        }

        if (consoante(palavra, 0)) {
            System.out.print("SIM ");
        } else {
            System.out.print("NAO ");
        }

        if (inteiro(palavra, 0)) {
            System.out.print("SIM ");
        } else {
            System.out.print("NAO ");
        }

        if (real(palavra, 0, 0)) {
            System.out.print("SIM ");
        } else {
            System.out.print("NAO ");
        }

        System.out.println();
    }

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        while (true) {
            String palavra = sc.nextLine();
            if (palavra.equals("FIM")) {
                sc.close();
                System.exit(0);
            }

            escreva(palavra);
        }
    }
}


import java.util.*;

public class Is {

    public static boolean vogal(String texto) {
        boolean result = true;

        for (int i = 0; i < texto.length(); i++) {
            if (texto.charAt(i) != 'a' && texto.charAt(i) != 'e' && texto.charAt(i) != 'i' && texto.charAt(i) != 'o'
                    && texto.charAt(i) != 'u' && texto.charAt(i) != 'A' && texto.charAt(i) != 'E'
                    && texto.charAt(i) != 'I' && texto.charAt(i) != 'O' && texto.charAt(i) != 'U') {
                result = false;
                return result;

            }
        }

        return result;
    }

    public static boolean Consoante(String texto) {
        boolean result = true;

        for (int i = 0; i < texto.length(); i++) {

            if (texto.charAt(i) < 65 || texto.charAt(i) > 90 && texto.charAt(i) < 97 || texto.charAt(i) > 122) {
                result = false;
                return result;
            } else if (texto.charAt(i) == 'a' || texto.charAt(i) == 'e' || texto.charAt(i) == 'i'
                    || texto.charAt(i) == 'o'
                    || texto.charAt(i) == 'u' || texto.charAt(i) == 'A' || texto.charAt(i) == 'E'
                    || texto.charAt(i) == 'I' || texto.charAt(i) == 'O' || texto.charAt(i) == 'U') {
                result = false;
                return result;

            }
        }

        return result;
    }

    public static boolean Inteiro(String texto) {
        boolean result = true;

        for (int i = 0; i < texto.length(); i++) {
            if (texto.charAt(i) < 48 || texto.charAt(i) > 57) {
                result = false;
                return result;
            }
        }

        return result;
    }

    public static boolean Real(String texto) {
        boolean result = true;

        for (int i = 0; i < texto.length(); i++) {
            if (texto.charAt(i) < 48 || texto.charAt(i) > 57 || texto.charAt(i) == 46 || texto.charAt(i) == 44) {
                result = false;
                return result;
            }
        }

        return result;
    }

    public static void main(String[] args) {

        Scanner sc = new Scanner(System.in);
        String texto = "";

        do {

            texto = sc.nextLine();

            if (texto.equals("FIM")) {
                break;
            }

            if (vogal(texto)) {
                System.out.print("SIM ");

            } else {
                System.out.print("NAO ");

            }

            if (Consoante(texto)) {
                System.out.print("SIM ");

            } else {
                System.out.print("NAO ");

            }

            if (Inteiro(texto)) {
                System.out.print("SIM ");

            } else {
                System.out.print("NAO ");

            }

            if (Real(texto)) {
                System.out.print("SIM");
                System.out.println();

            } else {
                System.out.print("NAO");
                System.out.println();

            }

        } while (true);

        sc.close();

    }

}

package VERDE.arquivosJAVA;

import java.util.Scanner;

public class Palindromo {
    public static void main(String[] args) {

        String texto = "";
        boolean result = true;
        Scanner sc = new Scanner(System.in);
        while (!texto.equals("FIM")) {
            result= true;
            texto = sc.nextLine();
            if (texto.equals("FIM")) {
                break;
            }

            for (int i = 0; i < texto.length() / 2; i++) {
                if (texto.charAt(i) != texto.charAt(texto.length() - i - 1)) {
                    result = false;
                    i = texto.length() / 2;
                }

            }

            if (result) {
                System.out.println("SIM");
            } else {
                System.out.println("NAO");
            }
        }

        sc.close();

    }

}

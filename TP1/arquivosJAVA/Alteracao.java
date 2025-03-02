
package TP1.arquivosJAVA;

import java.util.Random;
import java.util.Scanner;

public class Alteracao {

    public static void main(String[] args) {

        Scanner sc = new Scanner(System.in);

        Random gerador = new Random();
        gerador.setSeed(4);

        String texto = "";
      

        do{

            if (texto.equals("FIM")) {
                break;
            }

            char n = (char) ('a' + (Math.abs(gerador.nextInt()) % 26));
            char nn = (char) ('a' + (Math.abs(gerador.nextInt()) % 26));

            texto = sc.nextLine();
            char[] texto2 = new char[texto.length()];

            for (int i = 0; i < texto.length(); i++) {
                if (texto.charAt(i) != n) {
                    texto2[i] = texto.charAt(i);
                } else {
                    texto2[i] = nn;
                }

            }

            String alterada = new String(texto2);


            System.out.println(alterada);

            
        }while(!texto.equals("FIM"));

        sc.close();

    }

}

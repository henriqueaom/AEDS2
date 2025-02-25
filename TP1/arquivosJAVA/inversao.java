package TP1.arquivosJAVA;

import java.util.*;

public class inversao {

    public static void main(String[] args) {



         Scanner sc = new Scanner(System.in);
         String texto = "";

         do{
           
         texto = sc.nextLine();
         char[] texto2 = new char[texto.length()];
;

         for( int i =0; i< texto.length(); i ++){
            texto2[i] = texto.charAt(texto.length() -1 - i);
         }

         String invertida = new String(texto2);

         if(!texto.equals("FIM")){
         System.out.println(invertida);
         }
         }while(!texto.equals("FIM"));

         sc.close();


    }

    

}
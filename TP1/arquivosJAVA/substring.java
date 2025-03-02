package TP1.arquivosJAVA;

import java.util.Scanner;

public class substring {

    public static int stringlong(String texto) {
        int maxSequencia = 0; // Armazena o tamanho da maior sequência encontrada
        
        for (int i = 0; i < texto.length(); i++) {
            int sequenciaAtual = 1; // Cada caractere individual conta como uma sequência mínima de 1
            
            for (int j = i + 1; j < texto.length(); j++) {
                if (texto.charAt(j) != texto.charAt(j - 1)) {
                    sequenciaAtual++;
                } else {
                    break; // Interrompe a sequência se encontrar um caractere igual ao anterior
                }
            }
            
            maxSequencia = Math.max(maxSequencia, sequenciaAtual);
        }
        
        return maxSequencia;
    }

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        System.out.print("Digite o texto: ");
        String texto = sc.nextLine();

        int resultado = stringlong(texto);

        System.out.println("Maior sequência de caracteres diferentes consecutivos: " + resultado);

        sc.close();
    }
}

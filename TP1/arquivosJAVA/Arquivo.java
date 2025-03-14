import java.io.*;
import java.util.Scanner;

public class Arquivo {
    public static void main(String[] args) throws IOException {
        Scanner scanner = new Scanner(System.in);
        String caminho = "numeros.txt"; // Nome do arquivo

        // Criando e escrevendo no arquivo
        File arquivo = new File(caminho);
        FileWriter fw = new FileWriter(arquivo);
        PrintWriter pw = new PrintWriter(fw);

        int n = scanner.nextInt(); // Lê a quantidade de números
        scanner.nextLine(); // Consumir quebra de linha

        for (int i = 0; i < n; i++) {
            double numero = scanner.nextDouble();
            pw.println(numero); // Escreve no arquivo
        }

        pw.close();
        fw.close();
        scanner.close();

        // Lendo de trás para frente
        RandomAccessFile raf = new RandomAccessFile(arquivo, "r");
        long posicao = raf.length(); // Posição final do arquivo

        long ultimaPosicao = posicao;

        for (long i = posicao - 1; i >= 0; i--) {
            raf.seek(i);
            char c = (char) raf.read();
            
            if (c == '\n' && i != posicao - 1) {  // Garante que não está no final do arquivo
                raf.seek(i + 1); // Pular o '\n' e posicionar no início do número
                System.out.println(raf.readLine()); // Ler e imprimir o número
                ultimaPosicao = i; // Atualizar a última posição lida
            }

            if (i == 0) {  // Trata o primeiro número do arquivo
                raf.seek(0);
                System.out.println(raf.readLine());
            }
        }

        raf.close();
    }
}
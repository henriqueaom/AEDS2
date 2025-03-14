import java.io.*;
import java.util.*;

public class anagr {

    public static void main(String[] args) throws UnsupportedEncodingException {

        // Define a codificação UTF-8 para a saída padrão
        PrintStream ps = new PrintStream(System.out, true, "UTF-8");
        System.setOut(ps);

        Scanner sc = new Scanner(System.in);
        String texto = "";
        String texto2 = "";
        String resultado = "SIM";

        do {
            resultado = "SIM";  // Inicializa a resposta como "SIM"

            texto = sc.nextLine();  // Lê a linha inteira

            if (texto.equals("FIM")) {
                break;
            }

            // Separa as palavras antes e depois do "-"
            String[] partes = texto.split("-");

            // Atribui as duas palavras e remove espaços extras
            texto = partes[0].trim();
            texto2 = partes[1].trim();

            // Converte ambas as palavras para maiúsculas
            texto = texto.toUpperCase();
            texto2 = texto2.toUpperCase();

            // Ordena as duas palavras para comparação
            char[] textoArr = texto.toCharArray();
            char[] texto2Arr = texto2.toCharArray();

            Arrays.sort(textoArr);
            Arrays.sort(texto2Arr);

            // Verifica se as palavras são anagramas
            if (Arrays.equals(textoArr, texto2Arr)) {
                resultado = "SIM";
            } else {
                resultado = "N"+'\u00C3'+"O";
            }

            System.out.println(resultado);

        } while (!texto.equals("FIM"));

        sc.close();
    }
}

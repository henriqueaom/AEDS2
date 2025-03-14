import java.util.*;

public class InverterR {

    // Método recursivo para inverter a string
    public static String inverter(String texto) {
        if (texto.isEmpty()) {
            return "";
        }
        return texto.charAt(texto.length() - 1) + inverter(texto.substring(0, texto.length() - 1));
    }

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        String texto;

        do {
            texto = sc.nextLine();
            if (!texto.equals("FIM")) {
                System.out.println(inverter(texto));
            }
        } while (!texto.equals("FIM"));

        sc.close();
    }
}
import java.util.Scanner;

public class Senha {

    // Verifica se contém letra maiúscula
    static boolean maiuscula(String entrada) {
        for (int i = 0; i < entrada.length(); i++) {
            char c = entrada.charAt(i);
            if (c >= 'A' && c <= 'Z') {
                return true;
            }
        }
        return false;
    }

    // Verifica se contém letra minúscula
    static boolean minuscula(String entrada) {
        for (int i = 0; i < entrada.length(); i++) {
            char c = entrada.charAt(i);
            if (c >= 'a' && c <= 'z') {
                return true;
            }
        }
        return false;
    }

    // Verifica se a string tem tamanho mínimo de 8 caracteres
    static boolean tamanho(String entrada) {
        return entrada.length() >= 8;
    }

    // Verifica se contém número
    static boolean numero(String entrada) {
        for (int i = 0; i < entrada.length(); i++) {
            char c = entrada.charAt(i);
            if (c >= '0' && c <= '9') {
                return true;
            }
        }
        return false;
    }

    // Verifica se contém caractere especial
    static boolean caracter(String entrada) {
        for (int i = 0; i < entrada.length(); i++) {
            char c = entrada.charAt(i);
            // Verifica se o caractere não é uma letra (maiúscula ou minúscula) nem um número
            if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9'))) {
                return true; // Se for um caractere especial, retorna true
            }
        }
        return false;
    }

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        String entrada;

        while (true) {
            entrada = sc.nextLine();

            if (entrada.equals("FIM")) {
                break;
            }

            if (maiuscula(entrada) && minuscula(entrada) && numero(entrada) && caracter(entrada) && tamanho(entrada)) {
                System.out.println("SIM");
            } else {
                System.out.println("NAO");
            }
        }

        sc.close();
    }
}
package VERDE.arquivosJAVA;

import java.util.Scanner;

public class ParImpar {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        System.out.println("Digite um número: ");
        int n = sc.nextInt();

        if (n % 2 == 0) {
            System.out.print("par");

        } else {
            System.out.print("impar");
        }

        sc.close();
    }
}

package TP2.arquivosJAVA;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

public class Show2 {
    
    public static void main(String[] args) {
        String caminhoArquivo = "C:\\Users\\henri\\Desktop\\AEDS2\\TP2\\disneyplus.csv";
        String linha = "";

        try (BufferedReader br = new BufferedReader(new FileReader(caminhoArquivo))) {
            System.out.println();
            while ((linha = br.readLine()) != null) {
                String[] campos = linha.split(",(?=(?:[^\"]*\"[^\"]*\")*[^\"]*$)");
                if (campos[0].equals("s441")) {
                    for (int i =0;i<campos.length;i++) {
                        if (campos[i].equals("")) {
                            campos[i] = "NaN";
                        }
                    }
                    
                }
               
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}

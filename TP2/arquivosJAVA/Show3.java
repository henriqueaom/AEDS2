package TP2.arquivosJAVA;

import java.util.Date;
import java.util.List;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Locale;
import java.util.Scanner;
import java.text.SimpleDateFormat;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

public class Show3 {
    private String show_id;
    private String type;
    private String title;
    private String director;
    private List<String> cast;
    private String country;
    private Date date_added;
    private int release_year;
    private String rating;
    private String duration;
    private List<String> listed_in;

    // Construtor padrão
    public Show3() {
    }

    // Segundo construtor: com todos os atributos
    public Show3(String show_id, String type, String title, String director,
            List<String> cast, String country, Date date_added,
            int release_year, String rating, String duration,
            List<String> listed_in) {
        this.show_id = show_id;
        this.type = type;
        this.title = title;
        this.director = director;
        this.cast = cast;
        this.country = country;
        this.date_added = date_added;
        this.release_year = release_year;
        this.rating = rating;
        this.duration = duration;
        this.listed_in = listed_in;
    }

    // Getters e Setters
    public String getShow_id() {
        return show_id;
    }

    public void setShow_id(String show_id) {
        this.show_id = show_id;
    }

    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public String getDirector() {
        return director;
    }

    public void setDirector(String director) {
        this.director = director;
    }

    public List<String> getCast() {
        return cast;
    }

    public void setCast(List<String> cast) {
        this.cast = cast;
    }

    public String getCountry() {
        return country;
    }

    public void setCountry(String country) {
        this.country = country;
    }

    public Date getDate_added() {
        return date_added;
    }

    public void setDate_added(Date date_added) {
        this.date_added = date_added;
    }

    public int getRelease_year() {
        return release_year;
    }

    public void setRelease_year(int release_year) {
        this.release_year = release_year;
    }

    public String getRating() {
        return rating;
    }

    public void setRating(String rating) {
        this.rating = rating;
    }

    public String getDuration() {
        return duration;
    }

    public void setDuration(String duration) {
        this.duration = duration;
    }

    public List<String> getListed_in() {
        return listed_in;
    }

    public void setListed_in(List<String> listed_in) {
        this.listed_in = listed_in;
    }

    // Método clone() para criar uma cópia do objeto
    public Show3 clone() {
        Show3 copia = new Show3();
        copia.setShow_id(this.show_id);
        copia.setType(this.type);
        copia.setTitle(this.title);
        copia.setDirector(this.director);

        // Clonando listas (deep copy)
        copia.setCast(this.cast != null ? new ArrayList<>(this.cast) : null);
        copia.setCountry(this.country);
        copia.setDate_added(this.date_added != null ? (Date) this.date_added.clone() : null);
        copia.setRelease_year(this.release_year);
        copia.setRating(this.rating);
        copia.setDuration(this.duration);
        copia.setListed_in(this.listed_in != null ? new ArrayList<>(this.listed_in) : null);

        return copia;
    }

    // Método ler() que lê os registros de um arquivo CSV e retorna uma lista de
    // objetos Show
    public ArrayList<Show3> Ler(String chave) {
        ArrayList<Show3> shows = new ArrayList<>();
        // String caminhoArquivo =
        // "C:\\Users\\henri\\Desktop\\AEDS2\\TP2\\disneyplus.csv";
        String caminhoArquivo = "/tmp/disneyplus.csv";
        String linha;

        // Define o formato esperado para o campo de data, por exemplo: "May 29, 2020"
        SimpleDateFormat sdf = new SimpleDateFormat("MMM dd, yyyy", Locale.ENGLISH);

        try (BufferedReader br = new BufferedReader(new FileReader(caminhoArquivo))) {
            // Se houver cabeçalho, podemos ler a primeira linha e ignorá-la:
            // String header = br.readLine();

            while ((linha = br.readLine()) != null) {
                // Se a linha for "FIM", interrompe a leitura.
                if (linha.equals("FIM"))
                    break;

                // Divide a linha em campos considerando as vírgulas fora de aspas
                String[] campos = linha.split(",(?=(?:[^\"]*\"[^\"]*\")*[^\"]*$)");
                if (campos[0].equals(chave)) {
                    // Se algum campo estiver vazio, substitui por "NaN"
                    for (int i = 0; i < campos.length; i++) {

                        if (campos[i].trim().isEmpty())
                            campos[i] = "NaN";
                        else
                            campos[i] = campos[i].trim();
                    }

                    // Supondo que o CSV possua 11 campos na ordem:
                    // 0: show_id, 1: type, 2: title, 3: director, 4: cast, 5: country,
                    // 6: date_added, 7: release_year, 8: rating, 9: duration, 10: listed_in

                    campos[2] = campos[2].replace("\"", "");

                    // Processa o campo "cast" em uma lista
                    List<String> castList = new ArrayList<>();
                    if (!campos[4].equals("NaN")) {
                        // Remove aspas externas, se existirem
                        String campoCast = campos[4].replaceAll("^\"|\"$", "");
                        String[] elementos = campoCast.split(",\\s*");
                        for (String s : elementos) {
                            castList.add(s);
                        }
                        // Ordena os nomes do elenco em ordem alfabética
                        Collections.sort(castList, String.CASE_INSENSITIVE_ORDER);
                    } else {
                        castList.add("NaN");
                    }

                    // Processa o campo "listed_in" em uma lista
                    List<String> listedInList = new ArrayList<>();
                    if (campos.length > 10 && !campos[10].equals("NaN")) {
                        String campoListed = campos[10].replaceAll("^\"|\"$", "");
                        String[] elementos = campoListed.split(",\\s*");
                        for (String s : elementos) {
                            listedInList.add(s);
                        }
                    } else {
                        listedInList.add("NaN");
                    }

                    // Processa a data, convertendo a string para objeto Date
                    Date data = null;
                    if (!campos[6].equals("NaN")) {
                        try {
                            String dataBruta = campos[6].replace("\"", "").trim(); // remove as aspas duplas e espaços
                                                                                   // extras
                            // System.out.println("Tentando converter data: " + dataBruta);
                            data = sdf.parse(dataBruta);
                        } catch (Exception e) {
                            // System.out.println("Erro ao converter data: " + campos[6]);
                            e.printStackTrace();
                            data = null;
                        }
                    }

                    // Converte release_year para int
                    int releaseYear = 0;
                    try {
                        if (!campos[7].equals("NaN"))
                            releaseYear = Integer.parseInt(campos[7]);
                    } catch (NumberFormatException nfe) {
                        releaseYear = 0;
                    }

                    // Cria um novo objeto Show com os dados extraídos
                    Show3 show = new Show3(
                            campos[0], // show_id
                            campos[1], // type
                            campos[2], // title
                            campos[3], // director
                            castList, // cast
                            campos[5], // country
                            data, // date_added
                            releaseYear, // release_year
                            campos[8], // rating
                            campos[9], // duration
                            listedInList // listed_in
                    );

                    shows.add(show);
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }

        return shows;
    }

    public void imprimirTodos(ArrayList<Show3> shows) {
        // Formato exato: November 12, 2019
        SimpleDateFormat sdf = new SimpleDateFormat("MMMM dd, yyyy", Locale.ENGLISH);

        for (Show3 s : shows) {
            String dataStr = (s.getDate_added() != null) ? sdf.format(s.getDate_added()) : "NaN";
            String castStr = (s.getCast() != null) ? s.getCast().toString() : "NaN";
            String listedInStr = (s.getListed_in() != null) ? s.getListed_in().toString() : "NaN";

            String linha ="=> " +s.getShow_id() + " ## " +
                    s.getTitle() + " ## " +
                    s.getType() + " ## " +
                    s.getDirector() + " ## " +
                    castStr + " ## " +
                    s.getCountry() + " ## " +
                    dataStr + " ## " +
                    s.getRelease_year() + " ## " +
                    s.getRating() + " ## " +
                    s.getDuration() + " ## " +
                    listedInStr + " ## ";

            System.out.println(linha);
        }
    }

    public static void main(String[] args) {
        // Cria uma instância de Show para chamar os métodos não estáticos
        Show3 showObj = new Show3();
        Scanner sc = new Scanner(System.in);

        do {

            String chave = sc.nextLine();

            if (chave.equals("FIM")) {
                break;
            }

            // Lê os registros do arquivo CSV
            ArrayList<Show3> shows = showObj.Ler(chave);

            // Imprime todos os objetos, cada um em uma única linha com atributos separados
            // por "##"
            showObj.imprimirTodos(shows);

        } while (true);

    }
}

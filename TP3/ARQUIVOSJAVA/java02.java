import java.io.BufferedReader;
import java.io.FileReader;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.*;

public class java02 {

    static class Show {
        private String showId;
        private String type;
        private String title;
        private String director;
        private String[] cast;
        private String country;
        private Date dataAdded;
        private int releaseYear;
        private String rating;
        private String duration;
        private String[] listedIn;

        public Show() {}

        public void ler(String linha) {
            List<String> campos = parseLinhaCSV(linha);
            this.showId = getOrNaN(campos, 0);
            this.type = getOrNaN(campos, 1);
            this.title = getOrNaN(campos, 2);
            this.director = getOrNaN(campos, 3);

            String castStr = getOrNaN(campos, 4);
            this.cast = castStr.equals("NaN") ? new String[0] : castStr.split(",\\s*");

            this.country = getOrNaN(campos, 5);

            String dateAddedStr = getOrNaN(campos, 6);
            try {
                if (!dateAddedStr.equals("NaN")) {
                    SimpleDateFormat sdf = new SimpleDateFormat("MMMM d, yyyy", Locale.ENGLISH);
                    this.dataAdded = sdf.parse(dateAddedStr);
                } else {
                    this.dataAdded = null;
                }
            } catch (ParseException e) {
                this.dataAdded = null;
            }

            String releaseYearStr = getOrNaN(campos, 7);
            this.releaseYear = releaseYearStr.equals("NaN") ? 0 : Integer.parseInt(releaseYearStr);

            this.rating = getOrNaN(campos, 8);
            this.duration = getOrNaN(campos, 9);

            String listedInStr = getOrNaN(campos, 10);
            this.listedIn = listedInStr.equals("NaN") ? new String[0] : listedInStr.split(",\\s*");
        }

        private String getOrNaN(List<String> lista, int index) {
            if (index >= lista.size() || lista.get(index).trim().isEmpty()) {
                return "NaN";
            }
            return lista.get(index).trim();
        }

        private List<String> parseLinhaCSV(String linha) {
            List<String> campos = new ArrayList<>();
            boolean emAspas = false;
            StringBuilder sb = new StringBuilder();

            for (int i = 0; i < linha.length(); i++) {
                char c = linha.charAt(i);
                if (c == '\"') {
                    emAspas = !emAspas;
                } else if (c == ',' && !emAspas) {
                    campos.add(sb.toString().replaceAll("\"", "").trim());
                    sb.setLength(0);
                } else {
                    sb.append(c);
                }
            }
            campos.add(sb.toString().replaceAll("\"", "").trim());
            return campos;
        }

        public String getShowId() {
            return showId;
        }

        public String getTitle() {
            return title;
        }

        public String formatar() {
            String[] castOrdenado = Arrays.copyOf(cast, cast.length);
            Arrays.sort(castOrdenado);

            String dataStr = (dataAdded == null) ? "NaN" :
                    new SimpleDateFormat("MMMM d, yyyy", Locale.ENGLISH).format(dataAdded);

            return String.format("=> %s ## %s ## %s ## %s ## [%s] ## %s ## %s ## %d ## %s ## %s ## [%s] ##",
                    showId,
                    title,
                    type,
                    director,
                    castOrdenado.length > 0 ? String.join(", ", castOrdenado) : "NaN",
                    country,
                    dataStr,
                    releaseYear,
                    rating,
                    duration,
                    listedIn.length > 0 ? String.join(", ", listedIn) : "NaN"
            );
        }
    }

    static class Pilha {
        private Show[] array;
        private int topo;

        public Pilha(int tamanho) {
            array = new Show[tamanho];
            topo = 0;
        }

        public void empilhar(Show show) throws Exception {
            if (topo >= array.length) throw new Exception("Erro: pilha cheia!");
            array[topo++] = show;
        }

        public Show desempilhar() throws Exception {
            if (topo == 0) throw new Exception("Erro: pilha vazia!");
            return array[--topo];
        }

        public void mostrar() {
            for (int i = topo - 1; i >= 0; i--) {
                System.out.println(array[i].formatar());
            }
        }
    }

    public static Map<String, Show> lerCSV(String caminho) {
        Map<String, Show> mapa = new HashMap<>();
        try {
            BufferedReader br = new BufferedReader(new FileReader(caminho));
            String linha = br.readLine(); // pula cabeçalho
            while ((linha = br.readLine()) != null) {
                Show s = new Show();
                s.ler(linha);
                mapa.put(s.getShowId(), s);
            }
            br.close();
        } catch (Exception e) {
            System.out.println("Erro ao ler arquivo: " + e.getMessage());
        }
        return mapa;
    }

    static String caminhoCSV = "/tmp/disneyplus.csv";

    static String lerLinhaPorArquivo(String showId) throws Exception {
        BufferedReader br = new BufferedReader(new FileReader(caminhoCSV));
        br.readLine(); // pular cabeçalho
        String linha;
        while ((linha = br.readLine()) != null) {
            if (linha.startsWith(showId + ",")) {
                br.close();
                return linha;
            }
        }
        br.close();
        throw new Exception("Show não encontrado: " + showId);
    }

    public static void main(String[] args) throws Exception {
        Scanner sc = new Scanner(System.in);

        Map<String, Show> mapaShows = lerCSV(caminhoCSV);

        Pilha pilha = new Pilha(1000);

        while (true) {
            String linha = sc.nextLine().trim();
            if (linha.equals("FIM")) break;

            String id = linha;
            Show s = mapaShows.get(id);
            if (s != null) {
                pilha.empilhar(s);
            }
        }

        int n = Integer.parseInt(sc.nextLine().trim());

        for (int i = 0; i < n; i++) {
            String comando = sc.nextLine().trim();
            if (comando.startsWith("I ")) {
                String id = comando.substring(2).trim();
                String linhaShow = lerLinhaPorArquivo(id);
                Show s = new Show();
                s.ler(linhaShow);
                pilha.empilhar(s);
            } else if (comando.equals("R")) {
                Show removido = pilha.desempilhar();
                System.out.println("(R) " + removido.getTitle());
            }
        }

        pilha.mostrar();
        sc.close();
    }
}

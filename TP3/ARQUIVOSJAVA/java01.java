import java.io.BufferedReader;
import java.io.FileReader;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.*;

public class TP02_18 {

    static class Show {
        private String showId;
        private String type;
        private String title;
        private String director;
        private String[] cast;
        private String country;
        private Date dataAdded;  // Para ordenação
        private int releaseYear;
        private String rating;
        private String duration;
        private String listedIn;
        private String description;

        public Show() {}

        public void ler(String linha) {
            List<String> campos = parseLinhaCSV(linha);
            SimpleDateFormat sdf = new SimpleDateFormat("MMMM d, yyyy", Locale.ENGLISH);

            this.showId = getOrNaN(campos, 0);
            this.type = getOrNaN(campos, 1);
            this.title = getOrNaN(campos, 2);
            this.director = getOrNaN(campos, 3);

            String elenco = getOrNaN(campos, 4);
            this.cast = elenco.equals("NaN") ? new String[0] : elenco.split(",\\s*");

            this.country = getOrNaN(campos, 5);

            try {
                String data = getOrNaN(campos, 6);
                this.dataAdded = data.equals("NaN") ? new Date(0) : sdf.parse(data);
            } catch (ParseException e) {
                this.dataAdded = new Date(0);
            }

            String relYear = getOrNaN(campos, 7);
            if (relYear.equals("NaN")) {
                this.releaseYear = 0;
            } else {
                try {
                    this.releaseYear = Integer.parseInt(relYear);
                } catch (NumberFormatException e) {
                    this.releaseYear = 0;
                }
            }

            this.rating = getOrNaN(campos, 8);
            this.duration = getOrNaN(campos, 9);
            this.listedIn = getOrNaN(campos, 10);
            this.description = getOrNaN(campos, 11);
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

        public Date getDataAdded() {
            return dataAdded;
        }

        public String getTitle() {
            return title;
        }

        public String formatar() {
            String[] castOrdenado = Arrays.copyOf(cast, cast.length);
            Arrays.sort(castOrdenado);

            return String.format("=> %s ## %s ## %s ## %s ## [%s] ## %s ## %tB %<te, %<tY ## %d ## %s ## %s ## [%s] ##",
                    showId,
                    title,
                    type,
                    director,
                    castOrdenado.length > 0 ? String.join(", ", castOrdenado) : "NaN",
                    country,
                    dataAdded,
                    releaseYear,
                    rating,
                    duration,
                    listedIn
            );
        }
    }

    static class ListaShow {
        private Show[] array;
        private int n;

        public ListaShow(int tamanho) {
            array = new Show[tamanho];
            n = 0;
        }

        public void inserirInicio(Show show) throws Exception {
            if (n >= array.length) throw new Exception("Lista cheia");
            for (int i = n; i > 0; i--) {
                array[i] = array[i-1];
            }
            array[0] = show;
            n++;
        }

        public void inserirFim(Show show) throws Exception {
            if (n >= array.length) throw new Exception("Lista cheia");
            array[n++] = show;
        }

        public void inserir(Show show, int pos) throws Exception {
            if (n >= array.length || pos < 0 || pos > n) throw new Exception("Posição inválida ou lista cheia");
            for (int i = n; i > pos; i--) {
                array[i] = array[i-1];
            }
            array[pos] = show;
            n++;
        }

        public Show removerInicio() throws Exception {
            if (n == 0) throw new Exception("Lista vazia");
            Show resp = array[0];
            for (int i = 0; i < n - 1; i++) {
                array[i] = array[i+1];
            }
            n--;
            return resp;
        }

        public Show removerFim() throws Exception {
            if (n == 0) throw new Exception("Lista vazia");
            return array[--n];
        }

        public Show remover(int pos) throws Exception {
            if (n == 0 || pos < 0 || pos >= n) throw new Exception("Posição inválida ou lista vazia");
            Show resp = array[pos];
            for (int i = pos; i < n - 1; i++) {
                array[i] = array[i+1];
            }
            n--;
            return resp;
        }

        public void mostrar() {
            for (int i = 0; i < n; i++) {
                System.out.println(array[i].formatar());
            }
        }
    }

    // Leitura do CSV completo, já retornando mapa de Show por ID
    public static Map<String, Show> lerCSV(String caminho) {
        Map<String, Show> mapa = new HashMap<>();
        try {
            BufferedReader br = new BufferedReader(new FileReader(caminho));
            String linha = br.readLine(); // pula cabeçalho
            while ((linha = br.readLine()) != null) {
                Show s = new Show();
                s.ler(linha);
                mapa.put(s.showId, s);
            }
            br.close();
        } catch (Exception e) {
            System.out.println("Erro ao ler arquivo: " + e.getMessage());
        }
        return mapa;
    }

    public static void main(String[] args) throws Exception {
        Scanner sc = new Scanner(System.in);
        Map<String, Show> mapaShows = lerCSV("/tmp/disneyplus.csv");

        // Primeira parte: ler IDs e adicionar na lista em ordem
        ListaShow lista = new ListaShow(1000);
        while (true) {
            String id = sc.nextLine().trim();
            if (id.equals("FIM")) break;
            Show s = mapaShows.get(id);
            if (s != null) {
                lista.inserirFim(s);
            }
        }

        // Segunda parte: ler quantidade de comandos
        int n = Integer.parseInt(sc.nextLine().trim());
        for (int i = 0; i < n; i++) {
            String linha = sc.nextLine();
            String[] partes = linha.split(" ");

            switch(partes[0]) {
                case "II": { // inserir inicio
                    String arquivo = partes[1];
                    Show s = new Show();
                    s.ler(lerLinhaPorArquivo(arquivo));
                    lista.inserirInicio(s);
                    break;
                }
                case "IF": { // inserir fim
                    String arquivo = partes[1];
                    Show s = new Show();
                    s.ler(lerLinhaPorArquivo(arquivo));
                    lista.inserirFim(s);
                    break;
                }
                case "I*": { // inserir na posicao p
                    int pos = Integer.parseInt(partes[1]);
                    String arquivo = partes[2];
                    Show s = new Show();
                    s.ler(lerLinhaPorArquivo(arquivo));
                    lista.inserir(s, pos);
                    break;
                }
                case "RI": { // remover inicio
                    Show removido = lista.removerInicio();
                    System.out.println("(R) " + removido.getTitle());
                    break;
                }
                case "RF": { // remover fim
                    Show removido = lista.removerFim();
                    System.out.println("(R) " + removido.getTitle());
                    break;
                }
                case "R*": { // remover na posicao p
                    int pos = Integer.parseInt(partes[1]);
                    Show removido = lista.remover(pos);
                    System.out.println("(R) " + removido.getTitle());
                    break;
                }
            }
        }

        lista.mostrar();
        sc.close();
    }

    // Função que lê uma linha do CSV dado um arquivo (o nome do arquivo é o showId no enunciado)
    // Como o enunciado pediu usar nome do arquivo, entendi que deve abrir o csv e buscar linha com showId = nomeArquivo
    static String lerLinhaPorArquivo(String showId) throws Exception {
        BufferedReader br = new BufferedReader(new FileReader("/tmp/disneyplus.csv"));
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
}

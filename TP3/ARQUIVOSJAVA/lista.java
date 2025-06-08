package TP3.ARQUIVOSJAVA;

public class lista {

    private class No {
        public int elemento;
        public No prox;

        public No(int e) {
            elemento = e;
            prox = null;
        }
    }

    private No primeiro;
    private No ultimo;

    public lista() {
        this.primeiro = null;
        this.ultimo = null;
    }

    public void inserirFim(int elemento) {
        if (primeiro == null) {
            primeiro = new No(elemento);
            ultimo = primeiro;
        } else {
            No tmp = new No(elemento);
            ultimo.prox = tmp;
            ultimo = tmp;
        }
    }

    public void inserirInicio(int elemento) {
        No tmp = new No(elemento);
        if (primeiro == null) {
            primeiro = tmp;
            ultimo = tmp;
        } else {
            tmp.prox = primeiro;
            primeiro = tmp;
        }
    }

    public void inserirPosicao(int elemento, int pos) throws Exception {
        if (pos < 0 || (primeiro == null && pos != 0)) {
            throw new Exception("Posição inválida!");
        }
        if (pos == 0) {
            inserirInicio(elemento);
        } else {
            No i = primeiro;
            int count = 0;
            while (count < pos - 1 && i != null) {
                i = i.prox;
                count++;
            }
            if (i == null) {
                throw new Exception("Posição inválida!");
            }
            No tmp = new No(elemento);
            tmp.prox = i.prox;
            i.prox = tmp;
            if (tmp.prox == null) {
                ultimo = tmp;
            }
        }
    }

    public void mostrar() {
        No i = primeiro;
        while (i != null) {
            System.out.print(i.elemento + " ");
            i = i.prox;
        }
        System.out.println();
    }
}



import java.util.Scanner;

public class Soma{

    public static int fatorial(String n,int i) {

        if (i >= n.length()) {
            return 0;
        }

        int n2 = n.charAt(i) - '0';
        
        return n2 + fatorial(n,i+1);
    }

    public static void main(String[] args) {
      Scanner sc = new Scanner(System.in);

      do{

      String n = sc.nextLine();

      if(n.equals("FIM")){
        break;
      }

      int result  = fatorial(n,0);

      System.out.println(result);


      }while(true);


      sc.close();



  
    }
}
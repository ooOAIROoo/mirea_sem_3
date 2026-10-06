import java.util.Scanner;

public class task2 {
    public static void main(String[] args) {
        final double ROUBLES_PER_YUAN = 11.91;
        int yuen,digit,lastTwo;
        double roubles;
        
        Scanner input = new Scanner(System.in);
        
        System.out.print("Введите сумму денег в китайских юанях: ");
        yuen = input.nextInt();
        
        roubles = ROUBLES_PER_YUAN * yuen;
        System.out.println("Сумма в рублях: " + roubles);
        
        digit = yuen % 10;
        lastTwo = yuen % 100;
        
        String end;
        if (lastTwo >= 11 && lastTwo <= 14)
        end = "китайских юаней";
        
        else if (digit == 1)
        end = "китайский юань";
        
        else if (digit > 1 && digit < 5)
        end = "китайских юаня";
        
        else
        end = "китайских юаней";
        
        System.out.println(yuen + " " + end + " = " + roubles + "российских рубля");
        input.close();
    }
} 

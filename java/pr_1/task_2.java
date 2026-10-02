import java.util.Scanner;

public class task_2 {
    public static void main(String[] args) {
        final double roubles_per_yuan = 11.91;
        int yuan, digit;
        double roubles;

        Scanner input = new Scanner(System.in);

        System.out.println("Введите количество юаней: ");
        yuan = input.nextInt();

        roubles = roubles_per_yuan * yuan;

        digit = yuan % 10;

        if (digit == 1) {
            System.out.println("Юань");
        }
        
        else if (digit == 2 || digit == 3 || digit == 4) {
            System.out.println("Юаня");
        }

        else {
            System.out.println("Юаней");
        }   
    }
}

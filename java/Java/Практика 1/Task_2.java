import java.util.Scanner;

public class Task_2 {
    public static void main(String[] args) {
        final double ROUBLES_PER_YUAN = 11.91;
        int yuan;
        int digit;
        double roubles;
        Scanner input = new Scanner(System.in);
        System.out.print("Enter amount of yuan: ");
        yuan = input.nextInt();
        roubles = ROUBLES_PER_YUAN * yuan;
        digit = yuan % 10;
        if (digit == 1)
            System.out.print("Юань");
        else if (digit== 2 || digit == 3 || digit == 4)
            System.out.print("Юаня");
        else 
            System.out.print("Юаней");
    }
}

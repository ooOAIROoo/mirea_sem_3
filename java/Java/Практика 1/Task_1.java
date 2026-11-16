import java.util.Scanner;

public class Task_1 {
    public static void main(String[] args) {
        final double ROUBLES_PER_YUAN = 11.91;
        int yuan;
        double roubles;

        Scanner input = new Scanner(System.in);

        System.out.print("Введите колчиство юаней: ");
        yuan = input.nextInt();
        roubles = ROUBLES_PER_YUAN * yuan;
        System.out.print("Перевод ");
        System.out.print(roubles);
    }
}

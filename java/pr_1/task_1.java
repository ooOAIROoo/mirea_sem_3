import java.util.Scanner;

public class task_1 {
    public static void main(String[] args) {
        int yuan;
        final double rub_per_yuan = 11.91;

        Scanner input = new Scanner(System.in);

        System.out.println("Введите количество юаней: ");
        yuan = input.nextInt();

        int roubles = (int) Math.ceil(rub_per_yuan * yuan);

        System.out.println("Перевод ");
        System.out.println(roubles);
    }
}

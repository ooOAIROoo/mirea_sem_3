import java.util.Scanner;

public class task_5 {
    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);

        System.out.print("Введите число для вычисления факториала: ");
        int n = input.nextInt();

        // Вычисление факториала через цикл do-while
        long factorial = 1;
        int i = 1;
        do {
            factorial *= i;
            i++;
        } while (i <= n);

        System.out.println(n + "! = " + factorial);
        input.close();
    }
}

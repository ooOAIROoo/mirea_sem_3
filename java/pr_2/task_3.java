public class task_3 {
    public static void main(String[] args) {
        // Первые 10 чисел гармонического ряда: 1 + 1/2 + 1/3 + ... + 1/10
        double sum = 0;
        for (int i = 1; i <= 10; i++) {
            System.out.printf("%2d-й член гармонического ряда: %.4f%n", i, 1.0 / i);
            sum += 1.0 / i;
        }
        System.out.printf("Сумма первых 10 членов: %.4f%n", sum);
    }
}

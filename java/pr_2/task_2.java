public class task_2 {
    public static void main(String[] args) {
        int[] arr = {4, 8, 15, 16, 23, 42, 7, 11};

        // 1) сумма через цикл for
        int sumFor = 0;
        for (int i = 0; i < arr.length; i++) {
            sumFor += arr[i];
        }

        // 2) сумма через цикл while
        int sumWhile = 0;
        int i = 0;
        while (i < arr.length) {
            sumWhile += arr[i];
            i++;
        }

        // 3) сумма через цикл do-while
        int sumDoWhile = 0;
        i = 0;
        do {
            sumDoWhile += arr[i];
            i++;
        } while (i < arr.length);

        System.out.print("Массив: ");
        for (int x : arr) {
            System.out.print(x + " ");
        }
        System.out.println();
        System.out.println("Сумма (цикл for): " + sumFor);
        System.out.println("Сумма (цикл while): " + sumWhile);
        System.out.println("Сумма (цикл do-while): " + sumDoWhile);
    }
}

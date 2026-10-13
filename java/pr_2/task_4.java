import java.util.Arrays;
import java.util.Random;

public class task_4 {
    public static void main(String[] args) {
        final int N = 10;
        int[] arr = new int[N];
        Random random = new Random();

        // Заполнение массива случайными числами [0, 100)
        for (int i = 0; i < N; i++) {
            arr[i] = random.nextInt(100);
        }

        System.out.println("Исходный массив: " + Arrays.toString(arr));

        // Сортировка вставками
        for (int i = 1; i < N; i++) {
            int key = arr[i];
            int j = i - 1;
            while (j >= 0 && arr[j] > key) {
                arr[j + 1] = arr[j];
                j--;
            }
            arr[j + 1] = key;
        }

        System.out.println("Отсортированный:  " + Arrays.toString(arr));
    }
}

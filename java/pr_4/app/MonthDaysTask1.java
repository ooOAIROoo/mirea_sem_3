package app;

import java.util.InputMismatchException;
import java.util.Scanner;

/**
 * Часть 2. Задание 1.
 * Вывод названия месяца и числа дней по номеру 1..12 с обработкой
 * ArrayIndexOutOfBoundsException и защитой от ввода нецелого числа.
 */
public class MonthDaysTask1 {
    public static void main(String[] args) {
        String[] months = {
            "январь", "февраль", "март", "апрель", "май", "июнь",
            "июль", "август", "сентябрь", "октябрь", "ноябрь", "декабрь"
        };
        int[] dom = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

        Scanner scanner = new Scanner(System.in);
        System.out.print("Введите целое число от 1 до 12: ");

        try {
            int monthNumber = scanner.nextInt();
            String monthName = months[monthNumber - 1];
            int days = dom[monthNumber - 1];
            System.out.println("Месяц: " + monthName + ", количество дней: " + days);
        } catch (ArrayIndexOutOfBoundsException ex) {
            System.out.println("Недопустимое число");
        } catch (InputMismatchException ex) {
            System.out.println("Ошибка ввода: необходимо ввести целое число.");
        } finally {
            scanner.close();
        }
    }
}

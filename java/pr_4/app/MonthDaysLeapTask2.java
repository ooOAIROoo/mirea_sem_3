package app;

import java.util.InputMismatchException;
import java.util.Scanner;

/**
 * Часть 2. Задание 2.
 * Вывод месяца и количества дней с учётом високосного года для февраля.
 */
public class MonthDaysLeapTask2 {
    public static boolean isLeapYear(int year) {
        return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    }

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

            if (monthNumber == 2) {
                System.out.print("Введите год: ");
                int year = scanner.nextInt();
                if (year <= 0) {
                    System.out.println("Недопустимый год");
                    return;
                }
                boolean leap = isLeapYear(year);
                if (leap) {
                    days = 29;
                }
                System.out.println("Год " + year + (leap ? " — високосный." : " — невисокосный."));
            }

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

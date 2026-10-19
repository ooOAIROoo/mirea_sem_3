package app;

import vehicles.Car_t1;

/**
 * Часть 1. Задача #1.
 * Демонстрация работы класса Car с приватными полями ownerName, insuranceNumber
 * и защищённым полем engineType через геттеры и сеттеры.
 */
public class task_1 {
    public static void main(String[] args) {
        System.out.println("=== Часть 1. Задача #1 ===");

        // 1. Создание объекта конструктором по умолчанию
        Car_t1 car1 = new Car_t1();
        System.out.println("Автомобиль 1 (значения по умолчанию):");
        car1.printInfo();

        // Изменение значений закрытых полей через сеттеры
        car1.setOwnerName("Иванов Иван");
        car1.setInsuranceNumber("INS-001-77");
        car1.setEngineType("Petrol");

        System.out.println("Автомобиль 1 (после изменения через сеттеры):");
        System.out.println("Владелец: " + car1.getOwnerName());
        System.out.println("Страховой номер: " + car1.getInsuranceNumber());
        System.out.println("Тип двигателя: " + car1.getEngineType());
        System.out.println("------------------------------------------");

        // 2. Создание объекта конструктором с параметрами
        Car_t1 car2 = new Car_t1("Петров Пётр", "INS-002-99", "Diesel");
        System.out.println("Автомобиль 2 (создан через конструктор с параметрами):");
        car2.printInfo();
    }
}

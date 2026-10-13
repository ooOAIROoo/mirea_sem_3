# Практическая работа №2 — Java

Решение задания по классам, наследованию, инкапсуляции и полиморфизму.

- `vehicles.Vehicle` — абстрактный базовый класс с общими полями и геттерами/сеттерами.
- `vehicles.Car` — автомобиль, наследующий `Vehicle`.
- `vehicles.ElectricCar` — электромобиль, наследующий `Car`.
- `app.TestCar` — демонстрация изменения свойств через методы и полиморфного вызова `toString()`.

Сборка и запуск из каталога `java/pr_2`:

```sh
javac -d /tmp/pr2-classes vehicles/*.java app/*.java
java -cp /tmp/pr2-classes app.TestCar
```

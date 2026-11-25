# Практическая работа №7.1 — Красно-чёрное дерево (вариант 3)

## Вариант

- **Тип ключа:** символ (`char`)
- **Дерево:** красно-чёрное
- **Операции:** вставка с балансировкой, прямой обход (preorder), симметричный обход (inorder), сумма значений листьев, среднее арифметическое всех узлов

## Структура

| Путь | Назначение |
|---|---|
| `include/rbtree.hpp` | Интерфейс красно-чёрного дерева |
| `src/rbtree.cpp` | Реализация: повороты, балансировка, обходы, вычисления |
| `src/main.cpp` | Консольное меню, ввод, демонстрация |
| `tests/rbtree_tests.cpp` | Автоматические тесты |
| `CMakeLists.txt` | Сборка CMake |
| `report.md` | **Полный отчёт и пояснение**: постановка, алгоритмы, архитектура, тесты, ответы на контрольные вопросы |

## Сборка и запуск

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/pr3_part1 --demo
```

Без CMake:

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -Iinclude src/rbtree.cpp src/main.cpp -o pr3_part1
./pr3_part1 --demo
```

Тесты:

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -Iinclude src/rbtree.cpp tests/rbtree_tests.cpp -o rbtree_tests
./rbtree_tests
```

## Что читать

Для защиты/понимания работы откройте `report.md` — там описано что сделано, как устроено, как работает балансировка, ответы на контрольные вопросы и разбор тестов.

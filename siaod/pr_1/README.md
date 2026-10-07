# Практическая работа № 5 (каталог `pr_1`)

Реализация варианта 4 задания «Поиск по ключу»: записи об автомобилях, ключ — номер автомобиля. Подробное описание и результаты приведены в [`report_pr_1.md`](report_pr_1.md).

## Состав проекта

- `task_1/` — генерация текстового файла, его преобразование в бинарный файл и просмотр записей;
- `task_2/` — последовательный (линейный) поиск в бинарном файле;
- `task_3/` — индекс `номер → байтовое смещение`, поиск Фибоначчи в индексе и чтение найденной записи по смещению;
- `common/` — общие классы и функции;
- `tools/analyze.py` — чтение CSV средствами pandas, сравнение и построение PNG-графика средствами matplotlib;
- `tests/` — автоматическая проверка формата файла и поиска.

## Сборка и запуск

Нужны компилятор C++17 и CMake 3.16 или новее. Все команды сборки кроссплатформенные; запускайте их из каталога `siaod/pr_1`.

```text
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

Linux/macOS:

```bash
./build/task_1_app
./build/task_2_app
./build/task_3_app
```

Windows PowerShell (Visual Studio generator):

```powershell
.\build\Release\task_1_app.exe
.\build\Release\task_2_app.exe
.\build\Release\task_3_app.exe
```

Если используется одноконфигурационный генератор, исполняемые файлы могут находиться непосредственно в `build/`. Каждая программа также поддерживает `--help`.

### Демонстрация генерации и поиска

```bash
# Создать 100 записей: сначала data/records.txt, затем data/records.bin
./build/task_1_app --generate 100 2026
./build/task_1_app --show 5
# В этой Linux-проверке seed=2026 дал номер K647KC29:
./build/task_2_app --search K647KC29
./build/task_3_app --search K647KC29
```

`K647KC29` — ключ из демонстрационного набора с seed `2026` в проверенной Linux-сборке. В интерактивном режиме номер можно ввести с клавиатуры. При отличии стандартной библиотеки C++ генератор может выдать другие псевдослучайные ключи; в таком случае используйте номер из вывода `--show`. Для Windows PowerShell запускайте exe из `build/Release/`, например `.\build\Release\task_1_app.exe` (путь начинается с `.` и обратной косой черты). Если генератор CMake одноконфигурационный, exe может находиться непосредственно в `build/`. Формат номера — 8 латинских символов, например `A123BC77`.

### Замеры и построение графика

Из каталога `siaod/pr_1` запустите:

```bash
./build/task_2_app --benchmark
./build/task_3_app --benchmark
python3 -m venv .venv
.venv/bin/python -m pip install -r requirements.txt
.venv/bin/python tools/analyze.py
```

В Windows PowerShell создайте окружение `py -m venv .venv`, затем используйте команды:

```powershell
.\.venv\Scripts\python.exe -m pip install -r requirements.txt
.\.venv\Scripts\python.exe tools/analyze.py
```

Для приложений C++ используйте соответствующий путь к exe из `build/Release/`. Виртуальное окружение игнорируется локальным `.gitignore`.

Две C++-программы сохраняют сырые результаты в `results/linear_search.csv` и `results/fibonacci_search.csv`. Скрипт формирует `results/comparison.csv` и `results/search_comparison.png`. Эти файлы, тестовые наборы и сборка намеренно исключены из Git локальным `.gitignore`; их можно заново получить перечисленными командами.

### Автотесты

```bash
ctest --test-dir build --output-on-failure
```

Данные и результаты тестов создаются только во временном каталоге ОС.

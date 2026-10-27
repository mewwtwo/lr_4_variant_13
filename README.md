# ЛР 4, варіант №13

Два цілі поля (`m=2`). Частоти: поле 0 - read 10%, write 10%; поле 1 - read 50%, write 10%; string - 20%.

Програма читає операції з файлів і вимірює час роботи з 1, 2 та 3 потоками. Кожен потік має свій файл і свою структуру даних. Обидва поля захищені окремими м’ютексами, а для `string` блокуються обидва.

## Структура

| Файл | Для чого |
|---|---|
| `src/fields.hpp`, `src/fields.cpp` | Два поля, читання, запис, отримання рядка, м’ютекси |
| `src/main.cpp` | Читання тестових файлів, потоки, вимірювання часу |
| `src/create_input.cpp` | Генерація нових тестів |
| `input/` | По три файли для кожного типу операцій |
| `output/output_results.txt` | Результати 9 запусків |
| `report.pdf` | Звіт |
| `docs/workflow.png` | Схема роботи |

## Збирання

Потрібні CMake 3.16+ та компілятор C++17.

```bash
cmake -S . -B build
cmake --build build
```

## Запуск

macOS / Linux:

```bash
./build/run_tests 1 input/input_expected_1.txt
./build/run_tests 2 input/input_expected_1.txt input/input_expected_2.txt
./build/run_tests 3 input/input_expected_1.txt input/input_expected_2.txt input/input_expected_3.txt
```

Windows (залежно від генератора CMake): `build\Debug\run_tests.exe` або `build\run_tests.exe`.

## Генерація тестового файлу

```bash
./build/create_input expected 100000 42 input/input_new.txt
```

Параметри: тип (`expected`, `even`, `mostly_string`), кількість операцій (додатне ціле число), seed, шлях. Кожен потік обробляє однакову кількість операцій; сумарна робота для 1/2/3 потоків різна. Тому часи відображають пропускну здатність і накладні витрати, а не чисте прискорення однакової задачі.

## Перевірка невеликого прикладу

Файл `input/input_check.txt` містить шість команд з передбачуваними результатами.
Після запису `15` у поле 0 та `7` у поле 1 сума значень трьох `read` дорівнює 37, а довжина рядка `15 7` дорівнює 4. Отже, очікуваний `checksum` - **41**.

```bash
./build/run_tests 1 input/input_check.txt
```

`time_ms` залежить від комп’ютера, але `operations=6 checksum=41 status=OK` має бути незмінним.

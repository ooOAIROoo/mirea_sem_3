if(NOT DEFINED APP OR NOT DEFINED TASK1_INPUT OR NOT DEFINED TASK2_INPUT OR
   NOT DEFINED EOF_INPUT)
    message(FATAL_ERROR "APP, TASK1_INPUT, TASK2_INPUT and EOF_INPUT are required")
endif()

execute_process(
    COMMAND "${APP}"
    INPUT_FILE "${TASK1_INPUT}"
    OUTPUT_VARIABLE task1_output
    ERROR_VARIABLE task1_error
    RESULT_VARIABLE task1_result
    ENCODING UTF-8
    TIMEOUT 15
)

if(NOT task1_result STREQUAL "0")
    message(FATAL_ERROR "Task 1 CLI exited with ${task1_result}: ${task1_error}\n${task1_output}")
endif()

foreach(expected
        "Подходящих слов нет"
        "«Ароза»"
        "«Шалаш»"
        "Букв в слове: 5"
        "«а»")
    string(FIND "${task1_output}" "${expected}" position)
    if(position EQUAL -1)
        message(FATAL_ERROR "Task 1 output does not contain: ${expected}\n${task1_output}")
    endif()
endforeach()

execute_process(
    COMMAND "${APP}"
    INPUT_FILE "${TASK2_INPUT}"
    OUTPUT_VARIABLE task2_output
    ERROR_VARIABLE task2_error
    RESULT_VARIABLE task2_result
    ENCODING UTF-8
    TIMEOUT 15
)

if(NOT task2_result STREQUAL "0")
    message(FATAL_ERROR "Task 2 CLI exited with ${task2_result}: ${task2_error}\n${task2_output}")
endif()

# Разбор примеров КМП совпадает с отчётом: индексы и число сравнений.
foreach(expected
        "Префикс-функция образца (по байтам UTF-8): 0 1 2 0"
        "Все вхождения (номера символов, с нуля): 3"
        "Первое вхождение (КМП): 3 (сравнений: 9)"
        "Первое вхождение (наивный алгоритм): 3 (сравнений: 10)"
        "Последнее вхождение (КМП): вхождений нет (сравнений: 8)"
        "Поле не может быть пустым"
        "Последнее вхождение (КМП): символ 14 (байт 25, сравнений: 39)")
    string(FIND "${task2_output}" "${expected}" position)
    if(position EQUAL -1)
        message(FATAL_ERROR "Task 2 output does not contain: ${expected}\n${task2_output}")
    endif()
endforeach()

# Готовые примеры и справка работают без ввода.
execute_process(
    COMMAND "${APP}" --demo
    OUTPUT_VARIABLE demo_output
    ERROR_VARIABLE demo_error
    RESULT_VARIABLE demo_result
    ENCODING UTF-8
    TIMEOUT 15
)
if(NOT demo_result STREQUAL "0")
    message(FATAL_ERROR "--demo exited with ${demo_result}: ${demo_error}")
endif()
foreach(expected
        "первое вхождение (КМП): 3 (сравнений: 9)"
        "последнее вхождение (КМП): 8"
        "подходящих слов нет")
    string(FIND "${demo_output}" "${expected}" position)
    if(position EQUAL -1)
        message(FATAL_ERROR "--demo output does not contain: ${expected}\n${demo_output}")
    endif()
endforeach()

execute_process(
    COMMAND "${APP}" --help
    OUTPUT_VARIABLE help_output
    RESULT_VARIABLE help_result
    ENCODING UTF-8
    TIMEOUT 15
)
if(NOT help_result STREQUAL "0")
    message(FATAL_ERROR "--help exited with ${help_result}")
endif()
string(FIND "${help_output}" "--bench" position)
if(position EQUAL -1)
    message(FATAL_ERROR "--help output does not describe --bench\n${help_output}")
endif()

# Конец потока во время ввода и неизвестный параметр не должны зависать.
execute_process(
    COMMAND "${APP}"
    INPUT_FILE "${EOF_INPUT}"
    RESULT_VARIABLE eof_result
    OUTPUT_QUIET ERROR_QUIET TIMEOUT 15
)
if(NOT eof_result STREQUAL "0")
    message(FATAL_ERROR "EOF during input should terminate normally: ${eof_result}")
endif()
execute_process(
    COMMAND "${APP}" --invalid
    RESULT_VARIABLE invalid_result
    OUTPUT_QUIET ERROR_QUIET TIMEOUT 15
)
if(NOT invalid_result STREQUAL "2")
    message(FATAL_ERROR "Unknown option should return 2: ${invalid_result}")
endif()

message(STATUS "CLI workflows passed")

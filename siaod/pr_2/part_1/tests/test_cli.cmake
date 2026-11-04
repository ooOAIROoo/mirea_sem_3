if(NOT DEFINED APP OR NOT DEFINED INPUT OR NOT DEFINED EMPTY_INPUT OR
   NOT DEFINED COMPACT_INPUT OR NOT DEFINED EOF_INPUT)
    message(FATAL_ERROR "APP, INPUT, EMPTY_INPUT, COMPACT_INPUT and EOF_INPUT are required")
endif()

execute_process(
    COMMAND "${APP}"
    INPUT_FILE "${INPUT}"
    OUTPUT_VARIABLE output
    ERROR_VARIABLE error_output
    RESULT_VARIABLE result
    ENCODING UTF-8
    TIMEOUT 15
)

if(NOT result STREQUAL "0")
    message(FATAL_ERROR "CLI exited with ${result}: ${error_output}\n${output}")
endif()

foreach(expected
        "Автоматически добавлено 6"
        "Запись удалена"
        "Уже существует студент"
        "Расширение и рехеширование: 11 -> 23"
        "Индекс ячейки | Состояние | Ключ | h1 | h2 | Индекс записи")
    string(FIND "${output}" "${expected}" position)
    if(position EQUAL -1)
        message(FATAL_ERROR "CLI output does not contain: ${expected}\n${output}")
    endif()
endforeach()

# Ключ 1098 найден до удаления столкнувшегося с ним ключа и после удаления.
string(REGEX MATCHALL "Найден студент" found_messages "${output}")
list(LENGTH found_messages found_count)
if(NOT found_count EQUAL 2)
    message(FATAL_ERROR "Expected 2 successful searches, got ${found_count}\n${output}")
endif()

execute_process(
    COMMAND "${APP}" --empty
    INPUT_FILE "${EMPTY_INPUT}"
    OUTPUT_VARIABLE empty_output
    ERROR_VARIABLE empty_error
    RESULT_VARIABLE empty_result
    ENCODING UTF-8
    TIMEOUT 15
)
if(NOT empty_result STREQUAL "0")
    message(FATAL_ERROR "Empty-table CLI exited with ${empty_result}: ${empty_error}")
endif()
foreach(expected
        "Запуск с пустой таблицей"
        "Записей нет"
        "Поле не может быть пустым"
        "Студент добавлен"
        "Номер зачётной книжки: 0001"
        "Номер группы: Группа 1"
        "Иванова Мария Ивановна"
        "Запись удалена"
        "Неизвестный пункт меню")
    string(FIND "${empty_output}" "${expected}" position)
    if(position EQUAL -1)
        message(FATAL_ERROR "Empty-table CLI output does not contain: ${expected}\n${empty_output}")
    endif()
endforeach()
string(REGEX MATCHALL "не найден" missing_messages "${empty_output}")
list(LENGTH missing_messages missing_count)
if(NOT missing_count EQUAL 3)
    message(FATAL_ERROR "Expected 3 missing-key messages, got ${missing_count}\n${empty_output}")
endif()
string(REGEX MATCHALL "Поле не может быть пустым" empty_field_messages "${empty_output}")
list(LENGTH empty_field_messages empty_field_count)
if(NOT empty_field_count EQUAL 3)
    message(FATAL_ERROR "Expected 3 empty-field retries, got ${empty_field_count}\n${empty_output}")
endif()

# При накоплении меток выполняется рехеширование без роста.
execute_process(
    COMMAND "${APP}" --empty
    INPUT_FILE "${COMPACT_INPUT}"
    OUTPUT_VARIABLE compact_output
    ERROR_VARIABLE compact_error
    RESULT_VARIABLE compact_result
    ENCODING UTF-8
    TIMEOUT 15
)
if(NOT compact_result STREQUAL "0")
    message(FATAL_ERROR "Compaction CLI exited with ${compact_result}: ${compact_error}")
endif()
foreach(expected
        "Рехеширование без расширения: метки удаления очищены, размер таблицы 11"
        "Студенты: 5 | ячеек: 11 | меток удаления: 0"
        "Найден студент"
        "Номер зачётной книжки: 4")
    string(FIND "${compact_output}" "${expected}" position)
    if(position EQUAL -1)
        message(FATAL_ERROR "Compaction CLI output does not contain: ${expected}\n${compact_output}")
    endif()
endforeach()
string(FIND "${compact_output}" "Расширение и рехеширование:" grew)
if(NOT grew EQUAL -1)
    message(FATAL_ERROR "Compaction scenario should not grow the table\n${compact_output}")
endif()

# Конец потока во время ввода и неизвестный параметр не должны зависать.
execute_process(
    COMMAND "${APP}" --empty
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

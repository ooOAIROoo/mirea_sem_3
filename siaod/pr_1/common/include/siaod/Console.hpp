#pragma once

// Включаем UTF-8 для ввода и вывода кириллицы в консоли Windows.
// На Linux и macOS терминал обычно уже использует UTF-8.
#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

namespace siaod {
inline void configureUtf8Console() {
#ifdef _WIN32
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
#endif
}
} // namespace siaod

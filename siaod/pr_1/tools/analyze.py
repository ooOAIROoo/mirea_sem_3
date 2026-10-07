#!/usr/bin/env python3
"""Сводит CSV-замеры двух алгоритмов и сохраняет сравнительный график."""

from pathlib import Path

import matplotlib.pyplot as plt
import pandas as pd


PROJECT_DIR = Path(__file__).resolve().parents[1]
RESULTS_DIR = PROJECT_DIR / "results"
LINEAR_CSV = RESULTS_DIR / "linear_search.csv"
FIBONACCI_CSV = RESULTS_DIR / "fibonacci_search.csv"
COMPARISON_CSV = RESULTS_DIR / "comparison.csv"
PLOT_PNG = RESULTS_DIR / "search_comparison.png"


def main() -> None:
    missing = [path for path in (LINEAR_CSV, FIBONACCI_CSV) if not path.exists()]
    if missing:
        names = "\n".join(f"  - {path.relative_to(PROJECT_DIR)}" for path in missing)
        raise SystemExit(
            "Сначала выполните замеры C++-программ; отсутствуют файлы:\n" + names
        )

    linear = pd.read_csv(LINEAR_CSV)
    fibonacci = pd.read_csv(FIBONACCI_CSV)
    required = {"record_count", "mean_us", "hit_rate"}
    for label, frame in (("линейный поиск", linear), ("поиск Фибоначчи", fibonacci)):
        absent = required.difference(frame.columns)
        if absent:
            raise SystemExit(f"В CSV ({label}) отсутствуют колонки: {sorted(absent)}")

    # Оставляем только поля, необходимые для сопоставления; merge также выявляет
    # несовпадающие объемы файлов и исключает неявное сравнение разных наборов.
    comparison = linear[["record_count", "mean_us", "hit_rate"]].rename(
        columns={"mean_us": "linear_mean_us", "hit_rate": "linear_hit_rate"}
    ).merge(
        fibonacci[["record_count", "mean_us", "hit_rate"]].rename(
            columns={"mean_us": "fibonacci_mean_us", "hit_rate": "fibonacci_hit_rate"}
        ),
        on="record_count",
        how="outer",
        validate="one_to_one",
        indicator=True,
    )
    if not (comparison["_merge"] == "both").all():
        raise SystemExit("Наборы размеров в двух CSV не совпадают; сравнение остановлено.")
    comparison = comparison.drop(columns="_merge").sort_values("record_count")
    comparison["speedup"] = comparison["linear_mean_us"] / comparison["fibonacci_mean_us"]
    comparison["speedup"] = comparison["speedup"].round(3)
    comparison.to_csv(COMPARISON_CSV, index=False, float_format="%.6f")

    plt.style.use("seaborn-v0_8-whitegrid")
    figure, (time_axis, speed_axis) = plt.subplots(1, 2, figsize=(12, 5.4))
    figure.suptitle("Сравнение времени поиска по номеру автомобиля", fontsize=14)

    time_axis.plot(
        comparison["record_count"],
        comparison["linear_mean_us"],
        marker="o",
        linewidth=2,
        label="Линейный поиск в файле",
    )
    time_axis.plot(
        comparison["record_count"],
        comparison["fibonacci_mean_us"],
        marker="s",
        linewidth=2,
        label="Фибоначчи + индекс + чтение файла",
    )
    time_axis.set_xscale("log")
    time_axis.set_yscale("log")
    time_axis.set_xlabel("Число записей в файле")
    time_axis.set_ylabel("Среднее время одного поиска, мкс (лог. шкала)")
    time_axis.set_title("Время поиска")
    time_axis.legend(fontsize=8)

    speed_axis.plot(
        comparison["record_count"],
        comparison["speedup"],
        marker="o",
        color="#7a3db8",
        linewidth=2,
    )
    speed_axis.axhline(1.0, color="gray", linestyle="--", linewidth=1)
    speed_axis.set_xscale("log")
    speed_axis.set_yscale("log")
    speed_axis.set_xlabel("Число записей в файле")
    speed_axis.set_ylabel("Ускорение: линейный / Фибоначчи, раз")
    speed_axis.set_title("Выигрыш индексированного поиска")

    figure.tight_layout()
    RESULTS_DIR.mkdir(parents=True, exist_ok=True)
    figure.savefig(PLOT_PNG, dpi=180, bbox_inches="tight")
    plt.close(figure)

    print("Сводные результаты (среднее время одного поиска):")
    print(comparison.to_string(index=False, formatters={
        "linear_mean_us": "{:.3f}".format,
        "fibonacci_mean_us": "{:.3f}".format,
        "speedup": "{:.3f}".format,
    }))
    print(f"\nCSV:    {COMPARISON_CSV.relative_to(PROJECT_DIR)}")
    print(f"График: {PLOT_PNG.relative_to(PROJECT_DIR)}")


if __name__ == "__main__":
    main()

#!/usr/bin/env python3
"""
Скрипт для сравнения version_1.0.cpp и x2.cpp
ВАЖНО: x2.cpp использует одну переменную N для размера маски И максимального сдвига.
Поэтому сравнение имеет смысл только когда N == MAX_SH в version_1.0.cpp
"""

import subprocess
import re
import os
import sys
from typing import Optional

def modify_version_1_params(filename: str, N: int, MAX_SH: int, iterations: int) -> str:
    """Создает временную версию version_1.0.cpp с измененными параметрами"""
    with open(filename, 'r') as f:
        content = f.read()

    content = re.sub(r'const int N = \d+;', f'const int N = {N};', content)
    content = re.sub(r'const int MAX_SH = \d+;', f'const int MAX_SH = {MAX_SH};', content)
    content = re.sub(r'const int L = \d+[eE]?\d*;', f'const int L = {iterations};', content)

    temp_filename = filename.replace('.cpp', '_temp.cpp')
    with open(temp_filename, 'w') as f:
        f.write(content)

    return temp_filename

def modify_x2_params(filename: str, N: int, iterations: int) -> str:
    """Создает временную версию x2.cpp с измененными параметрами"""
    with open(filename, 'r') as f:
        content = f.read()

    content = re.sub(r'const int N = \d+;', f'const int N = {N};', content)
    content = re.sub(r'const int K = \d+[eE]?\d*;', f'const int K = {iterations};', content)

    temp_filename = filename.replace('.cpp', '_temp.cpp')
    with open(temp_filename, 'w') as f:
        f.write(content)

    return temp_filename

def compile_and_run_version_1(N: int, MAX_SH: int, iterations: int) -> Optional[float]:
    """Компилирует и запускает version_1.0.cpp"""
    filename = 'version_1.0.cpp'
    temp_file = modify_version_1_params(filename, N, MAX_SH, iterations)
    executable = temp_file.replace('.cpp', '.out')

    try:
        compile_cmd = ['g++', '-O3', '-std=c++17', temp_file, '-o', executable]
        result = subprocess.run(compile_cmd, capture_output=True, text=True, timeout=30)

        if result.returncode != 0:
            print(f"Ошибка компиляции {filename}:")
            print(result.stderr)
            return None

        print(f"  Запуск {os.path.basename(filename)}...", end=' ', flush=True)
        result = subprocess.run(['./' + executable], capture_output=True, text=True, timeout=600)

        if result.returncode != 0:
            print(f"Ошибка выполнения!")
            print(result.stderr)
            return None

        output = result.stdout
        match = re.search(r'[\d.]+\s+([\d.]+)\s+\[N=', output)
        if match:
            value = float(match.group(1))
            print(f"✓ ({value:.10f})")
            return value

        print("Не удалось извлечь результат!")
        print("Вывод:", output)
        return None

    except subprocess.TimeoutExpired:
        print(f"Timeout!")
        return None
    except Exception as e:
        print(f"Ошибка: {e}")
        return None
    finally:
        for f in [temp_file, executable]:
            if os.path.exists(f):
                os.remove(f)

def compile_and_run_x2(N: int, iterations: int) -> Optional[float]:
    """Компилирует и запускает x2.cpp"""
    filename = 'randombinary/x2.cpp'
    temp_file = modify_x2_params(filename, N, iterations)
    executable = temp_file.replace('.cpp', '_temp.out')

    try:
        compile_cmd = ['g++', '-O3', '-std=c++17', temp_file, '-o', executable]
        result = subprocess.run(compile_cmd, capture_output=True, text=True, timeout=30)

        if result.returncode != 0:
            print(f"Ошибка компиляции {filename}:")
            print(result.stderr)
            return None

        print(f"  Запуск {os.path.basename(filename)}...", end=' ', flush=True)
        result = subprocess.run(['./' + executable], capture_output=True, text=True, timeout=600)

        if result.returncode != 0:
            print(f"Ошибка выполнения!")
            print(result.stderr)
            return None

        output = result.stdout
        match = re.search(r'([\d.]+)', output)
        if match:
            value = float(match.group(1))
            print(f"✓ ({value:.10f})")
            return value

        print("Не удалось извлечь результат!")
        print("Вывод:", output)
        return None

    except subprocess.TimeoutExpired:
        print(f"Timeout!")
        return None
    except Exception as e:
        print(f"Ошибка: {e}")
        return None
    finally:
        for f in [temp_file, executable]:
            if os.path.exists(f):
                os.remove(f)

def test_configuration(N: int, MAX_SH: int, iterations: int) -> bool:
    """Тестирует одну конфигурацию параметров"""
    print(f"\n{'='*70}")
    print(f"Тест: N={N}, MAX_SH={MAX_SH}, iterations={iterations}")
    if N != MAX_SH:
        print(f"⚠️  ВНИМАНИЕ: N != MAX_SH - x2.cpp может давать другие результаты")
    print(f"{'='*70}")

    result1 = compile_and_run_version_1(N, MAX_SH, iterations)
    result2 = compile_and_run_x2(MAX_SH, iterations)  # x2.cpp: N соответствует MAX_SH

    if result1 is None or result2 is None:
        print("❌ ОШИБКА: Не удалось получить результаты")
        return False

    diff = abs(result1 - result2)
    rel_diff = diff / max(abs(result1), abs(result2), 1e-10) * 100

    print(f"\nРезультаты:")
    print(f"  version_1.0.cpp (N={N}, MAX_SH={MAX_SH}): {result1:.10f}")
    print(f"  x2.cpp (N={MAX_SH}):                      {result2:.10f}")
    print(f"  Разница:                                  {diff:.10e} ({rel_diff:.6f}%)")

    tolerance = 0.01
    if rel_diff < tolerance:
        print(f"✅ УСПЕХ: Результаты совпадают (разница < {tolerance}%)")
        return True
    else:
        print(f"❌ ОШИБКА: Результаты различаются (разница >= {tolerance}%)")
        return False

def main():
    """Основная функция"""
    print("Тестирование version_1.0.cpp и randombinary/x2.cpp")
    print("="*70)
    print("ПРИМЕЧАНИЕ: x2.cpp использует N для размера маски И максимального сдвига")
    print("Поэтому корректное сравнение возможно только когда N == MAX_SH")
    print("="*70)

    script_dir = os.path.dirname(os.path.abspath(__file__))
    os.chdir(script_dir)

    # Тестовые конфигурации где N == MAX_SH
    test_configs_equal = [
        # (N, MAX_SH, iterations) - где N == MAX_SH
        (3, 3, 200),
        (4, 4, 500),
        (5, 5, 1000),
        (6, 6, 1000),
    ]

    # Тестовые конфигурации где N != MAX_SH (для демонстрации различий)
    test_configs_different = [
        # (N, MAX_SH, iterations) - где N != MAX_SH
        (3, 2, 100),
        (4, 3, 500),
        (5, 4, 1000),
    ]

    print("\n" + "="*70)
    print("ТЕСТЫ С N == MAX_SH (ожидается совпадение)")
    print("="*70)

    results_equal = []
    for N, MAX_SH, iterations in test_configs_equal:
        success = test_configuration(N, MAX_SH, iterations)
        results_equal.append((N, MAX_SH, iterations, success))

    print("\n" + "="*70)
    print("ТЕСТЫ С N != MAX_SH (могут быть различия)")
    print("="*70)

    results_different = []
    for N, MAX_SH, iterations in test_configs_different:
        success = test_configuration(N, MAX_SH, iterations)
        results_different.append((N, MAX_SH, iterations, success))

    # Итоговый отчет
    print(f"\n{'='*70}")
    print("ИТОГОВЫЙ ОТЧЕТ")
    print(f"{'='*70}")

    print("\nТесты с N == MAX_SH:")
    print(f"{'N':<5} {'MAX_SH':<8} {'Iterations':<12} {'Результат':<10}")
    print(f"{'-'*50}")
    passed_equal = 0
    for N, MAX_SH, iterations, success in results_equal:
        status = "✅ PASS" if success else "❌ FAIL"
        print(f"{N:<5} {MAX_SH:<8} {iterations:<12} {status:<10}")
        if success:
            passed_equal += 1

    print(f"\nТесты с N != MAX_SH:")
    print(f"{'N':<5} {'MAX_SH':<8} {'Iterations':<12} {'Результат':<10}")
    print(f"{'-'*50}")
    passed_different = 0
    for N, MAX_SH, iterations, success in results_different:
        status = "✅ PASS" if success else "❌ FAIL"
        print(f"{N:<5} {MAX_SH:<8} {iterations:<12} {status:<10}")
        if success:
            passed_different += 1

    total_equal = len(results_equal)
    total_different = len(results_different)

    print(f"\n{'='*70}")
    print(f"Тесты с N == MAX_SH: {passed_equal}/{total_equal} пройдено")
    print(f"Тесты с N != MAX_SH: {passed_different}/{total_different} пройдено")

    if passed_equal == total_equal:
        print("\n✅ Все тесты с N == MAX_SH пройдены успешно!")
        print("Вывод: version_1.0.cpp и x2.cpp эквивалентны когда N == MAX_SH")
        sys.exit(0)
    else:
        print(f"\n❌ Провалено тестов с N == MAX_SH: {total_equal - passed_equal}")
        sys.exit(1)

if __name__ == "__main__":
    main()

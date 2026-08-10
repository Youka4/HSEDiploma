#!/usr/bin/env python3
"""
Скрипт для сравнения результатов version_1.0.cpp и x5.cpp
"""

import subprocess
import re
import os
import sys
from typing import Tuple, Optional

def modify_cpp_params(filename: str, N: int, MAX_SH: int, iterations: int) -> str:
    """Создает временную версию файла с измененными параметрами"""
    with open(filename, 'r') as f:
        content = f.read()

    # Замена параметров в файле
    if 'version_1.0.cpp' in filename:
        content = re.sub(r'const int N = \d+;', f'const int N = {N};', content)
        content = re.sub(r'const int MAX_SH = \d+;', f'const int MAX_SH = {MAX_SH};', content)
        content = re.sub(r'const int L = \d+[eE]?\d*;', f'const int L = {iterations};', content)
    else:  # x5.cpp
        content = re.sub(r'const int N = \d+,', f'const int N = {N},', content)
        content = re.sub(r'const int MAX_SH = \d+;', f'const int MAX_SH = {MAX_SH};', content)
        content = re.sub(r'const int K = \d+[eE]?\d*;', f'const int K = {iterations};', content)

    temp_filename = filename.replace('.cpp', '_temp.cpp')
    with open(temp_filename, 'w') as f:
        f.write(content)

    return temp_filename

def compile_and_run(filename: str, N: int, MAX_SH: int, iterations: int) -> Optional[float]:
    """Компилирует и запускает программу, возвращает результат"""
    temp_file = modify_cpp_params(filename, N, MAX_SH, iterations)
    executable = temp_file.replace('.cpp', '.out')

    try:
        # Компиляция
        compile_cmd = ['g++', '-O3', '-std=c++17', temp_file, '-o', executable]
        result = subprocess.run(compile_cmd, capture_output=True, text=True, timeout=30)

        if result.returncode != 0:
            print(f"Ошибка компиляции {filename}:")
            print(result.stderr)
            return None

        # Запуск
        print(f"  Запуск {os.path.basename(filename)}...", end=' ', flush=True)
        result = subprocess.run(['./' + executable], capture_output=True, text=True, timeout=300)

        if result.returncode != 0:
            print(f"Ошибка выполнения!")
            print(result.stderr)
            return None

        # Извлечение результата
        output = result.stdout

        # Для version_1.0.cpp ищем второе число в строке (ans/(L+MAX_SH))
        if 'version_1.0.cpp' in filename:
            match = re.search(r'[\d.]+\s+([\d.]+)\s+\[N=', output)
            if match:
                value = float(match.group(1))
                print(f"✓ ({value:.10f})")
                return value
        else:  # x5.cpp
            match = re.search(r'([\d.]+)\s+\[N=', output)
            if match:
                value = float(match.group(1))
                print(f"✓ ({value:.10f})")
                return value

        print("Не удалось извлечь результат!")
        print("Вывод программы:", output)
        return None

    except subprocess.TimeoutExpired:
        print(f"Timeout для {filename}")
        return None
    except Exception as e:
        print(f"Ошибка: {e}")
        return None
    finally:
        # Очистка временных файлов
        for f in [temp_file, executable]:
            if os.path.exists(f):
                os.remove(f)

def test_configuration(N: int, MAX_SH: int, iterations: int) -> bool:
    """Тестирует одну конфигурацию параметров"""
    print(f"\n{'='*70}")
    print(f"Тест: N={N}, MAX_SH={MAX_SH}, iterations={iterations}")
    print(f"{'='*70}")

    result1 = compile_and_run('version_1.0.cpp', N, MAX_SH, iterations)
    result2 = compile_and_run('x5.cpp', N, MAX_SH, iterations)

    if result1 is None or result2 is None:
        print("❌ ОШИБКА: Не удалось получить результаты")
        return False

    diff = abs(result1 - result2)
    rel_diff = diff / max(abs(result1), abs(result2), 1e-10) * 100

    print(f"\nРезультаты:")
    print(f"  version_1.0.cpp: {result1:.10f}")
    print(f"  x5.cpp:          {result2:.10f}")
    print(f"  Разница:         {diff:.10e} ({rel_diff:.6f}%)")

    # Считаем результаты равными если относительная разница < 0.01%
    tolerance = 0.01
    if rel_diff < tolerance:
        print(f"✅ УСПЕХ: Результаты совпадают (разница < {tolerance}%)")
        return True
    else:
        print(f"❌ ОШИБКА: Результаты различаются (разница >= {tolerance}%)")
        return False

def main():
    """Основная функция"""
    print("Тестирование version_1.0.cpp и x5.cpp")
    print("="*70)

    # Получаем директорию скрипта
    script_dir = os.path.dirname(os.path.abspath(__file__))
    os.chdir(script_dir)

    # Набор тестовых конфигураций
    test_configs = [
        # (N, MAX_SH, iterations)
        (3, 2, 100),      # Быстрый тест
        (3, 3, 500),      # Средний тест
        (4, 3, 1000),     # Средний тест
        (4, 4, 2000),     # Средний тест
        (5, 6, 10000),    # Оригинальные параметры
        (3, 5, 1000),     # Различные комбинации
        (4, 2, 5000),
    ]

    results = []
    for N, MAX_SH, iterations in test_configs:
        success = test_configuration(N, MAX_SH, iterations)
        results.append((N, MAX_SH, iterations, success))

    # Итоговый отчет
    print(f"\n{'='*70}")
    print("ИТОГОВЫЙ ОТЧЕТ")
    print(f"{'='*70}")
    print(f"{'N':<5} {'MAX_SH':<8} {'Iterations':<12} {'Результат':<10}")
    print(f"{'-'*70}")

    passed = 0
    for N, MAX_SH, iterations, success in results:
        status = "✅ PASS" if success else "❌ FAIL"
        print(f"{N:<5} {MAX_SH:<8} {iterations:<12} {status:<10}")
        if success:
            passed += 1

    print(f"{'-'*70}")
    print(f"Пройдено: {passed}/{len(results)}")

    if passed == len(results):
        print("\n✅ Все тесты пройдены успешно!")
        sys.exit(0)
    else:
        print(f"\n❌ Провалено тестов: {len(results) - passed}")
        sys.exit(1)

if __name__ == "__main__":
    main()

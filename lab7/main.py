import os
import string
import time
from collections import Counter
from concurrent.futures import ThreadPoolExecutor, ProcessPoolExecutor


def analyze_text(text):
    table = str.maketrans('', '', string.punctuation)
    text = text.translate(table).replace(' ', '').replace('\n', '').lower()
    return text


def process_file(file_path):
    with open(file_path, 'r') as file:
        content = file.read()
    return analyze_text(content)


def count_common_letters(all_text):
    return Counter(all_text).most_common(5)


def calculate_letter_probability(all_text, user_letter):
    letter_count = all_text.count(user_letter)
    total_letters = len(all_text)
    return letter_count / total_letters


def find_rare_letters(all_text):
    letter_counts = Counter(all_text)
    return sorted(letter_counts, key=letter_counts.get)[:3]


def main(directory_path, user_letter, max_w):
    start_time = time.time()

    files = [os.path.join(directory_path, file) for file in os.listdir(directory_path) if file.endswith('.txt')]

    with ThreadPoolExecutor(max_workers=max_w) as executor:
        results = list(executor.map(process_file, files))
    # results = []
    # for file in files:
    #     results.append(process_file(file))

    all_text = ' '.join(results)

    with ThreadPoolExecutor(max_workers=max_w) as executor:
        common_letters_future = executor.submit(count_common_letters, all_text)

        letter_probability_future = executor.submit(calculate_letter_probability, all_text, user_letter)
        rare_letters_future = executor.submit(find_rare_letters, all_text)

        common_letters = common_letters_future.result()
        letter_probability = letter_probability_future.result()
        rare_letters = rare_letters_future.result()

    # common_letters = count_common_letters(all_text)
    # letter_probability = calculate_letter_probability(all_text, user_letter)
    # rare_letters = find_rare_letters(all_text)

    print(f"5 самых распространенных букв: {common_letters}")
    print(f"Вероятность появления буквы '{user_letter}': {letter_probability:.4f}")
    print(f"3 самые редкие буквы: {rare_letters}")

    end_time = time.time()
    print(f"Время выполнения: {end_time - start_time} секунд")


if __name__ == "__main__":
    directory_path = "texts"
    user_letter = input("Введите букву для анализа вероятности: ").lower()
    max_w = int(input('Введите максимальное количество потоков: '))
    main(directory_path, user_letter, max_w)

# 8 -- 17.6
# 1 -- 27.5


# GIL - GIL — эта одиночный блокиратор самого интерпретатора Python.
# Он добавляет правило: любое выполнение байткода в Python требует блокировки интерпретатора.

# ThreadPoolExecutor - это класс, предоставляющий пул потоков для выполнения задач.
# Этот инструмент особенно полезен, когда задачи связаны с I/O-операциями, которые блокируют поток выполнения
# ProcessPoolExecutor предоставляет пул процессов для выполнения задач.
# Этот инструмент полезен, когда задачи могут выполняться независимо и имеют высокую вычислительную нагрузку:

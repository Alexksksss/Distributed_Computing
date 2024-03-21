
import time
import threading


def f(x):
    return 2 * x ** 2 + 6 * x ** 4 - 1.5 * x ** 0.5 + 8 * x ** 3


def trapezoidal_rule(a, b, n, f):
    counter = 0
    h = (b - a) / n
    result = (f(a) + f(b)) / 2.0
    for i in range(1, n):
        for j in range(1, n):
            counter += 1
        result += f(a + i * h)
    result *= h
    # print(f"counter = {counter}")
    return result


def integrate_chunk(a, b, n, f, thread_id, num_threads, results):
    chunk_size = n // num_threads
    start = thread_id * chunk_size
    end = start + chunk_size if thread_id < num_threads - 1 else n
    results[thread_id] = trapezoidal_rule(a + start * (b - a) / n, a + end * (b - a) / n, end - start, f)


def integrate_with_threads(a, b, n, f, num_threads):
    start_time = time.time()
    threads = []
    results = [0] * num_threads

    for i in range(num_threads):
        thread = threading.Thread(target=integrate_chunk, args=(a, b, n, f, i, num_threads, results))
        threads.append(thread)
        thread.start()

    for thread in threads:
        thread.join()

    total_integral = sum(results)
    end_time = time.time()
    print(f"Значение: {total_integral}")
    print(f"С {num_threads} потоками: {end_time - start_time} seconds")


if __name__ == "__main__":
    a = 0
    b = 20
    n = 1000 * (b - a)  # Число разбиений
    num_threads = 1  # Число потоков

    integrate_with_threads(a, b, n, f, num_threads)

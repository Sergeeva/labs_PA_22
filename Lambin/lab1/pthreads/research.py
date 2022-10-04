import os
import sys
import subprocess
import time
import random
import matplotlib.pyplot as plt


def create_test_data(width, height):
    random.seed()
    try:
        os.mkdir('test')
    except OSError as _:
        pass
    with open('./test/A.txt', 'w') as fileA, open('./test/B.txt', 'w') as fileB:
        for y in range(height):
            for x in range(width):
                fileA.write(f'{random.random()}')
                fileB.write(f'{random.random()}')
                if x < width - 1:
                    fileA.write(' ')
                    fileB.write(' ')
            if y < height - 1:
                fileA.write('\n')
                fileB.write('\n')


def remove_test_data():
    for file in os.listdir('test'):
        os.remove(f'./test/{file}')
    os.rmdir('test')


def run_with_different_threads(program_name):
    create_test_data(1000, 1000)
    threads = [i for i in range(1, 17)]
    seconds = []
    for thread in threads:
        start = time.time()
        with subprocess.Popen([f'{program_name} {thread} ./test/A.txt ./test/B.txt ./test/C.txt'], shell=True):
            pass
        seconds.append(time.time() - start)
    plt.plot(threads, seconds, 'ro-')
    plt.xlabel('Thread amount')
    plt.ylabel('Lead time, sec')
    plt.savefig('thread_change.png')
    remove_test_data()


def run_with_different_sizes(program_name, thread_amount):
    sizes = [i for i in range(10, 101)]
    seconds = []
    for size in sizes:
        create_test_data(10, size)
        start = time.time()
        with subprocess.Popen([f'{program_name} {thread_amount} ./test/A.txt ./test/B.txt ./test/C.txt'], shell=True):
            pass
        seconds.append(time.time() - start)
    plt.plot(sizes, seconds, 'ro-')
    plt.xlabel('Data size')
    plt.ylabel('Lead time, sec')
    plt.savefig('size_change.png')
    remove_test_data()


def main():
    if len(sys.argv) != 2:
        print("Usage:\n\tpython3 research.py <file_name>")
    # run_with_different_threads(sys.argv[1])
    run_with_different_sizes(sys.argv[1], 4)


if __name__ == '__main__':
    main()

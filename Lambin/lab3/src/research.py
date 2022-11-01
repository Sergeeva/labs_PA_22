import sys
import subprocess
import time
import matplotlib.pyplot as plt


def run(program_name_1, program_name_2):
    amount = [i for i in range(10, 201, 5)]
    seconds_1 = []
    seconds_2 = []
    for matrix_amount in amount:
        start = time.time()
        with subprocess.Popen([f'{program_name_1} {matrix_amount} /dev/null'], shell=True):
            pass
        seconds_1.append(time.time() - start)
        start = time.time()
        with subprocess.Popen([f'{program_name_2} {matrix_amount} /dev/null'], shell=True):
            pass
        seconds_2.append(time.time() - start)
    plt.plot(amount, seconds_1, 'ro-',
             amount, seconds_2, 'bo-')
    plt.xlabel('Matrix amount')
    plt.ylabel('Lead time, sec')
    plt.savefig('size_change.png')


def main():
    if len(sys.argv) != 3:
        print("Usage:\n\tpython3 research.py <file_name_1> <file_name_2>")
    run(sys.argv[1], sys.argv[2])


if __name__ == '__main__':
    main()

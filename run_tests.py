import subprocess
import random
import time
import numpy as np
import re
import matplotlib
import matplotlib.pyplot as plt

# run make test for accuarcy
def generate_lattices(num):

    for i in range(num):
        dim = str(random.randint(1,15))
        seed = str(random.randint(1,10000))
        subprocess.Popen(['./generate_lattice.sh', seed, dim])
        time.sleep(0.01)
    
    test_file = open('tests.txt', "r")
    test_cases = test_file.readlines()

    for i in range(len(test_cases)):
        test_cases[i] = test_cases[i][1:]
        test_cases[i] = test_cases[i][:-2] + '\n'
        test_cases[i] = test_cases[i].replace("][", "] [")

    test_file.close()

    new_file = open('test2.txt', 'w')
    new_file.writelines(test_cases)
    new_file.close

# generate_lattices(1000)

# time all tests
def get_time():

    no_lll = []
    runme = []
    lll_only = []
    bad_bound = []

    time_file = open("out.txt", "r")
    time_file = time_file.read()
    indices_list = find_substring_indices(time_file, "Time")

    i = 0
    while i < len(indices_list):
        runme.append(time_file[i+23:i+26])
        i+=1
        lll_only.append(time_file[i+23:i+26])
        i+=1
        no_lll.append(time_file[i+23:i+26])
        i+=1
        bad_bound.append(time_file[i+23:i+26])
        i+=1
    # for i in indices_list:
    #     print(time_file[i+23:i+26])

    dims = []
    dim_file = open("dimensions.txt", "r")
    lines = dim_file.readlines()
    for line in lines:
        dims.append(int(line))
    
    x = np.array(dims)
    y = np.array(runme)
    plt.plot(x, y)
    plt.title("Sample Plot")
# plt.xlabel("X Axis Label")
# plt.ylabel("Y Axis Label")
    # plt.show()
    plt.savefig('foo.png')

def find_substring_indices(main_string, substring):
    return [i for i in range(len(main_string)) if main_string.startswith(substring, i)]

get_time()
# get memory for all tests

# put into numpy arrays

# plot

# profit
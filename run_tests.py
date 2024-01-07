import subprocess
import os
import random
import time
import numpy as np
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
# import seaborn as sns

# run make test for accuarcy
def generate_lattices(num):

    for i in range(num):
        dim = str(random.randint(1,20))
        seed = str(random.randint(1,10000))
        subprocess.Popen(['./generate_lattice.sh', seed, dim])
        time.sleep(0.1)
    
    test_file = open('new_tests.txt', "r")
    test_cases = test_file.readlines()

    for i in range(len(test_cases)):
        test_cases[i] = test_cases[i][1:]
        test_cases[i] = test_cases[i][:-2] + '\n'
        test_cases[i] = test_cases[i].replace("][", "] [")

    test_file.close()

    new_file = open('new_test2.txt', 'w')
    new_file.writelines(test_cases)
    new_file.close

def generate_hard_lattices(num):

    for i in range(num):
        dim = str(random.randint(1,15))
        seed = str(random.randint(1, 10000))
        a = os.popen(f"latticegen -randseed {seed} r {dim} 32").read()
        a = a.replace("\n", "")
        a = a.replace("]]", "]")
        seed = str(random.randint(1, 10000))
        b = os.popen(f"latticegen -randseed {seed} r {dim} 32").read()
        b = b.replace("\n", "")
        b = b.replace("[[", "[")
        b = b[:b.index(']')+1]+"]"
        a = a+b
        os.popen(f"echo '{a}' >> hard_problems.txt")

    test_file = open('hard_problems.txt', "r")
    test_cases = test_file.readlines()

    for i in range(len(test_cases)):
        test_cases[i] = test_cases[i][1:]
        test_cases[i] = test_cases[i][:-2] + '\n'
        test_cases[i] = test_cases[i].replace("][", "] [")

    test_file.close()

    new_file = open('hard_problems2.txt', 'w')
    new_file.writelines(test_cases)
    new_file.close

generate_hard_lattices(1000)

# time all tests
def get_time():

    no_lll = []
    runme = []
    lll_only = []
    bad_bound = []

    time_file = open("out2.txt", "r")
    time_file = time_file.read()
    indices_list = find_substring_indices(time_file, "Time")

    i = 0
    while i < len(indices_list):
        lll_only.append(interpret_text(time_file, indices_list[i]))
        i+=1
        no_lll.append(interpret_text(time_file, indices_list[i]))
        i+=1
        bad_bound.append(interpret_text(time_file, indices_list[i]))
        i+=1
        runme.append(interpret_text(time_file, indices_list[i]))
        i+=1
    # for i in indices_list:
    #     print(time_file[i+23:i+26])

    dims = []
    dim_file = open("dimensions2.txt", "r")
    lines = dim_file.readlines()
    for line in lines:
        dims.append(int(line))
    
    x = dims[:88]
    length = len(x)
    i = 0
    while i < length:
        if x[i] > 50:
            x.pop(i)
            no_lll.pop(i)
            lll_only.pop(i)
            runme.pop(i)
            bad_bound.pop(i)
            length-=1
            i-=1
        i+=1
    x = np.array(x)
    y = np.array(runme)
    y1 = np.array(no_lll)

    # Calculate the line of best fit
    lll_enum = np.reshape(np.dstack((x,y)), (88,2))
    print(np.shape(lll_enum))
    enum_only = np.reshape(np.dstack((x,y1)), (88,2))
    print(np.shape(enum_only))

    runme_frame = pd.DataFrame(lll_enum, columns = ["dimensions", "time (ms)"])
    enum_frame = pd.DataFrame(enum_only, columns = ["dimensions", "time (ms)"])

    plt.yscale('log')
    # runme_frame["time (ms)"] = runme_frame["time (ms)"].rolling( 5).mean()
    sns.scatterplot(x="dimensions", y="time (ms)", data=runme_frame, label = "Enum + LLL")
    sns.scatterplot(x="dimensions", y="time (ms)", data=enum_frame, label = "Enum Only")
    leg = plt.legend(loc='upper center')
    plt.savefig('foo.png')

def find_substring_indices(main_string, substring):
    return [i for i in range(len(main_string)) if main_string.startswith(substring, i)]

def interpret_text(string, index):
    coefficient = 1
    if string[index+27] != "m":
        coefficient = 1000
    return float(string[index+19:index+26].strip())*coefficient

# get_time()
# get memory for all tests

# put into numpy arrays

# plot

# profit

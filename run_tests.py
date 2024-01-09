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
        dim = str(random.randint(1,40))
        seed = str(random.randint(1,10000))
        subprocess.Popen(['./generate_lattice.sh', seed, dim])
        time.sleep(0.1)
    
    test_file = open('uniform_test_cases.txt', "r")
    test_cases = test_file.readlines()

    for i in range(len(test_cases)):
        test_cases[i] = test_cases[i][1:]
        test_cases[i] = test_cases[i][:-2] + '\n'
        test_cases[i] = test_cases[i].replace("][", "] [")

    test_file.close()

    new_file = open('uniform_test_cases2.txt', 'w')
    new_file.writelines(test_cases)
    new_file.close

def generate_hard_lattices(num):

    for i in range(num):
        dim = str(random.randint(1,10))
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

# generate_hard_lattices(1000)
# generate_lattices(1000)

# time all tests
def get_time():

    no_lll = []
    runme = []

    time_file = open("uniform_time_results.txt", "r")
    time_file = time_file.read()
    indices_list = find_substring_indices(time_file, "Time")

    i = 0
    while i < len(indices_list):
        no_lll.append(interpret_text(time_file, indices_list[i]))
        i+=1
        runme.append(interpret_text(time_file, indices_list[i]))
        i+=1
    # for i in indices_list:
    #     print(time_file[i+23:i+26])

    dims = []
    dim_file = open("dimensions_uniform_problems.txt", "r")
    lines = dim_file.readlines()
    for line in lines:
        dims.append(int(line))
    
    x = dims[:1000]
    length = len(x)
    x = np.array(x)
    y = np.array(runme)
    y1 = np.array(no_lll)

    # Calculate the line of best fit
    lll_enum = np.reshape(np.dstack((x,y)), (length,2))
    enum_only = np.reshape(np.dstack((x,y1)), (length,2))

    runme_frame = pd.DataFrame(lll_enum, columns = ["dimensions", "time (ms)"])
    enum_frame = pd.DataFrame(enum_only, columns = ["dimensions", "time (ms)"])

    plt.yscale('log')
    # runme_frame["time (ms)"] = runme_frame["time (ms)"].rolling( 5).mean()
    sns.lineplot(x="dimensions", y="time (ms)", data=runme_frame, label = "Enum + LLL")
    sns.lineplot(x="dimensions", y="time (ms)", data=enum_frame, label = "Enum Only")
    leg = plt.legend(loc='upper center')
    plt.savefig('foo3.png')

def find_substring_indices(main_string, substring):
    return [i for i in range(len(main_string)) if main_string.startswith(substring, i)]

def interpret_text(string, index):
    coefficient = 1
    if string[index+27] != "m":
        coefficient = 1000
    return float(string[index+19:index+26].strip())*coefficient

def get_memory(file_memory, file_dim):

    lines = open(file_memory).readlines()
    dimensions = [int(line) for line in open(file_dim).readlines()]
    mem_runme = []
    mem_nolll = []
    i = 0

    for line in lines:

        line = line.replace("heap total: ", "")

        if i % 2 == 0:
            mem_runme.append(float(line))
        else:
            mem_nolll.append(float(line))
        
        i+=1
    
    length = len(dimensions)
    dimensions, mem_runme, mem_nolll = np.array(dimensions), np.array(mem_runme), np.array(mem_nolll)

    # Calculate the line of best fit
    lll_enum = np.reshape(np.dstack((dimensions, mem_runme)), (length,2))
    enum_only = np.reshape(np.dstack((dimensions,mem_nolll)), (length,2))

    runme_frame = pd.DataFrame(lll_enum, columns = ["dimensions", "memory use (bytes)"])
    enum_frame = pd.DataFrame(enum_only, columns = ["dimensions", "memory use (bytes)"])
    # runme_frame["time (ms)"] = runme_frame["time (ms)"].rolling( 5).mean()
    sns.lineplot(x="dimensions", y="memory use (bytes)", data=runme_frame, label = "Enum + LLL")
    sns.lineplot(x="dimensions", y="memory use (bytes)", data=enum_frame, label = "Enum Only")
    leg = plt.legend(loc='upper center')
    plt.savefig('foo3.png')

generate_hard_lattices(100)
generate_lattices(100)

# get_time()
# get memory for all tests

# get_memory("out.txt", "dimensions_uniform_problems.txt")

# put into numpy arrays

# plot

# profit

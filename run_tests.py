import subprocess
import random
import time
import numpy as np
import re
import matplotlib
import matplotlib.pyplot as plt
# import seaborn as sns

# run make test for accuarcy
def generate_lattices(num):

    for i in range(num):
        dim = str(random.randint(1,50))
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

# generate_lattices(200)

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
    y2 = np.array(lll_only)
    y3 = np.array(bad_bound)
    # Calculate the line of best fit
    slope, intercept = np.polyfit(x, y, 1)
    line_of_best_fit = slope * x + intercept

    # Plotting the line of best fit
    # plt.plot(x, line_of_best_fit, label='runme')# plt.xlabel("X Axis Label")
    plt.scatter(x,y, label = "runme")
    # Calculate the line of best fit
    slope, intercept = np.polyfit(x, y1, 1)
    line_of_best_fit = slope * x + intercept

    # Plotting the line of best fit
    # plt.plot(x, line_of_best_fit, label='No LLL')# plt.xlabel("X Axis Label")
    plt.scatter(x,y1, label = "No LLL")
    # Calculate the line of best fit
    slope, intercept = np.polyfit(x, y2, 1)
    line_of_best_fit = slope * x + intercept

    # Plotting the line of best fit
    # plt.plot(x, line_of_best_fit, label="LLL Only")# plt.xlabel("X Axis Label")
    # plt.scatter(x,y2, label = "LLL Only")
    # Calculate the line of best fit
    slope, intercept = np.polyfit(x, y3, 1)
    line_of_best_fit = slope * x + intercept

    # Plotting the line of best fit
    # plt.plot(x, line_of_best_fit, label='Bad Bound')# plt.xlabel("X Axis Label")
# plt.ylabel("Y Axis Label")
    # plt.show()
    leg = plt.legend(loc='upper center')
    plt.savefig('foo.png')

def find_substring_indices(main_string, substring):
    return [i for i in range(len(main_string)) if main_string.startswith(substring, i)]

def interpret_text(string, index):
    coefficient = 1
    if string[index+27] != "m":
        coefficient = 1000
    return float(string[index+19:index+26].strip())*coefficient

get_time()
# get memory for all tests

# put into numpy arrays

# plot

# profit

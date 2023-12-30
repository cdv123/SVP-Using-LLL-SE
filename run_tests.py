import subprocess
import random
import time

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

generate_lattices(1000)

# time all tests

# get memory for all tests

# put into numpy arrays

# plot

# profit
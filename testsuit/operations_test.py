import math
import random
import sys, string, os, subprocess
import time

CPP_FILE = sys.argv[1]
INPUT_FILE = sys.argv[2]
OUTPUT_FILE = sys.argv[3]
EXEC_FILE = './a.out'

subprocess.run(['g++', CPP_FILE])

file_in = open(INPUT_FILE, 'w')
file_in.close()
file_out = open(OUTPUT_FILE, 'w')
file_out.close()

def test(attempt):
    bi1_lst = []
    bi2_lst = []
    bi_lst = []
    base = 2**31 - 1

    bi1_len = random.randint(100, 100)             #change length of first number
    bi2_len = random.randint(100, 100)              #change length of second number
    for i in range(0, bi1_len):
        bi1_lst.append(random.randint(0, base - 1))
    for i in range(0, bi2_len):
        bi2_lst.append(random.randint(0, base - 1))
     
    bis_str = ''

    for num in bi1_lst:
        bis_str += str(num) + ' '
    bis_str = bis_str[:len(bis_str) - 1]
    bis_str += '\n'
    for num in bi2_lst:
        bis_str += str(num) + ' '

    bis_str = bis_str[:len(bis_str) - 1]

    with open(INPUT_FILE, 'w') as wr:
        wr.write('')
        wr.write(bis_str)

    start_time = time.time()
    subprocess.run([EXEC_FILE, INPUT_FILE, OUTPUT_FILE])
    finish_time = time.time()

    bis_strs = []
    with open(OUTPUT_FILE, 'r') as rd:
        bis_strs = rd.readlines()


    bi1 = 0
    bi2 = 0
    for i in range(len(bi1_lst)):
            bi1 += int(bi1_lst[-i - 1])*base**i
    for i in range(len(bi2_lst)):
            bi2 += int(bi2_lst[-i - 1])*base**i
    bis = []
    
    for bis_str in bis_strs:
        bis_lst = bis_str.split(' ')
        bi = 0
        for i in range(len(bis_lst)):
            bi += int(bis_lst[-i - 1])*base**i
        bis.append(bi)
    print('==================================================')
    bi1_exp = -1
    bi2_exp = -1
    _bi1 = bi1
    _bi2 = bi2
    while (_bi1 != 0):
        bi1_exp += 1
        _bi1 = _bi1 // 2
    while (_bi2 != 0):
        bi2_exp += 1
        _bi2 = _bi2 // 2
    print('bi1 exp > 2^', bi1_exp)
    print('bi2 exp > 2^', bi2_exp)
    print('time spent: ', finish_time - start_time)
    print('==================================================')
    attempt = ''
    if bis[0] == bi1 + bi2:
        print(str(attempt) + ' sum ' + ': OK')
    else:
        print(str(attempt) + ' sum ' + ': MISSED')
    if bis[1] == abs(bi1 - bi2):
        print(str(attempt) + ' difference ' + ': OK')
    else:
        print(str(attempt) + ' difference ' + ': MISSED')
    if bis[2] == bi1 * bi2:
        print(str(attempt) + ' product ' + ': OK')
    else:
        print(str(attempt) + ' product ' + ': MISSED')
    if bis[3] == bi1 // bi2:
        print(str(attempt) + ' div ' + ': OK')
    else:
        print(str(attempt) + ' div ' + ': MISSED')
    if bis[4] == bi1 % bi2:
        print(str(attempt) + ' mod ' + ': OK')
    else:
        print(str(attempt) + ' mod ' + ': MISSED')
    
test(0)

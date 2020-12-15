import matplotlib.pyplot as plt
from copy import deepcopy
import numpy as np

def foo(id):
    name = 'logs/logs-' + id + '.txt'
    fin = open(name, 'r')

    input = fin.readlines()
    data = [[]]
    groups = 0
    for i, line in enumerate(input):
        if 'Beginning test' in line:
            if 'Number of comparisons' in input[i + 1]:
                idx = input[i + 1].find(': ') + 2
                data[-1].append(int(input[i + 1].strip('\n')[idx:]))
            else:
                data[-1].append(-1)
            groups += 1
            if groups % 4 == 0:
                data[-1] = data[-1][1:] + data[-1][:1]
                data.append([])

    data.pop()

    K = [5, 20, 32, 1000, 31622, 10000, 50172, 251189, 100000, 500000]
    K[5], K[4] = K[4], K[5]
    K[8], K[7] = K[7], K[8]
    data = np.array(data).T
    data[:, 4], data[:, 5] = data[:, 5], data[:, 4]
    data[:, 7], data[:, 8] = data[:, 8], data[:, 7]

    """
    table = deepcopy(data)
    for i in range(0, len(table)):
        for j, x in enumerate(table[i]):
            table[i][j] = '-' if x == -1 else str(table[i][j])
        table[i] = table[i] = ' & '.join(table[i]) + ' \\\\'
    """

    print(data.shape)

    table = [['$k$', 'PICK', 'BIN-REP-DEL', 'FIB-REP-DEL', 'FIB-DEL']]
    for i in range(10):
        row = [str(K[i])]
        for j in range(4):
            row.append(str(data[j][i]))
        table.append(row)

    table_str = []

    for row in table:
        table_str.append(' & '.join(row)+' \\\\')

    with open('tables/table-'+id+'.txt', 'w') as f:
        f.write('\n'.join(table_str))

    print('\n'.join(table_str))

    plt.xscale('log')
    plt.title('Ordered-Ordered')
    plt.xlabel('k')
    plt.ylabel('comparisons')
    for i in range(4):
        if i != 0:
            plt.plot(K, data[i])
        else:
            plt.plot(K[6:], data[i][6:])
    plt.legend(['PICK', 'BIN-REP-DEL', 'FIB-REP-DEL', 'FIB-DEL'])
    plt.savefig('graphs/graph'+id+'.png')

for i in range(12):
    foo(str(i))

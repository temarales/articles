import math
from read_write import *


def compute_dft(matrix):
    n = len(matrix)
    matrix[0][0] = round(1 / math.sqrt(2 * n - 1), 6)
    for j in range(1, n):
        matrix[0][j] = round(2 / math.sqrt(2 * n - 1), 6)
    for i in range(1, n):
        matrix[i][0] = round(1 / math.sqrt(2 * n - 1), 6)
        for j in range(1, n):
            matrix[i][j] = round(2 * math.cos(2 * math.pi * i * j / (2 * n-1)) / math.sqrt(2 * n - 1), 6)
    answer = input("Сохранить матрицу ДПФ в файл? (y/n) ")
    if answer == 'y':
        write_matrix_to_file(matrix, "DFT.txt")


def multiply(res, a, b):
    for i in range((len(a))):
        res[i] = 0
        for j in range((len(a[i]))):
            res[i] += a[i][j]*b[j]


def signal_conversion(signals, s, t):
    n = len(signals[0])
    m = len(signals)
    g = [[0] * n for i in range(m)]
    f = [[0] * n for i in range(m)]
    index = [1] * n
    for p in range(m):
        multiply(g[p], t, signals[p])

    for p in range(m):
        for k in range(n):
            index[k] = 1
            for k1 in range(n):
                if math.fabs(g[p][k]) < math.fabs(g[p][k1]):
                    index[k] += 1
        for k in range(n):
            if index[k] > 2:
                g[p][k] = 0

    for p in range(m):
        multiply(f[p], s, g[p])
    delta1 = 0
    for p in range(m):
        delta_buf = 0
        for k in range(n):
            delta_buf += (signals[p][k]-f[p][k])*(signals[p][k]-f[p][k])/(m*m*n)
        delta1 += math.sqrt(delta_buf)
    return delta1


def copy(frm, to):
    for i in range(len(frm)):
        for j in range(len(frm[i])):
            to[i][j] = frm[i][j]


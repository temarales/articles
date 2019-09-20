from read_write import *
import compute
import time
import copy
import math


eps = 0.1
n = int(input("Количество отсчетов в сигнале: "))
m = int(input("Количество сигналов в наборе: "))
signals = [[0] * n for i in range(m)]
s = [[0] * n for i in range(n)]
t = [[0] * n for i in range(n)]
s_buf = [[0] * n for i in range(n)]
t_buf = [[0] * n for i in range(n)]
alpha = [0] * n
read_data(signals)
compute.compute_dft(s)
t = s.copy()
deltaDFT = compute.signal_conversion(signals, s, t)
delta = deltaDFT
better = 1
start_time = time.time()
while better == 1:
    better = 0
    for i in range(n):
        best = 1
        # Генерация преобразования по i - ой строке
        for q in range(1, round(math.exp(n*math.log(3)))+1):
            s1 = copy.deepcopy(s)
            t1 = copy.deepcopy(t)
            a = q-1
            j_local = 0
            while a > 0:
                b = a % 3
                a = round(((a-b)/3.0))
                s1[i][j_local] += (b-1)*eps;
                j_local += 1
            # Нахождение обратной матрицы
            for j in range(n):
                alpha[j] = 0
                for k in range(n):
                    alpha[j] += (s1[i][k]-s[i][k])*t[k][j]
            buf = 1 + alpha[i]
            for j in range(n):
                alpha[j] = -alpha[j]/buf
            for k in range(n):
                for j in range(n):
                    t1[k][j] = t[k][j] + alpha[j]*t[k][i]
            # Применение преобразования к сигналам и подсчет среднего отклонения
            delta1 = compute.signal_conversion(signals, s1, t1)
            if delta1 < delta:
                best = q
                s_buf = s1.copy()
                t_buf = t1.copy()
                delta = delta1
        # Сохранение наилучшего преобразования
        if best > 1:
            better = 1
            s = s_buf.copy()
            t = t_buf.copy()
write_matrix_to_file(s, "S.txt")
write_matrix_to_file(t, "T.txt")
write_results(delta, deltaDFT)
print("--- %s seconds ---" % (time.time() - start_time))



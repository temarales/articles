def read_data(signals):
    file = input("Имя файла с данными о сигналах: ")
    f = open(file, "r", encoding='utf-8')
    i = - 1
    for signal in f:
        i += 1
        elements = signal.translate({ord('\n'): None}).split(" ")
        for j in range(len(elements)):
            signals[i][j] = int(elements[j])
    f.close()


def read_matrix_from_file(matrix, filename):
    f = open(filename, "r", encoding='utf-8')
    i = -1
    for row in f:
        i += 1
        elements = row.translate({ord('\n'): None}).split(" ")
        for j in range(len(elements)):
            matrix[i][j] = int(elements[j])
    f.close()


def write_matrix_to_file(matrix, filename):
    f = open(filename, "w", encoding='utf-8')
    for i in range(len(matrix)):
        for j in range(len(matrix[i])):
            f.write(str(matrix[i][j]) + " ")
        f.write("\n")
    f.close()


def write_results(res1, res2):
    filename = input("Имя файла для сохранения результата: ")
    f = open(filename, "a", encoding='utf-8')
    f.write("Среднее отклонение при сжатии найденной матрицей: " + str(res1) + "\n")
    f.write("Среднее отклонение при сжатии матрицей ДПФ: " + str(res2) + "\n")
    f.close()


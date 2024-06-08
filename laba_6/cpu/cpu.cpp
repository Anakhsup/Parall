#include <iostream>
#include <boost/program_options.hpp>
#include <cmath>
#include <memory>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <chrono>

namespace opt = boost::program_options;

// Вычисляем линейную интерполяцию между двумя точками (x1, y1) и (x2, y2) для значения x.
double linearInterpolation(double x, double x1, double y1, double x2, double y2) {
    return y1 + ((x - x1) * (y2 - y1) / (x2 - x1));
}

// Открываем файл для записи и сохраняет матрицу в указанный файл с заданной точностью форматирования
void saveMatrixToFile(const std::unique_ptr<double[]>& matrix, int size, const std::string& filename) {
    std::ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        std::cerr << "Can't open the file " << filename << " to writing." << std::endl;
        return;
    }

    int fieldWidth = 10; // Ширина поля вывода

    // запись матрицы в файл
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            outputFile << std::setw(fieldWidth) << std::fixed << std::setprecision(4) << matrix[i * size + j];
        }
        outputFile << std::endl;
    }

    outputFile.close();
}

// Заполняем матрицу начальными значениями с использованием линейной интерполяции на краях матрицы
void initializeMatrix(std::unique_ptr<double[]>& matrix, int size) {
    matrix[0] = 10.0;
    matrix[size - 1] = 20.0;
    matrix[(size - 1) * size + (size - 1)] = 30.0;
    matrix[(size - 1) * size] = 20.0;

    for (size_t i = 1; i < size - 1; i++) {
        matrix[0 * size + i] = linearInterpolation(i, 0.0, matrix[0], size - 1, matrix[size - 1]);
        matrix[i * size + 0] = linearInterpolation(i, 0.0, matrix[0], size - 1, matrix[(size - 1) * size]);
        matrix[i * size + (size - 1)] = linearInterpolation(i, 0.0, matrix[size - 1], size - 1, matrix[(size - 1) * size + (size - 1)]);
        matrix[(size - 1) * size + i] = linearInterpolation(i, 0.0, matrix[(size - 1) * size], size - 1, matrix[(size - 1) * size + (size - 1)]);
    }
}

int main(int argc, char const *argv[]) {
    // Создаем опции командной строки с помощью библиотеки boost::program_options. 
    // Определяем параметры: точность (accuracy), размер матрицы (size), количество итераций (iterations) и помощь (help).

    opt::options_description desc("Опции");
    desc.add_options()
        ("accuracy", opt::value<double>()->default_value(1e-6), "Точность")
        ("size", opt::value<int>()->default_value(1024), "Размер матрицы")
        ("iterations", opt::value<int>()->default_value(1000000), "Количество итераций")
        ("help", "Помощь");

    opt::variables_map vm;
    opt::store(opt::parse_command_line(argc, argv, desc), vm);
    opt::notify(vm);
    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 1;
    }

    int matrixSize = vm["size"].as<int>();
    double accuracy = vm["accuracy"].as<double>();
    int maxIterations = vm["iterations"].as<int>();

    // Создаем переменные для отслеживание ошибки и итераций.
    // Создаем уникальные указатели для текущей и новой матриц, 
    // инициализируем матрицы начальными значениями, а также задаем указатели на предыдущую и обновляемую матрицы.

    double error = 1.0;
    int iteration = 0;

    std::unique_ptr<double[]> currentMatrix(new double[matrixSize * matrixSize]);
    std::unique_ptr<double[]> newMatrix(new double[matrixSize * matrixSize]);

    initializeMatrix(currentMatrix, matrixSize);
    initializeMatrix(newMatrix, matrixSize);
    
    double* previousMatrix = newMatrix.get();
    double* updatedMatrix = currentMatrix.get();


    // Выполняются итерации, в каждой из которых вычисляем новые значения для матрицы в соответствии с алгоритмом, 
    // описанным внутри #pragma acc parallel loop. 
    // Также вычисляется ошибка и производится вывод текущего состояния.

    auto start = std::chrono::high_resolution_clock::now();
    while (iteration < maxIterations && iteration < 10000000 && error > accuracy) {
        #pragma acc parallel loop independent collapse(2) vector vector_length(16) gang num_gangs(40)
        for (size_t i = 1; i < matrixSize - 1; i++) {
            for (size_t j = 1; j < matrixSize - 1; j++) {
                updatedMatrix[i * matrixSize + j] = 0.25 * (previousMatrix[i * matrixSize + j + 1] + previousMatrix[i * matrixSize + j - 1] + previousMatrix[(i - 1) * matrixSize + j] + previousMatrix[(i + 1) * matrixSize + j]);
            }
        }

        if ((iteration + 1) % 10000 == 0) {
            error = 0.0;
            #pragma acc parallel loop independent collapse(2) vector vector_length(16) gang num_gangs(40) reduction(max:error)
            for (size_t i = 1; i < matrixSize - 1; i++) {
                for (size_t j = 1; j < matrixSize - 1; j++) {
                    // вычисляем максимальное значение ошибки между текущим значением error 
                    // и абсолютной разницей между соответствующими элементами матриц updatedMatrix и previousMatrix
                    error = fmax(error, fabs(updatedMatrix[i * matrixSize + j] - previousMatrix[i * matrixSize + j]));
                }
            }
            std::cout << "Итерация: " << iteration + 1 << " Ошибка: " << error << std::endl;
        }

        // обмен указателей
        double* temp = previousMatrix;
        previousMatrix = updatedMatrix;
        updatedMatrix = temp;

        iteration++;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsedMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();


    // Завершаем программу и выводим время выполнение программы

    std::cout << "Time: " << elapsedMilliseconds << " ms, Error: " << error << ", Iterations: " << iteration << std::endl;

    if (matrixSize == 13 || matrixSize == 10) {
        for (size_t i = 0; i < matrixSize; i++) {
            for (size_t j = 0; j < matrixSize; j++) {
                std::cout << currentMatrix[i * matrixSize + j] << ' ';
            }
            std::cout << std::endl;
        }
    }

    saveMatrixToFile(currentMatrix, matrixSize, "matrix.txt");
    currentMatrix = nullptr;
    newMatrix = nullptr;

    return 0;
}
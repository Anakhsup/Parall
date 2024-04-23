#include <iostream>
#include <vector>
#include <thread>
#include <omp.h>
#include <chrono>

using namespace std;
int potoks;

void initialize(const int N, unique_ptr<double[]>& matrix, unique_ptr<double[]>& vector, unique_ptr<double[]>& result) {
    #pragma omp parallel for schedule(guided, int(N / (potoks * 2)))
    for (int i = 0; i < N; i++) 
    {
        vector[i] = 0.0;
        result[i] = 0.0;
        for (int j = 0; j < N; j++) 
        {
            matrix[i * N + j] = (i == j) ? 2.0 : 1.0;
        }
    }
}

void matrix_vector_mul(const int N, unique_ptr<double[]>& matrix, unique_ptr<double[]>& vector, unique_ptr<double[]>& result) 
{
  #pragma omp parallel for schedule(guided, int(N / (potoks * 2)))
  for (int i = 0; i < N; ++i) 
  {
    result[i] = 0.0;
    for (int j = 0; j < N; ++j) 
    {
      result[i] += matrix[i * N + j] * vector[j];
    }
  }
}

void print_time_of_program(double time, int potoks, int N) 
{
  cout << "Size of matrix: " << N << endl << "Potoks: " << potoks << endl << "Time of working: " << time << " seconds" << endl;
}

int main(int argc, char const *argv[]) 
{
    if (argc < 3) {
        cout << " N= - size of matrix и potoks= - numbers of potoks" << endl;
        return 0;
    }

    int N = stoi(argv[1]);
    potoks = stoi(argv[2]);

    omp_set_num_threads(potoks);

    unique_ptr<double[]> matrix(new double[N * N]);
    unique_ptr<double[]> vector(new double[N]);
    unique_ptr<double[]> result(new double[N]);

    initialize(N, matrix, vector, result);
  
    double start_of_programm = omp_get_wtime();
    matrix_vector_mul(N, matrix, vector, result);
    double end_of_programm = omp_get_wtime();

    print_time_of_program(end_of_programm - start_of_programm, potoks, N);
    return 0;
}

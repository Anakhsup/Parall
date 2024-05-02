#include<iostream>   
#include<array>
#include <vector>
#include <thread>

double cpuSecond()
{
	struct timespec ts;
	timespec_get(&ts, TIME_UTC);
	return ((double)ts.tv_sec + (double)ts.tv_nsec * 1.e-9);
}

void initialization(double* matrix, double* vect, int sizeOfMatrix, int countOfThreads, int lb , int ub) {

	for (int i = lb; i < ub; i++) {
		for (int j = 0; j < sizeOfMatrix; j++) {
			matrix[i * sizeOfMatrix + j] = i + j;
		}
	}

	for (int j = lb; j < ub; j++) {
		vect[j] = j;
	}
}

void matrixVectorProduct(double* matrix, double* vect, double* result, int sizeOfMatrix, int countOfThreads, int lb, int ub)
{
	for (int i = lb; i < ub; i++) {
		for (int j = 0; j < sizeOfMatrix; j++)
			result[i] += matrix[i * sizeOfMatrix + j] * vect[j];
	}
}

double runSerial(int sizeOfMatrix)
{
	double time = cpuSecond();

	double* matrix = new double[sizeOfMatrix * sizeOfMatrix];
	double* vect = new double[sizeOfMatrix];
	double* result = new double[sizeOfMatrix]{};

	initialization(matrix, vect, sizeOfMatrix, 1, 0, sizeOfMatrix);
	matrixVectorProduct(matrix, vect, result, sizeOfMatrix, 1, 0, sizeOfMatrix);

	time = cpuSecond() - time;

	std::cout << "Size of matrix " << sizeOfMatrix << "*" << sizeOfMatrix << " countOfThreads = " << 1 << " Time = " << time << " S = " << 1 << std::endl;
	return time;
}

void runParallel(double* matrix, double* vect, double* result, int sizeOfMatrix, int countOfThreads, double serialTime) {
	std::vector<std::jthread> threads;

	double time = cpuSecond();

	for (int threadid = 0; threadid < countOfThreads; threadid++) {

		int itemsPerThread = sizeOfMatrix / countOfThreads;
		int lb = threadid * itemsPerThread;
		int ub = (threadid == countOfThreads - 1) ? (sizeOfMatrix - 1) : (lb + itemsPerThread - 1);
		threads.emplace_back(initialization, matrix, vect, sizeOfMatrix, countOfThreads, lb, ub);
	}
	threads.clear();

	for (int threadid = 0; threadid < countOfThreads; threadid++) {

		int itemsPerThread = sizeOfMatrix / countOfThreads;
		int lb = threadid * itemsPerThread;
		int ub = (threadid == countOfThreads - 1) ? (sizeOfMatrix - 1) : (lb + itemsPerThread - 1);
		threads.emplace_back(matrixVectorProduct, matrix, vect, result, sizeOfMatrix, countOfThreads, lb, ub);		
	}
	threads.clear();

	time = cpuSecond() - time;
	std::cout << "Size of matrix " << sizeOfMatrix << "*" << sizeOfMatrix << " countOfThreads = " << countOfThreads << " Time = " << time << " S = " << serialTime / time << std::endl;
}

int main(int argc, char** argv)
{
	std::array<int, 2> matrixSizes{20000, 40000};
	std::array<int, 7> numberOfThreads{2, 4, 7, 8, 16, 20, 40};

	double timeSerialTwo = runSerial(matrixSizes[0]);
	double timeSerialFour = runSerial(matrixSizes[1]);

	for (int i = 0; i < matrixSizes.size(); i++) {

		double* matrix = new double[matrixSizes[i] * matrixSizes[i]];
		double* vect = new double[matrixSizes[i]];
		
		std::cout << "Matrix - vector product(c[m] = a[m, n] * b[n]; m = " << matrixSizes[i] << " n = " << matrixSizes[i] << std::endl;
		for (int j = 0; j < numberOfThreads.size(); j++) {
			double* result = new double[matrixSizes[i]] {};
			if (i == 0) runParallel(matrix, vect, result, matrixSizes[i], numberOfThreads[j], timeSerialTwo);
			else runParallel(matrix, vect, result, matrixSizes[i], numberOfThreads[j], timeSerialFour);
		}
	}
	return 0;
}

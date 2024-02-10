#include <iostream>
#include <cmath>

int size = 10000000;

int main() {
    double* mass;
    mass = (double*)malloc(size * sizeof(double));
    
    for (int i = 0; i < size; i++) {
        mass[i] = sin(i * 2 * M_PI / size);
    }

    double sum = 0;
    for (int i = 0; i < size; i++) {
        sum += mass[i];
    }

    std::cout << "Sum: " << sum << std::endl;

    return 0;
}

//g++ 1.cpp
//./a.out
//Sum: 4.89582e-11

//прописать make float или make double
//g++ -std=c++11 -DARRAY_TYPE=float -o program_float 1.cpp
//g++ -std=c++11 -DARRAY_TYPE=double -o program_double 1.cpp

//./program_double 
//Sum: 4.89582e-11

//./program_float 
//Sum: 4.89582e-11


//mkdir build
//cd build
//cmake -DARRAY_TYPE=float ..
//make

//./program_double 
//Sum: 6.27585e-10

//./program_float 
//Sum: 6.27585e-10


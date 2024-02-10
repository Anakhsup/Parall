Gрописать make float или make double
g++ -std=c++11 -DARRAY_TYPE=float -o program_float 1.cpp
g++ -std=c++11 -DARRAY_TYPE=double -o program_double 1.cpp

Запуск double и результат
./program_double 
Sum: 4.89582e-11

Запуск float и результат
./program_float 
Sum: 4.89582e-11

make: make double
	    make float
cmake: cmake -DBUILD_TYPE=double CMakeLists.txt
	     cmake -DBUILD_TYPE=float CMakeLists.txt
Value Double: -1.02753e-09
Value Float:  0.144355

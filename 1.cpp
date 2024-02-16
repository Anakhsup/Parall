#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <vector>

#ifdef DOUBLE 
#define tp double
#endif

#ifdef FLOAT
#define tp float
#endif


int main(int argc, char** argv) {
    std::vector<tp> arr(10000000);

#ifdef DOUBLE
    tp res = 0.0;

    for (int x = 0; x < 10000000; ++x) {
        arr[x] = sin(2 * M_PI * x / 10000000);
        res += sin(arr[x]);
    }
    std::cout << "Double output " << res << "\n";
#endif

#ifdef FLOAT
    tp res = 0.0f;

    for (int x = 0; x < 10000000; ++x) {
        arr[x] = sin(2 * M_PI * x / 10000000);
        res += sinf(arr[x]);
    }
    std::cout << "Float output " << res << "\n";
#endif

    return 0;
}

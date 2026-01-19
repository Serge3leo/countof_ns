#include <assert.h>
#include <stdcountof.h>
int main(void) {
    int a00[0][0];
    static_assert(0 == sizeof(a00) && 0 == countof(a00));
    static_assert(0 == countof(int [0][0]));
    int a70[7][0];
    static_assert(0 == sizeof(a70) && 7 == countof(a70));
    static_assert(7 == countof(int [7][0]));
    volatile unsigned n0 = 0;
    volatile unsigned n7 = 7;
    int v00[n0][n0];
    assert(0 == sizeof(v00) && 0 == countof(v00));
    assert(0 == countof(int [n0][n0]));
    int v70[n7][n0];
    assert(0 == sizeof(v70) && 7 == countof(v70));
    assert(7 == countof(int [n7][n0]));
}

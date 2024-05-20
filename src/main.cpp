#include <crypt.h>
#include <iostream>

using namespace std;

int main() {
    cout << rdtsc << endl << rdtsc << endl;
    return 0;
}

// g++ don't have a CPU cycle counter 
static inline unsigned long long rdtsc() { // Второй вариант функции
    unsigned int hi, lo;
    __asm__ volatile("rdtsc" : "=a" (lo), "=d" (hi));
    return ((unsigned long long)hi << 32) | lo;
}

unsigned long long cyclePRNG() {
    static unsigned long long key{rdtsc()};
    static unsigned long long c{};
    static unsigned long long x{};
    x=des(key, c);
    c++;
    return x;
}

unsigned long long des(unsigned long long key, unsigned long long data) {

}
#include <crypt.h>
#include <iostream>
#include <math.h>

using namespace std;

uint64_t cyclePRNG(void);
uint64_t des_encrypt(uint64_t, uint64_t);
uint64_t rearrangement(uint64_t, uint16_t*);
void print_binary(uint64_t);
uint64_t feistel_keygen(uint64_t);

static inline uint64_t rdtsc(void);

int main(int argc, char *argv[]) {
    int value{};
    if(argc > 1) {
        value = atoi(argv[1]);
    } else value = 3;
    print_binary(value);
    cout << endl;
    uint64_t fvalue = feistel_keygen(value);
    print_binary(fvalue);
    cout << endl;
    cout << fvalue;

    return 0;
}

uint16_t ip[]{57,49,41,33,25,17,9,1,59,51,43,35,27,19,11,3,
                61,53,45,37,29,21,13,5,63,55,47,39,31,23,15,7,
                56,48,40,32,24,16,8,0,58,50,42,34,26,18,10,2,
                60,52,44,36,29,28,12,4,62,54,46,38,30,22,14,6};

uint64_t rearrangement(uint64_t n, uint16_t *a) {
    uint64_t res{0};
    for(int i=0; i<64; i++) {
        res |= ((n >> a[i]) & 1) << i;
    } 
    return res;

}
// g++ don't have a CPU cycle counter 
static inline uint64_t rdtsc() { // Второй вариант функции
    uint64_t x;
    __asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
    return x;
}

uint64_t cyclePRNG() {
    static uint64_t key{rdtsc()};
    static uint64_t c{};
    static uint64_t x{};
    x=des_encrypt(key, c);
    c++;
    return x;
}

// extend r_old from 32 to 48 bit for feistel's function
uint64_t feistel_r_extend(uint32_t r_old) {
    static uint16_t pos[]{31,0,1,2,3,4,
                    3,4,5,6,7,8,
                    7,8,9,10,11,12,
                    11,12,13,14,15,16,
                    15,16,17,18,19,20,
                    19,20,21,22,23,24,
                    23,24,25,26,27,28,
                    27,28,29,30,31,0};
    uint64_t r_old_extended{};
    for(int i = 0; i < 48; i++) {
        r_old_extended|=((r_old >> pos[i]) & 1) << i;
    }
    return r_old_extended;
} 

void print_binary(uint64_t value) {
    char str[64]{};
    int i{};
    do {
        if(value%2) {
            str[63-i]='1';
        } 
        else str[63-i]='0';
        i++;
        value >>= 1;
    } while(value);
    i--;
    while(i>=0) {
        putchar(str[63-i]);
        i--;
    }
}
// it convert a 56-bit key to a 48-bit key
uint64_t feistel_keygen(uint64_t key) {
    int count{};
    // add inv parity bit
    uint64_t new_key{};
    for(uint64_t i = 0; i < 8; i++) {
        count=0;
        uint64_t temp = key & 0b1111111;
        new_key |= temp << 8*i + 1; 
        while(temp) {
            if(temp&1) {
                count++;
            }
            temp >>= 1;
        }
        
        if(!(count&1)) {
            new_key |= (uint64_t)(pow(2, i*8));
        }
        key >>= 7UL;
        
        
    }     
    return new_key;
}

// feistel's function that repeats 16 cycles
uint32_t feistel(uint32_t r_old, uint64_t key) {
    // extend r_old to 48 bit
    uint64_t r_old_extended = feistel_r_extend(r_old);
    uint64_t ki = feistel_keygen(key);
    return 0;
}

uint64_t des_encrypt(uint64_t key, uint64_t data) {
    uint64_t data_ip = rearrangement(data, ip);
    uint32_t l_old,r_old,l,r;
    l_old = data_ip >> 32;
    r_old = data_ip;
    for(int i=0; i<16; i++) {
        l=r_old;
        r=l_old^feistel(r_old, key);
    }
    return data_ip;
}

#include <stdint.h>
#include "bp_utils.h"

// This test program write and read the the lowest dword of each blocks
// It totaly writes and read 2 times of the number of dcache blocks
int main(int argc, char** argv) {
    uint64_t core_id = bp_get_hart();
    uint64_t *base_addr = (uint64_t *)(0x80100000 + (core_id << 14));

    uint64_t dcache_block_size = 128;
    uint64_t scale_factor = dcache_block_size / 64; // pointer arithmetic 


    uint64_t dcache_sets = 256;
    uint64_t dcache_assoc = 2;
    uint64_t total_block_cnt = dcache_sets*dcache_assoc*2; // multiply by 2 because we want eviction

    for (int i = 0; i < total_block_cnt; i++) {
        uint64_t *addr = base_addr + i*scale_factor;
        uint64_t data = (uint64_t) addr | 0x0fff;

        *addr = data;
    }

    for (int i = 0; i < total_block_cnt; i++) {
        uint64_t *addr = base_addr + i*scale_factor;
        uint64_t expected = (uint64_t) addr | 0x0fff;
        uint64_t actual = *addr;

        if (expected != actual) {
           bp_finish(-1);
        }
    }

    bp_finish(0);

    return 0;
}

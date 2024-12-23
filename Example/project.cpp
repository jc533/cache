#include <iostream>

using namespace std;


void parse_cache_config(const string &path_cache) {
    // TODO: Read config file and initialize cache
    // - Read: Address_bits, Block_size, Cache_sets, Associativity
    // - Calculate: Offset_bits, Indexing_bits
    // - Initialize Cache array
}


void parse_reference_list(const string &path_ref) {
    // TODO: Process memory access sequence
    // - Read addresses and validate
    // - Store both original and converted forms
}


struct cache_line {    // Represents a cache line (equivalent to a set)
    // TODO: Define data structures for cache line
    // - Valid bits for each way
    // - Tags for each way
    // - Reference bits for clock policy
    // - Clock pointer for replacement
};
vector<cache_line> Cache;  // All cache lines (sets)



ull getIdx(ull addr, ull Mask) {
    // TODO: Extract index from address using mask
}

int update_Cache(ull addr, int line, ull Mask) {
    // TODO: Handle cache access
    // - Find cache line using index
    // - Check ways in line for hit
    // - Handle hit/miss cases
// Return: 1 for hit, 0 for miss
}

int clock_replacement(cache_line &cache_set) {
    // TODO: Like a clock hand moving through ways
    // - If current way's ref_bit is 0:
    //   -> Select this way for replacement
    // - If current way's ref_bit is 1:
    //   -> Set it to 0 and move to next way
    // Remember to advance clock pointer
}


void output(const string &path_rpt, ull Mask) {
    // TODO: Generate report
    // - Write cache parameters
    // - Write access results and statistics
}

int main(int argc, char *argv[]){

    for(auto i=0;i<argc;i++){
        cout << argv[i] << endl;
    }

    return 0;
}


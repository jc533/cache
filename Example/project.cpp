#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#define ull unsigned long long
using namespace std;


int offset_bits,indexing_bits;
int Address_bits,Block_size,Cache_sets,Associativity;
string testcase;
vector<ull> reference;
vector<string> refstring;
vector<bool> results;

struct cache_line {    // Represents a cache line (equivalent to a set)
    int associativity;
    vector<bool> valid;
    vector<bool> reference;
    vector<ull> tags;
    int clock_pointer;
    cache_line(int associativity):associativity(associativity),clock_pointer(0){
        tags.resize(associativity);
        valid.resize(associativity,0);
        reference.resize(associativity);
    }
};
vector<cache_line> Cache;  // All cache lines (sets)

void parse_cache_config(const string &path_cache) {
    ifstream fin;
    fin.open(path_cache,ios::in);
    string tmp;
    // - Read: Address_bits, Block_size, Cache_sets, Associativity
    fin >> tmp >> Address_bits;
    fin >> tmp >> Block_size;
    fin >> tmp >> Cache_sets;
    fin >> tmp >> Associativity;
    // - Calculate: Offset_bits, Indexing_bits
    offset_bits = log2(Block_size);
    indexing_bits = ceil(log2(Cache_sets));
    // - Initialize Cache array
    Cache.resize(Cache_sets, cache_line(Associativity));
}


void parse_reference_list(const string &path_ref) {
    ifstream fin;
    fin.open(path_ref,ios::in);
    string tmp;
    ull addr,converted;
    fin >> tmp >> testcase;
    fin >> tmp;
    while(tmp != ".end"){
        addr = stoull(tmp,nullptr,2);
        converted = addr >> offset_bits;
        reference.push_back(converted);
        refstring.push_back(tmp);
        fin >> tmp;
    }
}

int clock_replacement(cache_line &cache_set) {
    int *i = &cache_set.clock_pointer;
    while(cache_set.reference[*i]==1){
        cache_set.reference[*i] = 0;
        *i= (*i+1)%cache_set.associativity;
    }
    return *i;
}

ull getIdx(ull addr, ull Mask) {
    // TODO: Extract index from address using mask
    return addr & Mask;
}
int update_Cache(ull addr, int line) {
    cache_line *c = &Cache[line];
    ull tag = addr>>indexing_bits;
    for(int i=0;i<c->tags.size();i++){
        if(c->valid[i] && c->tags[i] == tag){
            c->reference[i] = 1;
            return 1;
        }
    }
    int replace_way = clock_replacement(*c);
    c->tags[replace_way] = tag;
    c->reference[replace_way] = 1;
    c->valid[replace_way] = 1; 
    return 0;
// Return: 1 for hit, 0 for miss
}

void output(const string &path_rpt) {
    ofstream fout(path_rpt,ios::out);
    int miss=0;
    fout << "Address bits: "<< Address_bits << "\n";
    fout << "Block size: "<< Block_size << "\n";
    fout << "Cache sets: "<< Cache_sets << "\n";
    fout << "Associativity: "<< Associativity << "\n";
    fout << "\n\n";
    fout << "Offset bit count: " << offset_bits << "\n";
    fout << "Indexing bit count: " << indexing_bits << "\n";
    fout << "Indexing bits:";
    for(int i=indexing_bits-1;i>=0;i--){
        fout<<" "<<(i+offset_bits);
    }
    fout << "\n\n";
    fout << ".benchmark " << testcase <<"\n";
    for(int i=0;i<results.size();i++){
        fout << refstring[i] << " ";
        miss += (1-results[i]);
        fout << (results[i] ? "hit" : "miss") << "\n";
    }
    fout <<".end\n";
    fout << "Total cache miss count: "<< miss<<"\n";
}

int main(int argc, char *argv[]){
    string config,outfile,input;
    config = argv[1];
    input = argv[2];
    outfile = argv[3];
    parse_cache_config(config);
    parse_reference_list(input);
    ull mask = (1ULL << indexing_bits)-1;
    for(auto it=reference.begin();it!=reference.end();it++){
        results.push_back(update_Cache(*it,getIdx(*it,mask)));
    }
    output(outfile);
    return 0;
}


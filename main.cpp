#include <iostream>
#include <cmath>
#include <map>
#include <vector>
#include<fstream>
#include<iomanip>

using namespace std;

double N_miss = 0;
double N_hit = 0;
double N_access = 0;
double miss_rate;
double hit_rate;
double AMAT = 0;
int S, L, C, t_cache;
const int t_memory = 100;
int offset_size, index_size, tag_size;
    
vector<int> address_seq;

struct cache{
    bool valid_bit;
    vector<int> tag;
    
    cache(){
    }

    cache(int t){
        valid_bit = false;
        tag.resize(t);
    }
};

map<int, cache> my_cache;

vector <int> DecimalToBinary(long long n, int size) {
    vector <int> binary(size, 0);
    long long num = n;
    if (n < 0) n = abs(n);
    // Building the binary vector 
    int i = 0;
    while (n > 0 && i < size) {
        binary[i] = n % 2;
        n = n / 2;
        i++;
    }
    // Two's Complement
    if (num < 0)
    {
        int index = 0;
        int k = 0;
        while (index == 0)
        {
            if (binary[k] == 0)
            {
                k++;
            }
            else
                index = ++k;
        }
        for (int l = k; l < size; l++)
        {
            if (binary[l] == 0)
                binary[l] = 1;
            else
                binary[l] = 0;
        }
    }
    return binary;
}

long long BinaryToDecimal(int size, vector <int> binary, bool sign){
    bool its_negative = false;
    long long decimal = 0;
    int old_size = binary.size();
    binary.resize(size);
    if (sign && size > old_size)
    {
        for (int i = old_size; i < size; i++)
            binary[i] = binary[old_size - 1];
    }

    if (sign && binary[size - 1] == 1) // Negative number
    {
        its_negative = true;
        //Two's Complement
        int index = 0;
        int k = 0;
        while (index == 0)
        {
            if (binary[k] == 0)
            {
                k++;
            }
            else
                index = ++k;
        }
        for (int l = k; l < size; l++)
        {
            if (binary[l] == 0)
                binary[l] = 1;
            else
                binary[l] = 0;
        }

    }

    // Converting from binary to decimal
    for (int i = 0; i < binary.size(); i++)
        decimal += binary[i] * pow(2, i);
    if (its_negative)
        decimal *= -1;


    return decimal;
}

vector <int> read_part(vector <int> binary, int size, int start){
    vector <int> part(size);
    for (int i = start; i < size + start; i++)
        part[i - start] = binary[i];
    return part;
}

void init(string s, string l, string t, string file_name){
    // cout << "Hi! Welcome to our Direct mapping Cache simulator!\n";
    // cout << "Please, enter the following:\n";
    // cout <<"1. The total cache size (S). \n";
    // cin >> S;
    // cout << "2. The cache line size (L). \n";
    // cin >> L;
    // cout << "3. The number of cycles needed to access the cache (e.g., an integer between 1 and 10 clock cycles). \n";
    // cin >> t_cache;
    
    S= stoi(s);
    L = stoi(l);
    t_cache = stoi(t);

    C = S/ L;
    offset_size = log2(L);
    index_size = log2(C);
    tag_size = 32 - offset_size - index_size;
    
    ifstream myfile;
    myfile.open(file_name);

    if(!myfile.is_open()){
        cout << "Error opening the file!\n";
    }else{
        string line;
        while(getline(myfile, line)){
            if (line.empty())
                continue;
            
            address_seq.push_back(stoi(line));   
        }

    }
    myfile.close();

    //Initializing the map
    for (int i = 0; i < C; i++)
    {  
        cache value(tag_size);
        my_cache[i] = value;
        // cout << i << " " << value.tag.size() << " " << value.valid_bit << " " << endl;;
    }
}

void print(int addr){
    cout << "After accessing the address: " << addr << endl;
    cout << "The Total Number of Accesses = " << N_access << endl;
    cout << "The Hit Ratio = " << hit_rate << endl;
    cout << "The Miss Ratio = " << miss_rate << endl;
    cout << "The Average Memory Access Time (AMAT) in cycles = " << AMAT << endl;
    cout << "The Cache Now: " << endl;
    cout <<"Idx"<< setw(14) << "valid_bit" << setw(15)<< "tag" << endl;

    for (int i = 0; i < C; i++)
    {  
        cout << i << setw(11) << my_cache[i].valid_bit << setw(7);
        for (int j = my_cache[i].tag.size() - 1; j >= 0; j--)
        {
            cout << my_cache[i].tag[j];
        }
        cout << endl;
    }
    cout << endl;
}

void Direct_mapping(){
    for (int i =0; i <address_seq.size(); i++){
        vector<int>addr_bin = DecimalToBinary(address_seq[i], 32);
        vector<int> idx = read_part(addr_bin, index_size, offset_size);
        vector<int> tagg = read_part(addr_bin, tag_size, index_size + offset_size);

        long long block_num = BinaryToDecimal(idx.size(), idx, false);
        
        if(!my_cache[block_num].valid_bit){
            N_miss++;
            my_cache[block_num].valid_bit = true;
            my_cache[block_num].tag = tagg;

        }else{
            if(my_cache[block_num].tag == tagg){
                N_hit++;
            }else{
                N_miss++;
                my_cache[block_num].valid_bit = true;
                my_cache[block_num].tag = tagg;
            }
        }
        N_access++;
        miss_rate = N_miss / N_access;
        hit_rate = N_hit / N_access;
        AMAT = t_cache + miss_rate*t_memory;
        print(address_seq[i]);
    }


}

int main(int argc, char** argv) {
    if (argc < 5){
        cout << "Missing argument you need to provide S, L, cache_time, and file name!" << endl;
    }
    
    init(argv[1], argv[2], argv[3], argv[4]);
    Direct_mapping();
    return 0;
}
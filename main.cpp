#include <iostream>       // cout
#include <atomic>         // atomic_flag
#include <thread>         // thread
#include <vector>         // vector
#include <sstream>        // stringstream
#include <queue> 
#include <map>
#include <cstdlib>
#include <time.h>
#include <unistd.h>

#define RAM_SIZE 1024 // should fit 'int' 
#define CACHE_SIZE 64 // should be <= RAM_SIZE / 10

using namespace std;

typedef struct ram_block {
    int size;
    int loc;
    int next;
    int prev;
    char free;
} ram_block;

class RAM;

class Process {
    private:
        int size;
        double time;
        RAM* ram;
    public:
        Process(int s, double t) {
            this->size = s;
            this->time = t;
        }

        char* read(int where, int how_many) {
            
        }
};

class RAM {
private:
    vector<char> content;
    list<ram_block> blocks;

    void fill(int loc, char* what, int size) {
        for (int i = loc; i < loc + size; i++) {
            content[i] = what[i - loc];
        }
    }
public:
    RAM() {
        ram_block b;
        b.size = RAM_SIZE - sizeof(ram_block);
        b.loc = 0;
        b.next = -1;
        b.prev = -1;
        b.free = 1;
        blocks.push_back(b);
        char* c = (char*)&b;
        fill(0, c, sizeof(ram_block));
    }
    

};

typedef struct request {
    int terminal;
    int client;
    int amount;
    int id;
} request;


void terminal_process(int terminal_id, int count_of_clients) 


int main ()
{
    

    return 0;
}

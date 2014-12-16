#include "Process.h"
#include <queue>

#define MIN_PROCESS_SIZE 20
#define MAX_PROCESS_SIZE 60
#define MAX_PROCESS_WORK 8
#define COUNT_OF_PROCESSES 6

using std::queue;

class Scheduler {
private:
    queue<Process*> Q;
public:
    Scheduler();
    ~Scheduler();

    void create(ushort size);
    void kill();

    void generate(int n);
    void run();
};

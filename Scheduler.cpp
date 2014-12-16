#include "Scheduler.h"
#include <stdlib.h>
#include <time.h>

Scheduler::Scheduler() : Q() {}

Scheduler::~Scheduler() {
   while(!Q.empty()) kill();
}

void Scheduler::create(ushort size) {
    ushort loc = mem_alloc(size);
    byte id = Q.size();
    Process *p = new Process(id, loc, size);
    Q.push(p);
}

void Scheduler::kill() {
    if (Q.empty()) return;
    Process *p = Q.front();
    mem_free(p->loc);
    delete p;
    Q.pop();
}

void Scheduler::generate(int n) {
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        create(rand () % MIN_PROCESS_SIZE + (MAX_PROCESS_SIZE - MIN_PROCESS_SIZE));
    }
}

void Scheduler::run() {
    while(!Q.empty()) {
        Process *p = Q.front();
        p->do_something(rand() % MAX_PROCESS_WORK);
        kill();
    }
}

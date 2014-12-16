#include "Process.h"

int main() {
    mem_init();
    mem_dump();
    
    Process p(0, 30);
    p.log = true;
    p.do_something(10);
    p.write(0,255);
    
    mem_dump();

    return 0;
}


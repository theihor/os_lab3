#include "memory.h"

byte mem[MEM_SIZE];

byte cache[(CACHE_WORD_SIZE + HEADER_SIZE) * CACHE_WORD_COUNT];
int hit[CACHE_WORD_COUNT];
bool modified[CACHE_WORD_COUNT];

ushort make_header(ushort size, bool busy) {
    if (size > MEM_SIZE) {
        printf("Error in make_header: too big size.\n");
        return 0;
    }
    else {
        if (busy) return size | 0x8000;
        else return size;
    }
}

block_info block_info_of_header(ushort address, ushort header) {
    block_info info;
    info.address = address;
    info.size = header & 0x7FFF;
    info.busy = (bool)((header & 0x8000) >> 15);

    return info;
}

void mem_set_block(block_info info) {
    ushort header = make_header(info.size, info.busy);
    byte* p_header = (byte*)&header;
    mem[info.address + 1] = *p_header;
    mem[info.address] = *(p_header + 1);
}

ushort read_header(ushort address) {
    ushort header = (ushort)mem[address];
    header <<= 8;
    header |= (ushort)mem[address + 1];
    return header; 
}

block_info read_block_info(ushort address) {
    return block_info_of_header(address, read_header(address));
}

void mem_dump() {
    printf("MEM_DUMP START");
    ushort i = 0;
    for (i = 0; i < MEM_SIZE; i++) {
        if (i % 16 == 0) printf("\n");
        printf("%#02x\t", mem[i]);
    }
    printf("\nMEM_DUMP END\n");
    
    printf("CACHE\n");
    for (int i = 0; i < CACHE_WORD_COUNT; i++) {
        for (int j = 0; j < CACHE_WORD_SIZE + HEADER_SIZE; j++) {
            printf("%#02x\t", cache[i * (CACHE_WORD_SIZE + HEADER_SIZE) + j]);
        }
        printf(" : %d hits\n", hit[i]);
    }
    printf("END OF CACHE\n");
}

void fill_block(block_info block, byte value) {
    ushort i;
    for (i = block.address + HEADER_SIZE; 
         i < block.address + HEADER_SIZE + block.size;
         i++) {
         mem[i] = value;
    }
}

// best fit
ushort mem_alloc(ushort size) {
    ushort address = 0;

    // find the smallest valid block
    ushort i = 0;
    block_info info;
    info.size = MEM_SIZE; // to find min
    do {
        block_info cur_info = read_block_info(i);
        //printf("cur_size = %d\n", cur_size);
        // if block is not busy and large enough
        if (!cur_info.busy && cur_info.size >= size) { 
            if (info.size > cur_info.size) {
                info = cur_info;
            }
        }
        i += cur_info.size + HEADER_SIZE;
    } while (i < MEM_SIZE - HEADER_SIZE);

    if (info.size == MEM_SIZE) { // valid free block is not found
        printf("Error in mem_alloc: valid block not found.\n");
        return 0;
    }
    else if (info.size >= size + THRESHOLD) { 
        ushort free_size = info.size - size - HEADER_SIZE;
        
        // locate block 
        info.size = size;
        info.busy = true;
        mem_set_block(info);

        // and create a new free block
        block_info new_info;
        new_info.address = info.address + info.size + HEADER_SIZE;
        new_info.size = free_size;
        new_info.busy = false;
        mem_set_block(new_info);
    } else {
        // just locate block 
        info.size = size + (info.size - size);
        info.busy = true;
        mem_set_block(info);
    }

    return info.address;
}

void mem_clean() {
   ushort i = 0;
   do {
        block_info block = read_block_info(i);
        if (block.busy) i += block.size + HEADER_SIZE;
        else 
            if (i + block.size + HEADER_SIZE < MEM_SIZE - HEADER_SIZE) { 
                block_info next = read_block_info(i + block.size + HEADER_SIZE);
                if (!next.busy) {
                    block_info new_info;
                    new_info.address = block.address;
                    new_info.size = block.size + next.size + HEADER_SIZE;
                    new_info.busy = false;
                    mem_set_block(new_info);
                    fill_block(new_info, 0);
                } else i = next.address + next.size + HEADER_SIZE;
            } else return;
    } while (i < MEM_SIZE - HEADER_SIZE);
}

void mem_free(ushort address) {
    block_info block = read_block_info(address);
    block.busy = false;
    mem_set_block(block);
    fill_block(block, 0);
    mem_clean();
}

ushort mem_realloc(ushort address, ushort new_size) {
    block_info current = read_block_info(address);
    bool located = false;
    if (address + current.size + HEADER_SIZE < MEM_SIZE - HEADER_SIZE) {
        block_info next = read_block_info(address + current.size + HEADER_SIZE);
        if (!next.busy && current.size + next.size >= new_size) {
            located = true;
            if (current.size + next.size >= new_size + THRESHOLD) {
                ushort free_size = current.size + next.size - new_size;
        
                // locate block 
                current.size = new_size;
                current.busy = true;
                mem_set_block(current);

                // and create a new free block
                block_info new_free;
                new_free.address = current.address + current.size + HEADER_SIZE;
                new_free.size = free_size;
                new_free.busy = false;
                mem_set_block(new_free);
            } else {
                // just locate block 
                current.size = new_size + (current.size + next.size - new_size) + HEADER_SIZE;
                current.busy = true;
                mem_set_block(current);
            } 
        }  
    }

    if (!located) {
        block_info new_block = read_block_info(mem_alloc(new_size));
        ushort i, current_address, new_address;
        for (i = 0; i < current.size; i++) {
            current_address = current.address + HEADER_SIZE + i;
            new_address = new_block.address + HEADER_SIZE + i;
            mem[new_address] = mem[current_address];
        }
        mem_set_block(new_block);
        mem_free(current.address);
        current = new_block;
    }

    return current.address;
}

void cache_init() {
    for (int i = 0; i < CACHE_WORD_COUNT; i++) {
        cache[i * (CACHE_WORD_SIZE + HEADER_SIZE)] = 0;
        cache[i * (CACHE_WORD_SIZE + HEADER_SIZE) + 1] = 0;
        hit[i] = 0;
        modified[i] = false;
    }
    
//    for (int i = 2; i < CACHE_SIZE; i++) {
//        cache[i] = mem[i];
//    }
//    ushort address = 2;
//    byte* p = (byte*)&address;
//    cache[1] = *p;
//    cache[0] = *(p + 1);
}

void mem_init() {
    block_info info;
    info.address = 0;
    info.size = MEM_SIZE - HEADER_SIZE;
    info.busy = false;

    mem_set_block(info);
    cache_init();
}

byte mem_read(ushort address) {
    // looking in the cache
    for (int i = 0; i < CACHE_WORD_COUNT; i++) {
        int word_index = i * (CACHE_WORD_SIZE + HEADER_SIZE);
        
        ushort cache_address = (ushort)cache[word_index];
        cache_address <<= 8;
        cache_address |= (ushort)cache[word_index + 1];
       
      //  printf("CACHE_ADDRESS BEFORE = %d\n", cache_address);

        if (cache_address == 0) {
            // cache is empty, let's fix this
            byte* p = (byte*)&address;
            cache[word_index + 1] = *p;
            cache[word_index] = *(p + 1);
            hit[i] = 1;
            for (int j = address; j < address + CACHE_WORD_SIZE && j < MEM_SIZE; j++) {
                cache[word_index + HEADER_SIZE + (j - address)] = mem[j];
            }
            return cache[word_index + HEADER_SIZE];
        }
        
        if (address >= cache_address && address < cache_address + CACHE_WORD_SIZE) {
            // HIT !
            hit[i]++;
            return cache[word_index + HEADER_SIZE + (address - cache_address)];
        }
    }
   // printf("CACHE IS FULL\n");
    // looks like we didn't hit and cache is full
    // let's clean a little bit
    int min = hit[0];
    int min_i = 0;
    for (int j = 1; j < CACHE_WORD_COUNT; j++) {
        if (hit[j] < min) {
            min = hit[j];
            min_i = j;
        }
    }

    hit[min_i] = 1;
    int word_index = min_i * (CACHE_WORD_SIZE + HEADER_SIZE);
   
    // write back!
    if (modified[min_i]) {
        for (int i = address; i < CACHE_WORD_SIZE && i < MEM_SIZE; i++) {
            mem[i] = cache[word_index + HEADER_SIZE + (i - address)];
        }
        modified[min_i] = false;
    }

    // filling up cache
    byte* p = (byte*)&address;
    cache[word_index + 1] = *p;
    cache[word_index] = *(p + 1);
    for (int j = address; j < address + CACHE_WORD_SIZE && j < MEM_SIZE; j++) {
        cache[word_index + HEADER_SIZE + (j - address)] = mem[j];
    }
    
    return cache[word_index + HEADER_SIZE];
}

void mem_write(ushort address, byte value) {
    // looking in the cache
    for (int i = 0; i < CACHE_WORD_COUNT; i++) {
        int word_index = i * (CACHE_WORD_SIZE + HEADER_SIZE);
        
        ushort cache_address = (ushort)cache[word_index];
        cache_address <<= 8;
        cache_address |= (ushort)cache[word_index + 1];

        if (address >= cache_address && address < cache_address + CACHE_WORD_SIZE) {
            // HIT !
            hit[i]++;
            modified[i] = true;
            cache[word_index + HEADER_SIZE + (address - cache_address)] = value;
            return;
        }
    }
    
    mem[address] = value;
}


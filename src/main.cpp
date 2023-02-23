#include <iostream>
#include <string>
#include <vector>

extern "C" {
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
}

size_t estimate_memory_size(size_t machine_code_size) {
    size_t b = sysconf(_SC_PAGE_SIZE);
    return (machine_code_size + b - 1) / b * b;
}

std::vector<int> datas { 1, 2, 3 };

void test() {
    for(int data: datas) {
        std::cout << data << ' ';
    }
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
    // clang-format off
    std::vector<uint8_t> machine_code = {
          0x55,                                     // push   rbp
          0x48, 0x89, 0xe5,                         // mov    rbp,rsp
          0x48, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, // movabs rax,0x0
          0x00, 0x00, 0x00,
          0xff, 0xd0,                               // call   rax
          0x5d,                                     // pop    rbp
          0xc3,                                     // ret
    };
    // clang-format on
    uint64_t address = reinterpret_cast<uint64_t>(&test);
    memcpy(&machine_code[6], &address, sizeof(address));

    size_t required_mem_size = estimate_memory_size(machine_code.size());

    uint8_t* mem = (uint8_t*)mmap(nullptr, required_mem_size, PROT_READ | PROT_WRITE | PROT_EXEC,
                                  MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if(mem == MAP_FAILED) {
        std::cerr << "分配内存失败\n";
        return 1;
    }

    memcpy(mem, &machine_code[0], machine_code.size());

    auto func = reinterpret_cast<void (*)()>(mem);
    func();

    munmap(mem, required_mem_size);
    return 0;
}

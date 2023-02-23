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

int main(int argc, char* argv[]) {
    std::string hello_name = "bob";

    // clang-format off
    std::vector<uint8_t> machine_code = {
        0x48, 0xc7, 0xc0, 0x01, 0x00, 0x00, 0x00, // mov rax, 1
        0x48, 0xc7, 0xc7, 0x01, 0x00, 0x00, 0x00, // mov rdi, 1
        0x48, 0x8d, 0x35, 0x0a, 0x00, 0x00, 0x00, // lea rsi, [rip + 0xa]
        0x48, 0xc7, 0xc2, 0x11, 0x00, 0x00, 0x00, // mov rdx, 0x11
        0x0f, 0x05, // syscall
        0xc3 // ret
    };
    // clang-format on
    uint32_t len = hello_name.length();
    memcpy(&machine_code[24], &len, 4);
    for(auto ch: hello_name) {
        machine_code.emplace_back(ch);
    }

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

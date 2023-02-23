.intel_syntax noprefix

# 调用 write 系统调用（man 2 write）
# ssize_t write(int fd, const void *buf, size_t count);
mov rax, 1 # 系统调用号
# 将函数参数放到 rdi, rsi, rdx, r10, r8, r9 寄存器中
mov rdi, 1
lea rsi, [rip + 0xa]
mov rdx, 0x11
syscall
ret
.string "Hello, Your Name\n"

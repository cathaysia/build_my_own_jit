.intel_syntax noprefix

# 调用函数
push rbp
mov rbp, rsp
# 传入参数地址
movabs rax, 0x0
call rax

# 返回
pop rbp
ret

.data

msg:
  .ascii "Hello, world!\n"
  len = . - msg

.text
  movq  $4, %rax
  movq  $1, %rbx
  movq  $msg, %rcx
  movq  $len, %rdx
  int $0x80

  movq $1, %rax
  movq $0, %rbx
  int $0x80

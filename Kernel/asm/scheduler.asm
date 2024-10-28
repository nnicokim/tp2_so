GLOBAL save_current_context 
GLOBAL load_current_context

SECTION .text
save_current_context: ; push
    ; Armo el stack frame actual
    push rbp
    mov rbp, rsp

    ; Guardo los registros de la CPU en la estructura StackFrame
;   mov [rdi+8], r15
; 	mov [rdi+8*2], r14
;	mov [rdi+8*3], r13
;   mov [rdi+8*4], r12
;	mov [rdi+8*5], r11
;	mov [rdi+8*6], r10
;	mov [rdi+8*7], r9
;	mov [rdi+8*8], r8
;	mov [rdi+8*9], rsi
;	mov [rdi+8*10], rdi
;	mov [rdi+8*11], rbp
;	mov [rdi+8*12], rdx
;	mov [rdi+8*13], rcx
;	mov [rdi+8*14], rbx
;	mov [rdi+8*15], rax

mov [rdi+8], r15
 	mov [rsp+8*2], r14
	mov [rsp+8*3], r13
    mov [rsp+8*4], r12
	mov [rsp+8*5], r11
	mov [rsp+8*6], r10
	mov [rsp+8*7], r9
	mov [rsp+8*8], r8
	mov [rsp+8*9], rsi
	mov [rsp+8*10], rdi
	mov [rsp+8*11], rbp
	mov [rsp+8*12], rdx
	mov [rsp+8*13], rcx
	mov [rsp+8*14], rbx
	mov [rsp+8*15], rax



    mov rsp, rbp
    pop rbp
    ret

load_current_context: ; pop
    ; Armo el stack frame actual
    push rbp
    mov rbp, rsp

;   mov r15, [rdi+8]
;	mov r14, [rdi+8*2]
;	mov r13, [rdi+8*3]
;   mov r12, [rdi+8*4]
;	mov r11, [rdi+8*5]
;	mov r10, [rdi+8*6]
;	mov r9, [rdi+8*7]
;	mov r8, [rdi+8*8]
;	mov rsi, [rdi+8*9]
;	mov rdi, [rdi+8*10]
;	mov rbp, [rdi+8*11]
;	mov rdx, [rdi+8*12]
;	mov rcx, [rdi+8*13]
;	mov rbx, [rdi+8*14]
;	mov rax, [rdi+8*15]

	mov r15, [rsp+8]
	mov r14, [rsp+8*2]
	mov r13, [rsp+8*3]
    mov r12, [rsp+8*4]
	mov r11, [rsp+8*5]
	mov r10, [rsp+8*6]
	mov r9, [rsp+8*7]
	mov r8, [rsp+8*8]
	mov rsi, [rsp+8*9]
	mov rdi, [rsp+8*10]
	mov rbp, [rsp+8*11]
	mov rdx, [rsp+8*12]
	mov rcx, [rsp+8*13]
	mov rbx, [rsp+8*14]
	mov rax, [rsp+8*15]

    mov rsp, rbp
    pop rbp
    ret
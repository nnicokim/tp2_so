GLOBAL usys_write
GLOBAL usys_read
GLOBAL usys_change_draw_size
GLOBAL usys_print_array_of_draw_size
GLOBAL usys_getRegisters
GLOBAL usys_wait
GLOBAL usys_write_color
GLOBAL usys_clear_screen
GLOBAL usys_change_font_size
GLOBAL usys_get_font_size
GLOBAL usys_beep
GLOBAL usys_print_square
GLOBAL usys_print_rect
GLOBAL usys_get_time
GLOBAL usys_draw_array
GLOBAL usys_flush_buffer

; SO syscalls

; Process
GLOBAL usys_createOneProcess
GLOBAL usys_createProcess
GLOBAL usys_blockProcess
GLOBAL usys_unblockProcess
GLOBAL usys_getCurrentpid
GLOBAL usys_getCurrentPpid
GLOBAL usys_killProcess
GLOBAL usys_leaveCPU
GLOBAL usys_waitPid
GLOBAL usys_myExit
GLOBAL usys_my_nice
GLOBAL usys_increase_priority
GLOBAL usys_decrease_priority
GLOBAL usys_print_processes
GLOBAL usys_print_memory
GLOBAL usys_loop_print
GLOBAL usys_mymalloc
GLOBAL usys_myfree
GLOBAL usys_memset

; Pipes
GLOBAL usys_pollPipe
GLOBAL usys_readPipe
GLOBAL usys_writePipe

; Semaphores
GLOBAL usys_semOpen
GLOBAL usys_semClose
GLOBAL usys_semWait
GLOBAL usys_semPost
GLOBAL usys_findSem
GLOBAL usys_yield

; RICK
GLOBAL usys_rick


section .text

usys_write:
    push rbp
    mov rbp, rsp 

    mov rax, 1
    int 80h

    mov rsp, rbp
    pop rbp
    ret

usys_getRegisters:
    push rbp
    mov rbp, rsp 

    mov rax, 8
    int 80h

    mov rsp, rbp
    pop rbp
    ret

usys_wait:
    push rbp
    mov rbp, rsp 

    mov rax, 9
    int 80h

    mov rsp, rbp
    pop rbp
    ret

usys_read: 
    push rbp 
    mov rbp, rsp 

    mov rax, 0
    int 80h 

    mov rsp, rbp 
    pop rbp
    ret

usys_write_color: 
    push rbp 
    mov rbp, rsp 

    mov rax, 6
    int 80h

    mov rsp, rbp 
    pop rbp 
    ret

usys_clear_screen: 
    push rbp 
    mov rbp, rsp 

    mov rax, 7
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret 

usys_change_font_size: 
    push rbp 
    mov rbp, rsp 

    mov rax, 10
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret

usys_get_font_size: 
    push rbp 
    mov rbp, rsp 

    mov rax, 11 
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret
    
usys_beep: 
    push rbp
    mov rbp, rsp 

    mov rax, 12 
    int 80h

    mov rsp, rbp 
    pop rbp
    ret

usys_print_square: 
    push rbp 
    mov rbp, rsp 

    mov rax, 13
    int 80h 

    mov rsp, rbp 
    pop rbp
    ret

usys_print_rect:
    push rbp 
    mov rbp, rsp 

    mov rax, 14
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret

usys_get_time: 
    push rbp 
    mov rbp, rsp 

    mov rax, 2
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret

usys_change_draw_size:
    push rbp 
    mov rbp, rsp 

    mov rax, 3
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret

usys_print_array_of_draw_size:
    push rbp 
    mov rbp, rsp 

    mov rax, 4
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret

usys_draw_array: 
    push rbp 
    mov rbp, rsp 

    mov rax, 5
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret

usys_flush_buffer:
    push rbp 
    mov rbp, rsp 

    mov rax, 15
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret

usys_createOneProcess:
    push rbp
    mov rbp, rsp
    mov rax, 16
    int 80h
    mov rsp, rbp
    pop rbp
    ret
    
usys_blockProcess:
    push rbp
    mov rbp, rsp
    mov rax, 17
    int 80h
    mov rsp, rbp
    pop rbp
    ret

usys_unblockProcess:
    push rbp
    mov rbp, rsp
    mov rax, 18
    int 80h
    mov rsp, rbp
    pop rbp
    ret

usys_getCurrentpid:
    push rbp
    mov rbp, rsp
    mov rax, 19
    int 80h
    mov rsp, rbp
    pop rbp
    ret

usys_getCurrentPpid:
    push rbp
    mov rbp, rsp
    mov rax, 20
    int 80h
    mov rsp, rbp
    pop rbp
    ret

usys_killProcess:
    push rbp
    mov rbp, rsp
    mov rax, 21
    int 80h
    mov rsp, rbp
    pop rbp
    ret

usys_leaveCPU:
    push rbp
    mov rbp, rsp
    mov rax, 22
    int 80h
    mov rsp, rbp
    pop rbp
    ret

usys_waitPid:
    push rbp
    mov rbp, rsp
    mov rax, 23
    int 80h
    mov rsp, rbp
    pop rbp
    ret


usys_myExit:
    push rbp
    mov rbp, rsp
    mov rax, 26
    int 80h
    mov rsp, rbp
    pop rbp
    ret
    
usys_my_nice:
    push rbp
    mov rbp, rsp
    mov rax, 28
    int 80h
    mov rsp, rbp
    pop rbp
    ret

usys_increase_priority:
    push rbp
    mov rbp, rsp
    mov rax, 29
    int 80h
    mov rsp, rbp
    pop rbp
    ret

usys_decrease_priority:
    push rbp
    mov rbp, rsp
    mov rax, 30
    int 80h
    mov rsp, rbp
    pop rbp
    ret

usys_print_processes:
    push rbp
    mov rbp, rsp
    mov rax, 31
    int 80h
    mov rsp, rbp
    pop rbp
    ret

usys_print_memory:
    push rbp
    mov rbp, rsp
    mov rax, 32
    int 80h
    mov rsp, rbp
    pop rbp
    ret

usys_loop_print:
    push rbp
    mov rbp, rsp
    mov rax, 33
    int 80h
    mov rsp, rbp
    pop rbp
    ret


usys_createProcess:
    push rbp
    mov rbp, rsp
    mov rax, 35
    int 80h
    mov rsp, rbp
    pop rbp
    ret

usys_pollPipe:
    push rbp
    mov rbp, rsp
    mov rax, 36
    int 80h
    mov rsp, rbp
    pop rbp
    ret

usys_readPipe:
    push rbp
    mov rbp, rsp
    mov rax, 37
    int 80h
    mov rsp, rbp
    pop rbp
    ret

usys_writePipe:
    push rbp
    mov rbp, rsp
    mov rax, 38
    int 80h
    mov rsp, rbp
    pop rbp
    ret

usys_rick:
    push rbp
    mov rbp, rsp
    mov rax, 39
    int 80h
    mov rsp, rbp
    pop rbp
    ret

usys_mymalloc:
    push rbp
    mov rbp, rsp
    mov rax, 40
    int 80h
    mov rsp, rbp
    pop rbp
    ret

usys_myfree:
    push rbp
    mov rbp, rsp
    mov rax, 41
    int 80h
    mov rsp, rbp
    pop rbp
    ret

usys_semOpen:
    push rbp
    mov rbp, rsp
    mov rax, 42
    int 80h
    mov rsp, rbp
    pop rbp
    ret

usys_semClose:
    push rbp
    mov rbp, rsp
    mov rax, 43
    int 80h
    mov rsp, rbp
    pop rbp
    ret

usys_semWait:
    push rbp
    mov rbp, rsp
    mov rax, 44
    int 80h
    mov rsp, rbp
    pop rbp
    ret

usys_semPost:
    push rbp
    mov rbp, rsp
    mov rax, 45
    int 80h
    mov rsp, rbp
    pop rbp
    ret

usys_findSem:
    push rbp
    mov rbp, rsp
    mov rax, 46
    int 80h
    mov rsp, rbp
    pop rbp
    ret

usys_yield:
    push rbp  
    mov rbp, rsp
    mov rax, 47
    int 80h
    mov rsp, rbp
    pop rbp
    ret

usys_memset:
    push rbp  
    mov rbp, rsp
    mov rax, 48
    int 80h
    mov rsp, rbp
    pop rbp
    ret
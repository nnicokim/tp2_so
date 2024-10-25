GLOBAL _write
GLOBAL _read
GLOBAL _change_draw_size
GLOBAL _print_array_of_draw_size
GLOBAL _getRegisters
GLOBAL _wait
GLOBAL _write_color
GLOBAL _clear_screen
GLOBAL _change_font_size
GLOBAL _get_font_size
GLOBAL _beep
GLOBAL _print_square
GLOBAL _print_rect
GLOBAL _get_time
GLOBAL _draw_array
GLOBAL _flush_buffer

GLOBAL _createProcess
GLOBAL _blockProcess
GLOBAL _unblockProcess
GLOBAL _getCurrentpid
GLOBAL _getCurrentPpid
GLOBAL _killProcess
GLOBAL _leaveCPU
GLOBAL _waitPid
GLOBAL _test_mm
GLOBAL _test_processes
GLOBAL _myExit
GLOBAL _test_prio

section .text

_write:
    push rbp
    mov rbp, rsp 

    mov rax, 1
    int 80h

    mov rsp, rbp
    pop rbp
    ret

_getRegisters:
    push rbp
    mov rbp, rsp 

    mov rax, 8
    int 80h

    mov rsp, rbp
    pop rbp
    ret

_wait:
    push rbp
    mov rbp, rsp 

    mov rax, 9
    int 80h

    mov rsp, rbp
    pop rbp
    ret

_read: 
    push rbp 
    mov rbp, rsp 

    mov rax, 0
    int 80h 

    mov rsp, rbp 
    pop rbp
    ret

_write_color: 
    push rbp 
    mov rbp, rsp 

    mov rax, 6
    int 80h

    mov rsp, rbp 
    pop rbp 
    ret

_clear_screen: 
    push rbp 
    mov rbp, rsp 

    mov rax, 7
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret 

_change_font_size: 
    push rbp 
    mov rbp, rsp 

    mov rax, 10
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret

_get_font_size: 
    push rbp 
    mov rbp, rsp 

    mov rax, 11 
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret
    
_beep: 
    push rbp
    mov rbp, rsp 

    mov rax, 12 
    int 80h

    mov rsp, rbp 
    pop rbp
    ret

_print_square: 
    push rbp 
    mov rbp, rsp 

    mov rax, 13
    int 80h 

    mov rsp, rbp 
    pop rbp
    ret

_print_rect:
    push rbp 
    mov rbp, rsp 

    mov rax, 14
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret

_get_time: 
    push rbp 
    mov rbp, rsp 

    mov rax, 2
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret

_change_draw_size:
    push rbp 
    mov rbp, rsp 

    mov rax, 3
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret

_print_array_of_draw_size:
    push rbp 
    mov rbp, rsp 

    mov rax, 4
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret

_draw_array: 
    push rbp 
    mov rbp, rsp 

    mov rax, 5
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret

_flush_buffer:
    push rbp 
    mov rbp, rsp 

    mov rax, 15
    int 80h 

    mov rsp, rbp 
    pop rbp 
    ret

_createProcess:
    push rbp
    mov rbp, rsp
    mov rax, 16
    int 80h
    mov rsp, rbp
    pop rbp
    ret
    
_blockProcess:
    push rbp
    mov rbp, rsp
    mov rax, 17
    int 80h
    mov rsp, rbp
    pop rbp
    ret

_unblockProcess:
    push rbp
    mov rbp, rsp
    mov rax, 18
    int 80h
    mov rsp, rbp
    pop rbp
    ret

_getCurrentpid:
    push rbp
    mov rbp, rsp
    mov rax, 19
    int 80h
    mov rsp, rbp
    pop rbp
    ret

_getCurrentPpid:
    push rbp
    mov rbp, rsp
    mov rax, 20
    int 80h
    mov rsp, rbp
    pop rbp
    ret

_killProcess:
    push rbp
    mov rbp, rsp
    mov rax, 21
    int 80h
    mov rsp, rbp
    pop rbp
    ret

_leaveCPU:
    push rbp
    mov rbp, rsp
    mov rax, 22
    int 80h
    mov rsp, rbp
    pop rbp
    ret

_waitPid:
    push rbp
    mov rbp, rsp
    mov rax, 23
    int 80h
    mov rsp, rbp
    pop rbp
    ret

_test_mm:
    push rbp
    mov rbp, rsp
    mov rax, 24
    int 80h
    mov rsp, rbp
    pop rbp
    ret

_test_processes:
    push rbp
    mov rbp, rsp
    mov rax, 25
    int 80h
    mov rsp, rbp
    pop rbp
    ret

_myExit:
    push rbp
    mov rbp, rsp
    mov rax, 26
    int 80h
    mov rsp, rbp
    pop rbp
    ret

_sys_test_prio:
    push rbp
    mov rbp, rsp
    mov rax, 27
    int 80h
    mov rsp, rbp
    pop rbp
    ret
    
_my_nice:
    push rbp
    mov rbp, rsp
    mov rax, 28
    int 80h
    mov rsp, rbp
    pop rbp
    ret

_increase_priority:
    push rbp
    mov rbp, rsp
    mov rax, 29
    int 80h
    mov rsp, rbp
    pop rbp
    ret

_decrease_priority:
    push rbp
    mov rbp, rsp
    mov rax, 30
    int 80h
    mov rsp, rbp
    pop rbp
    ret
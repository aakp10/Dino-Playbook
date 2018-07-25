BOOT_SECTOR		equ 0x7c0
STACK			equ 0x7e0  ;0x7c00+(512bytes)0x200 =>0x7e0(base)
STACK_PTR		equ 0x100  ;4kb stack
DISPLAY_UTIL    equ 0x0e
DISPLAY_COLOR   equ 0x02


section .text
global _start
_start:
	;set ds to phy mem loc where boot segment is loaded
	mov ax,BOOT_SECTOR
	mov ds,ax

	;set stack segment
	mov ax,STACK
	mov ss,ax

	;set stack ptr
	mov sp,STACK_PTR

	;function call print_char(a)
	push "a"
	call print_char

	;call print_string(msg)
    push msg
    call print_string

	jmp $
	msg db "Testing: print_string",0

print_char:
	;stack frame setup
	push bp
	mov bp,sp
	;fetch argument
	mov al,[bp+4]

    mov ah,DISPLAY_UTIL
    mov bh,0
    mov bl,DISPLAY_COLOR
    int 0x10

    ;leave
    mov sp,bp
    pop bp
	ret

print_string:
	;stack frame setup
	push bp
	mov bp,sp
	mov si,[bp+4]
.loop:
	lodsb
	cmp al,0
	je _done

	;call print_char(al)
	push ax
	call print_char

	jmp .loop
.done:
	;leave
	mov sp,bp
	pop bp
	ret
	times 0x200-2-($-$$) db 0
	dw 0xaa55




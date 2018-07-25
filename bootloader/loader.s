BOOT_SECTOR		equ 0x7c0
STACK			equ 0x7e0	;0x7c00+(512bytes)0x200 =>0x7e0(base)
STACK_PTR		equ 0x100	;4kb stack
DISPLAY_UTIL	equ 0x0e
DISPLAY_COLOR	equ 0x02
DISK_READ		equ 0x02


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
	;push "a"
	;call print_char

	;call print_string(msg)
	push msg
	call print_string

	call load_dummy_kernel

	mov ax,0x810
	mov ss,ax
	mov sp,STACK_PTR
	;mov ax,0x800
	;mov ds,ax
	;mov cs,ax
	jmp 0x0800:0x0000
	;jmp $
	msg db "> TEST: Booting",0
	errmsg db "> Failed to load",0
load_dummy_kernel:
	mov ah,DISK_READ 
	;al has to be calculated before the image is generated
	mov al,1 ;no of sectors to be read *hARDCODED*
	mov ch,0
	mov cl,2
	mov dh,0
	mov ax,0x800
	mov es,ax
	mov bx,0
	int 13h
	jc .error
	ret
.error:
	push errmsg
	call print_string
	ret

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
	je .done

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








.586
	.model flat, stdcall
	includelib libucrt.lib
	includelib kernel32.lib
	includelib ../Debug/StandardLibrary.lib
	ExitProcess PROTO :DWORD

	EXTRN out: proc
	EXTRN strl: proc
	EXTRN power: proc

.stack 4096
.const
	L0 DWORD 2
	L1 DWORD 5
	L2 DWORD 10
	L3 BYTE "число 1 меньше числа 2", 0
	L4 BYTE "число 1 больше или равно числу 2", 0
	L5 BYTE "hello world", 0
	L6 BYTE "belstu", 0
	L7 DWORD 13
	L8 BYTE "08121998", 0
.data
	funac DWORD ?
	funad DWORD ?
	funbe DWORD ?
	bodya DWORD ?
	bodyb DWORD ?
	bodyc DWORD ?
	bodyq BYTE ?
	bodyw BYTE ?

.code
funa PROC funaa : SDWORD, funab : SDWORD
	push funaa
	push L0
	push funaa
	push funab
	push funac
funb PROC funbq : WORD, funbw : SDWORD
	push funbq
	push funbw
main PROC
	push L1
	push L2
	push funa
	push bodya
	push bodyb
	cmp 	cmp 	push 0
	call ExitProcess
main ENDP
end main	push offset L5
	push offset L6
	push funb
	push bodyq
	push bodya
	push 0
	call ExitProcess
main ENDP
end main
.586
	.model flat, stdcall
	includelib libucrt.lib
	includelib kernel32.lib
		includelib ../Debug/Standard_Library.lib
	ExitProcess PROTO :DWORD

	outc PROTO: DWORD
	outd PROTO: DWORD
	strl PROTO: DWORD
	power PROTO: DWORD, :DWORD

.stack 4096
.const
	L0 DWORD 2
	L1 DWORD 10
	L2 DWORD 5
	L3 BYTE "Privet, Andrey", 0
	L4 BYTE "test", 0
	L5 BYTE "Число 1 меньше числа 2!", 0
	L6 BYTE "Число 1 больше числа 2.", 0
	L7 BYTE "Курсовой проект", 0
	L8 BYTE "08121998", 0
	L9 DWORD 0
.data
	funac SDWORD ?
	funad SDWORD ?
	funbe SDWORD ?
	bodya SDWORD ?
	bodyb SDWORD ?
	bodyc SDWORD ?
	bodyd SDWORD ?
	bodyq SDWORD ?
	bodye DWORD ?
	bodyk DWORD ?

.code

funa PROC funaa : SDWORD, funab : SDWORD
	push funaa
	push L0
	pop edx
	pop edx
	push L0
	push funaa
		call power
	push eax
	pop funac
	push funaa
	push funab
	push funac
	pop eax
	pop ebx
	add eax, ebx
	push eax
	pop eax
	pop ebx
	mul ebx
	push eax
	pop funad
	push funad
		jmp local0
local0:
	pop eax
	ret
funa ENDP

funb PROC funbq : SDWORD, funbw : DWORD
	push funbq
	push L0
	pop eax
	pop ebx
	mul ebx
	push eax
	push funbw
	pop edx
	push funbw
		call strl
	push eax
	pop ebx
	pop eax
	sub eax, ebx
	push eax
	pop funbe
	push funbe
		jmp local1
local1:
	pop eax
	ret
funb ENDP

main PROC
	push L1
	pop bodyd
	push L2
	pop bodya
	push bodyd
	pop bodyb
	push bodya
	push bodyb
	pop edx
	pop edx
	push bodyb
	push bodya
		call funa
	push eax
	pop bodyc
	push bodyc
		call outd
	push offset L3
	pop bodye
	push bodyq
	push bodye
	pop edx
	pop edx
	push bodye
	push bodyq
		call funb
	push eax
	push L1
	pop eax
	pop ebx
	add eax, ebx
	push eax
	pop bodyc
	push bodyc
		call outd
	push offset L4
	pop bodyk
	push bodyk
	pop edx
	push bodyk
		call strl
	push eax
	pop bodyd
	mov eax, bodya
	cmp eax, bodyb
		jl m0
		jg m1
		je m1
m0:
	push offset L5
		call outc
	jmp e0
m1:
	push offset L6
		call outc
e0:
	push offset L7
		call outc
	push offset L8
		call outc
	push 0
		jmp theend
theend:
	call ExitProcess
main ENDP
end main
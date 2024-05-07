.data
	wSystemCall DWORD 000h              ; this is a global variable used to keep the SSN of a syscall

.code
	HellsGate PROC
		mov wSystemCall, 000h
		mov wSystemCall, ecx            ; updating the 'wSystemCall' variable with input argument (ecx register's value)
		ret
	HellsGate ENDP

	HellDescent PROC
		mov r10, rcx
		mov eax, wSystemCall            ; `wSystemCall` is the SSN of the syscall to call
		syscall
		ret
	HellDescent ENDP
end
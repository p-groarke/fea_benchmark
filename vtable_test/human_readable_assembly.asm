###
## Abstract:
##
	cmp	dword ptr [rbp - 320], 1000000000
	jge	LBB9_6
	mov	rax, qword ptr [rbp - 280]		## Potato* p -> rax
	mov	rcx, qword ptr [rax]			## vtable* -> rcx
	mov	rdi, rax
	call	qword ptr [rcx]				## (*vtable)() or vtable[0]()
	mov	eax, dword ptr [rbp - 320]
	add	eax, 1
	mov	dword ptr [rbp - 320], eax
	jmp	LBB9_2

__ZN6Potato6updateEv:                   ## @_ZN6Potato6updateEv
[...]
	mov	qword ptr [rbp - 8], rdi
	mov	rdi, qword ptr [rbp - 8]
	mov	eax, dword ptr [rdi + 8]		## Difference! int is not at address 0
	add	eax, 1
	mov	dword ptr [rdi + 8], eax
	pop	rbp
	ret



###
## Not abstract:
##
	cmp	dword ptr [rbp - 380], 1000000000
	jge	LBB9_14
	mov	rdi, qword ptr [rbp - 352]		## NormalClass* n -> rdi
	call	__ZN11NormalClass6updateEv
	mov	eax, dword ptr [rbp - 380]
	add	eax, 1
	mov	dword ptr [rbp - 380], eax
	jmp	LBB9_9


__ZN11NormalClass6updateEv:             ## @_ZN11NormalClass6updateEv
[...]
	mov	qword ptr [rbp - 8], rdi		## NormalClass* n onto stack
	mov	rdi, qword ptr [rbp - 8]		## n->int& into rdi
	mov	eax, dword ptr [rdi]			## *(n->int) int eax
	add	eax, 1							## ++int
	mov	dword ptr [rdi], eax			## int into *(n->int)
	pop	rbp
	ret

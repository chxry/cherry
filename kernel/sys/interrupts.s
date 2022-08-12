global irq0
global irq1
global irq14
extern pit_handler
extern kb_handler
extern ata_handler

%macro PUSHALL 0
  push rax
  push rbx
  push rcx
  push rdx
  push rbp
  push rdi
  push rsi
  push r8
  push r9
  push r10
  push r11
  push r12
  push r13
  push r14
  push r15
%endmacro

%macro POPALL 0
  pop r15
  pop r14
  pop r13
  pop r12
  pop r11
  pop r10
  pop r9
  pop r8
  pop rsi
  pop rdi
  pop rbp
  pop rdx
  pop rcx
  pop rbx
  pop rax
%endmacro

irq0:
	PUSHALL
  cld
	call pit_handler
	POPALL
	iretq

irq1:
	PUSHALL
  cld
	call kb_handler
	POPALL
	iretq
	
irq14:
	PUSHALL
  cld
	call ata_handler
	POPALL
	iretq
	
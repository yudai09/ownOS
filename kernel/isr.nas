[bits 32]


[global int_ignore]
[global isr00]
[global isr01]
[global isr02]
[global isr03]
[global isr04]
[global isr05]
[global isr06]
[global isr07]
[global isr08]
[global isr09]
[global isr0a]
[global isr0b]
[global isr0c]
[global isr0d]
[global isr0e]
[global isr0f]

[global isr10]
[global isr11]
[global isr12]
[global isr13]
[global isr14]
[global isr15]
[global isr16]
[global isr17]
[global isr18]
[global isr19]
[global isr1a]
[global isr1b]
[global isr1c]
[global isr1d]
[global isr1e]
[global isr1f]

[global isr20]
[global isr21]
[global isr22]
[global isr23]
[global isr24]
[global isr25]
[global isr26]
[global isr27]
[global isr28]
[global isr29]
[global isr2a]
[global isr2b]
[global isr2c]
[global isr2d]
[global isr2e]
[global isr2f]

[global isr30]
[global isr31]
[global isr32]
[global isr33]
[global isr34]
[global isr35]
[global isr36]
[global isr37]
[global isr38]
[global isr39]
[global isr3a]
[global isr3b]
[global isr3c]
[global isr3d]
[global isr3e]
[global isr3f]

[global isr40]
[global isr41]
[global isr42]
[global isr43]
[global isr44]
[global isr45]
[global isr46]
[global isr47]
[global isr48]
[global isr49]
[global isr4a]
[global isr4b]
[global isr4c]
[global isr4d]
[global isr4e]
[global isr4f]

[global isr50]
[global isr51]
[global isr52]
[global isr53]
[global isr54]
[global isr55]
[global isr56]
[global isr57]
[global isr58]
[global isr59]
[global isr5a]
[global isr5b]
[global isr5c]
[global isr5d]
[global isr5e]
[global isr5f]

[global isr60]
[global isr61]
[global isr62]
[global isr63]
[global isr64]
[global isr65]
[global isr66]
[global isr67]
[global isr68]
[global isr69]
[global isr6a]
[global isr6b]
[global isr6c]
[global isr6d]
[global isr6e]
[global isr6f]

[global isr70]
[global isr71]
[global isr72]
[global isr73]
[global isr74]
[global isr75]
[global isr76]
[global isr77]
[global isr78]
[global isr79]
[global isr7a]
[global isr7b]
[global isr7c]
[global isr7d]
[global isr7e]
[global isr7f]

[global isr80]
[global isr81]
[global isr82]
[global isr83]
[global isr84]
[global isr85]
[global isr86]
[global isr87]
[global isr88]
[global isr89]
[global isr8a]
[global isr8b]
[global isr8c]
[global isr8d]
[global isr8e]
[global isr8f]

[global isr90]
[global isr91]
[global isr92]
[global isr93]
[global isr94]
[global isr95]
[global isr96]
[global isr97]
[global isr98]
[global isr99]
[global isr9a]
[global isr9b]
[global isr9c]
[global isr9d]
[global isr9e]
[global isr9f]

[global ret2user]

[extern schedule]
[extern next_sf]
[extern intr_func]
[extern generalException]
[extern pageFault]

kernelstacktop equ 0x7c00	;$B%+!<%M%k$N%9%?%C%/(B



%macro save	0
   pushad
   push	ds
   push	es		            
   push	fs		            
   push	gs	                
	
	mov	di, ss		
	mov	ds, di		
	mov	es, di
	mov	gs, di
	mov	fs, di
	
;	mov	esp,kernelstacktop
%endmacro

%macro load	0
	pop	gs
	pop	fs
	pop	es
	pop	ds
	popad	
%endmacro	

%macro irq_master 1	;irq_master IRQ
	save          
	call	[intr_func+4*(%1)]
	mov	dx,0x20
	mov	al,0x60+%1-0x20
	out	dx ,al
	call	schedule
	load
	iret
%endmacro

ret2user:
	load
	iret

%macro irq_slave 1
	save
	call	[intr_func+4*(%1)]
    call    schedule  
	load
	mov	dx,0xa0
	mov	al,0x60+%1
	out	dx,al
;	out	0xa0,0xal
	mov	dx,0x20
	mov	al,0x62
	out	dx,al
	iret
%%message:
	db	"interrupted slave",
	dw	'0'+%1
%endmacro	

%macro interrupts 1
	save
	call	[intr_func+4*(%1)]
	call	schedule
	mov	esp,[next_sf]
	load
	iret
%endmacro

%macro irq_over30 1	;0x30$B0J>e$N3d$j9~$_(B
	save
	call	[intr_func+4*(%1)]
	call	schedule
	load
	iret
%%retvalue:
			dd	0
%endmacro

isr00:
	cli
	hlt
	interrupts 0x00
isr01:
	interrupts 0x01
isr02:
	interrupts 0x02
isr03:
	interrupts 0x03
isr04:
	interrupts 0x04
isr05:
	interrupts 0x05
isr06:
	interrupts 0x06
isr07:
	interrupts 0x07
isr08:
	interrupts 0x08
isr09:
	interrupts 0x09
isr0a:
	interrupts 0x0a
isr0b:
	interrupts 0x0B
isr0c: ;一般保語例外
	pop dword[errorCode]
	mov	eax,[esp]
	mov dword[s_eip],eax
	save
	
	push dword[s_eip]
	push dword[errorCode]	;エラーコードをプッシュ
	
	call generalException
hlt
	;add	esp,12
	call	schedule
	mov	esp,[next_sf]
	load
	;add	esp,4	;エラーコードが事前につまれるので回避
	iret
isr0d: 
	interrupts 0x0d
	cli
	hlt

isr0e:	;ページフォルト
	pop dword[errorCode]
	mov	eax,[esp]
	mov dword[s_eip],eax
	save
	push dword[s_eip]
	push dword[errorCode]	;エラーコードをプッシュ
	mov  eax,cr2		;ページフォールトを起こした番地をPUSH
	push eax
	call pageFault
	;add	esp,12
	call	schedule
	mov	esp,[next_sf]
	load
	;add	esp,4	;エラーコードが事前につまれるので回避
	iret
errorCode:
	dd 0x0
s_eip:
	dd 0x0
isr0f:
	interrupts 0x0f;
isr10:
isr11:
isr12:
isr13:
isr14:
isr15:
isr16:
isr17:
isr18:
isr19:
isr1a:
isr1b:
isr1c:
isr1d:
isr1e:
isr1f:
	hlt
isr20:
	irq_master 0x20
isr21:
	irq_master 0x21
isr22:
	irq_master 0x22
isr23:
	irq_master 0x23
isr24:
	irq_master 0x24
isr25:
	irq_master 0x25
isr26:
	irq_master 0x26
isr27:
	irq_master 0x27
isr28:
	irq_slave 0x28
isr29:
	irq_slave 0x29
isr2a:
	irq_slave 0x2a
isr2b:
	irq_slave 0x2b
isr2c:
	irq_slave 0x2c
isr2d:
	irq_slave 0x2d
isr2e:
	irq_slave 0x2e
isr2f:
	irq_slave 0x2f
isr30:
	irq_over30 0x30
isr31:
	irq_over30 0x31
isr32:
	irq_over30 0x32
isr33:
	irq_over30 0x33
isr34:
	irq_over30 0x34
isr35:
	irq_over30 0x35
isr36:
	irq_over30 0x36
isr37:
	irq_over30 0x37
isr38:
	irq_over30 0x38
isr39:
	irq_over30 0x39
isr3a:
	irq_over30 0x3a
isr3b:
	irq_over30 0x3b
isr3c:
	irq_over30 0x3c
isr3d:
	irq_over30 0x3d
isr3e:
	irq_over30 0x3e
isr3f:
	irq_over30 0x3f
isr40:
	irq_over30 0x40
isr41:
	irq_over30 0x41
isr42:
	irq_over30 0x42
isr43:
	irq_over30 0x43
isr44:
	irq_over30 0x44
isr45:
	irq_over30 0x45
isr46:
	irq_over30 0x46
isr47:
	irq_over30 0x47
isr48:
	irq_over30 0x48
isr49:
	irq_over30 0x49
isr4a:
	irq_over30 0x4a
isr4b:
	irq_over30 0x4b
isr4c:
	irq_over30 0x4c
isr4d:
	irq_over30 0x4d
isr4e:
	irq_over30 0x4e
isr4f:
	irq_over30 0x4f
isr50:
	irq_over30 0x50
isr51:
	irq_over30 0x51
isr52:
	irq_over30 0x52
isr53:
	irq_over30 0x53
isr54:
	irq_over30 0x54
isr55:
	irq_over30 0x55
isr56:
	irq_over30 0x56
isr57:
	irq_over30 0x57
isr58:
	irq_over30 0x58
isr59:
	irq_over30 0x59
isr5a:
	irq_over30 0x5a
isr5b:
	irq_over30 0x5b
isr5c:
	irq_over30 0x5c
isr5d:
	irq_over30 0x5d
isr5e:
	irq_over30 0x5e
isr5f:
	irq_over30 0x5f
isr60:
	irq_over30 0x60
isr61:
	irq_over30 0x61
isr62:
	irq_over30 0x62
isr63:
	irq_over30 0x63
isr64:
	irq_over30 0x64
isr65:
	irq_over30 0x65
isr66:
	irq_over30 0x66
isr67:
	irq_over30 0x67
isr68:
	irq_over30 0x68
isr69:
	irq_over30 0x69
isr6a:
	irq_over30 0x6a
isr6b:
	irq_over30 0x6b
isr6c:
	irq_over30 0x6c
isr6d:
	irq_over30 0x6d
isr6e:
	irq_over30 0x6e
isr6f:
	irq_over30 0x6f
isr70:
	irq_over30 0x70
isr71:
	irq_over30 0x71
isr72:
	irq_over30 0x72
isr73:
	irq_over30 0x73
isr74:
	irq_over30 0x74
isr75:
	irq_over30 0x75
isr76:
	irq_over30 0x76
isr77:
	irq_over30 0x77
isr78:
	irq_over30 0x78
isr79:
	irq_over30 0x79
isr7a:
	irq_over30 0x7a
isr7b:
	irq_over30 0x7b
isr7c:
	irq_over30 0x7c
isr7d:
	irq_over30 0x7d
isr7e:
	irq_over30 0x7e
isr7f:
	irq_over30 0x7f
isr80:
	irq_over30 0x80
isr81:
	irq_over30 0x81
isr82:
	irq_over30 0x82
isr83:
	irq_over30 0x83
isr84:
	irq_over30 0x84
isr85:
	irq_over30 0x85
isr86:
	irq_over30 0x86
isr87:
	irq_over30 0x87
isr88:
	irq_over30 0x88
isr89:
	irq_over30 0x89
isr8a:
	irq_over30 0x8a
isr8b:
	irq_over30 0x8b
isr8c:
	irq_over30 0x8c
isr8d:
	irq_over30 0x8d
isr8e:
	irq_over30 0x8e
isr8f:
	irq_over30 0x8f
isr90:
	irq_over30 0x90
isr91:
	irq_over30 0x91
isr92:
	irq_over30 0x92
isr93:
	irq_over30 0x93
isr94:
	irq_over30 0x94
isr95:
	irq_over30 0x95
isr96:
	irq_over30 0x96
isr97:
	irq_over30 0x97
isr98:
	irq_over30 0x98
isr99:
	irq_over30 0x99
isr9a:
	irq_over30 0x9a
isr9b:
	irq_over30 0x9b
isr9c:
	irq_over30 0x9c
isr9d:
	irq_over30 0x9d
isr9e:
	irq_over30 0x9e
isr9f:
	irq_over30 0x9f

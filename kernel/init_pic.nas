[bits 32]

segment .text

[global init_pic]

init_pic:
	cli
	mov	al, 0x11	; PIC$B$N=i4|2=(B
	out	0x20, al	; $B%^%9%?!<(BPIC
	dw	0x00eb, 0x00eb	; jmp $+2, jmp $+2
	out	0xA0, al	; $B%9%l!<%V(BPIC
	dw	0x00eb, 0x00eb

	mov	al, 0x20	; $B%^%9%?!<(BPIC$B3d$j9~$_HV9f$N%9%?!<%HCOE@(B
	out	0x21, al
	dw	0x00eb, 0x00eb
	mov	al, 0x28	; $B%9%l!<%V(B PIC$B3d$j9~$_HV9f$N%9%?!<%HCOE@(B
	out	0xA1, al
	dw	0x00eb, 0x00eb

	mov	al, 0x04	; $B%^%9%?!<(BPIC$B$N(BIRQ2$BHV$K(B
	out	0x21, al	; $B%9%l!<%V(BPIC$B$,7R$,$C$F$$$k(B
	dw	0x00eb, 0x00eb
	mov	al, 0x02	; $B%9%l!<%V(BPIC$B$,%^%9%?!<(BPIC$B$N(B
	out	0xA1, al	; IRQ2$BHV$K7R$,$C$F$$$k(B
	dw	0x00eb, 0x00eb

	mov	al, 0x01	; 8086$B%b!<%I$r;HMQ$9$k(B
	out	0x21, al
	dw	0x00eb, 0x00eb
	out	0xA1, al
	dw	0x00eb, 0x00eb

	mov	al, 0xFF	; $B%9%l!<%V(BPIC$B$N$9$Y$F$N3d$j9~$_$r(B
	out	0xA1, al	; $BKI$$$G$*$/(B
	dw	0x00eb, 0x00eb
	mov	al, 0xFB	; $B%^%9%?!<(BPIC$B$N(BIRQ2$BHV$r=|$$$?(B
	out	0x21, al	; $B$9$Y$F$N3d$j9~$_$rKI$$$G$*$/(B
	
	ret



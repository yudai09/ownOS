[bits 32]

segment .text

[global init_pic]

init_pic:
	cli
	mov	al, 0x11	; PICの初期化
	out	0x20, al	; マスターPIC
	dw	0x00eb, 0x00eb	; jmp $+2, jmp $+2
	out	0xA0, al	; スレーブPIC
	dw	0x00eb, 0x00eb

	mov	al, 0x20	; マスターPIC割り込み番号のスタート地点
	out	0x21, al
	dw	0x00eb, 0x00eb
	mov	al, 0x28	; スレーブ PIC割り込み番号のスタート地点
	out	0xA1, al
	dw	0x00eb, 0x00eb

	mov	al, 0x04	; マスターPICのIRQ2番に
	out	0x21, al	; スレーブPICが繋がっている
	dw	0x00eb, 0x00eb
	mov	al, 0x02	; スレーブPICがマスターPICの
	out	0xA1, al	; IRQ2番に繋がっている
	dw	0x00eb, 0x00eb

	mov	al, 0x01	; 8086モードを使用する
	out	0x21, al
	dw	0x00eb, 0x00eb
	out	0xA1, al
	dw	0x00eb, 0x00eb

	mov	al, 0xFF	; スレーブPICのすべての割り込みを
	out	0xA1, al	; 防いでおく
	dw	0x00eb, 0x00eb
	mov	al, 0xFB	; マスターPICのIRQ2番を除いた
	out	0x21, al	; すべての割り込みを防いでおく
	
	ret



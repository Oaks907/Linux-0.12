!
! SYS_SIZE is the number of clicks (16 bytes) to be loaded.
! 0x3000 is 0x30000 bytes = 196kB, more than enough for current
! versions of linux
! SYS_SIZE 是要加载的系统模块长度。 长度单位是节（ paragraph） ，每节 16 字节。 这里 0x3000 共为
! 0x30000 字节=196KB。 若以 1024 字节为 1KB 计，则应该是 192KB。对于当前内核版本这个空间长度
! 已足够了。当该值为 0x8000 时，表示内核最大为 512KB。因为内存 0x90000 处开始存放移动后的
! bootsect 和 setup 的代码，因此该值最大不得超过 0x9000（表示 584KB）。
!
! 头文件 linux/config.h 中定义了内核用到的一些常数符号和 Linus 自己使用的默认硬盘参数块。
! 例如其中定义了以下一些常数：
! DEF_SYSSIZE = 0x3000 - 默认系统模块长度。单位是节，每节为 16 字节；
! DEF_INITSEG = 0x9000 - 默认本程序代码移动目的段位置；
! DEF_SETUPSEG = 0x9020 - 默认 setup 程序代码段位置；
! DEF_SYSSEG = 0x1000 - 默认从磁盘加载系统模块到内存的段位置
#include <linux/config.h>
SYSSIZE = DEF_SYSSIZE
!
!	bootsect.s		(C) 1991 Linus Torvalds
!	modified by Drew Eckhardt
!
! bootsect.s is loaded at 0x7c00 by the bios-startup routines, and moves
! iself out of the way to address 0x90000, and jumps there.
!
! It then loads 'setup' directly after itself (0x90200), and the system
! at 0x10000, using BIOS interrupts. 
!
! NOTE! currently system is at most 8*65536 bytes long. This should be no
! problem, even in the future. I want to keep it simple. This 512 kB
! kernel size should be enough, especially as this doesn't contain the
! buffer cache as in minix
!
! The loader has been made as simple as possible, and continuos
! read errors will result in a unbreakable loop. Reboot by hand. It
! loads pretty fast by getting whole sectors at a time whenever possible.

! bootsect.s 被 ROM BIOS 启动子程序加载至 0x7c00 (31KB)处，并将自己移到了地址 0x90000
! (576KB)处，并跳转至那里。
!
! 它然后使用 BIOS 中断将'setup'直接加载到自己的后面(0x90200)(576.5KB)，并将 system 加
! 载到地址 0x10000 处。
!
! 注意! 目前的内核系统最大长度限制为(8*65536)(512KB)字节，即使是在将来这也应该没有问
! 题的。我想让它保持简单明了。这样 512KB 的最大内核长度应该足够了，尤其是这里没有象
! MINIX 中一样包含缓冲区高速缓冲。
!
! 加载程序已经做得够简单了，所以持续地读操作出错将导致死循环。只能手工重启。只要可能，
! 通过一次读取所有的扇区，加载过程可以做得很快。

! 伪指令（伪操作符） .globl 或.global 用于定义随后的标识符是外部的或全局的，并且即使不
! 使用也强制引入。 .text、 .data 和.bss 用于分别定义当前代码段、数据段和未初始化数据段。
! 在链接多个目标模块时，链接程序（ ld86） 会根据它们的类别把各个目标模块中的相应段分别
! 组合（合并）在一起。这里把三个段都定义在同一重叠地址范围中，因此本程序实际上不分段。
! 另外，后面带冒号的字符串是标号，例如下面的'begtext:'。
! 一条汇编语句通常由标号（可选）、指令助记符（指令名）和操作数三个字段组成。标号位于
! 一条指令的第一个字段。它代表其所在位置的地址，通常指明一个跳转指令的目标位置
.globl begtext, begdata, begbss, endtext, enddata, endbss
.text
begtext:
.data
begdata:
.bss
begbss:
.text

! 下面等号'='或符号'EQU'用于定义标识符或标号所代表的值
SETUPLEN = 4				! nr of setup-sectors 		setup 程序代码占用磁盘扇区数(setup-sectors)值；
BOOTSEG  = 0x07c0			! original address of boot-sector	bootsect 代码所在内存原始段地址；
INITSEG  = DEF_INITSEG			! we move boot here - out of the way 值为0x9000，将 bootsect 移到位置 0x90000 - 避开系统模块占用处；
SETUPSEG = DEF_SETUPSEG			! setup starts here		值为：0x9020，setup 程序从内存 0x90200 处开始；
SYSSEG   = DEF_SYSSEG			! system loaded at 0x10000 (65536).		值为：0x1000,system 模块加载到 0x10000（ 64 KB） 处；
ENDSEG   = SYSSEG + SYSSIZE		! where to stop loading 	值为：0x1000 + 0x3000 停止加载的段地址

! ROOT_DEV & SWAP_DEV are now written by "build".
! 根文件系统设备号 ROOT_DEV 和交换设备号 SWAP_DEV 现在由 tools 目录下的 build 程序写入。
! 设备号 0x306 指定根文件系统设备是第 2 个硬盘的第 1 个分区。 当年 Linus 是在第 2 个硬盘上
! 安装了 Linux 0.11 系统，所以这里 ROOT_DEV 被设置为 0x306。在编译这个内核时你可以根据
! 自己根文件系统所在设备位置修改这个设备号。 例如，若你的根文件系统在第 1 个硬盘的第 1 个
! 分区上，那么该值应该为 0x0301，即（ 0x01, 0x03） 。 这个设备号是 Linux 系统老式的硬盘设备
! 号命名方式，硬盘设备号具体值的含义如下：
! 设备号=主设备号*256 + 次设备号（也即 dev_no = (major<<8) + minor ）
! （主设备号： 1-内存,2-磁盘,3-硬盘,4-ttyx,5-tty,6-并行口,7-非命名管道）
! 0x300 - /dev/hd0 - 代表整个第 1 个硬盘；
! 0x301 - /dev/hd1 - 第 1 个盘的第 1 个分区；
! …
! 0x304 - /dev/hd4 - 第 1 个盘的第 4 个分区；
! 0x305 - /dev/hd5 - 代表整个第 2 个硬盘；
! 0x306 - /dev/hd6 - 第 2 个盘的第 1 个分区；
! …
! 0x309 - /dev/hd9 - 第 2 个盘的第 4 个分区；
! 从 Linux 内核 0.95 版后就已经使用与现在内核相同的命名方法了。
ROOT_DEV = 0	! 根文件系统设备使用与系统引导时同样的设备；
SWAP_DEV = 0	! 交换设备使用与系统引导时同样的设备；


! 伪指令 entry 迫使链接程序在生成的执行程序（ a.out）中包含指定的标识符或标号。这里是
! 程序执行开始点。 49 -- 58 行作用是将自身(bootsect)从目前段位置 0x07c0(31KB) 移动到
! 0x9000(576KB) 处，共 256 字（ 512 字节），然后跳转到移动后代码的 go 标号处，也即本程
! 序的下一语句处。
entry start		! 告知链接程序，程序从 start 标号开始执行
start:
	mov	ax,#BOOTSEG		! 将 ds 段寄存器置为 0x7C0；
	mov	ds,ax
	mov	ax,#INITSEG		! 将 es 段寄存器置为 0x9000；
	mov	es,ax
	mov	cx,#256			! 设置移动计数值=256 字（ 512 字节）；
	sub	si,si			! 将si置为0x0000，源地址 ds:si = 0x07C0:0x0000
	sub	di,di			! 将di置为0x0000目的地址 es:di = 0x9000:0x0000
	rep					! 重复执行并递减 cx 的值，直到 cx = 0 为止
	movw				! 即 movs 指令。从内存[si]处移动 cx 个字到[di]处。
	jmpi	go,INITSEG	! 段间跳转（ Jump Intersegment） 。这里 INITSEG指出跳转到的段地址， 标号 go 是段内偏移地址

go:	mov	ax,cs		
	mov	dx,#0xfef4	! arbitrary value >>512 - disk parm size

	mov	ds,ax
	mov	es,ax
	push	ax

	mov	ss,ax		! put stack at 0x9ff00 - 12.
	mov	sp,dx
/*
 *	Many BIOS's default disk parameter tables will not 
 *	recognize multi-sector reads beyond the maximum sector number
 *	specified in the default diskette parameter tables - this may
 *	mean 7 sectors in some cases.
 *
 *	Since single sector reads are slow and out of the question,
 *	we must take care of this by creating new parameter tables
 *	(for the first disk) in RAM.  We will set the maximum sector
 *	count to 18 - the most we will encounter on an HD 1.44.  
 *
 *	High doesn't hurt.  Low does.
 *
 *	Segments are as follows: ds=es=ss=cs - INITSEG,
 *		fs = 0, gs = parameter table segment
 */


	push	#0
	pop	fs
	mov	bx,#0x78		! fs:bx is parameter table address
	seg fs
	lgs	si,(bx)			! gs:si is source

	mov	di,dx			! es:di is destination
	mov	cx,#6			! copy 12 bytes
	cld

	rep
	seg gs
	movw

	mov	di,dx
	movb	4(di),*18		! patch sector count

	seg fs
	mov	(bx),di
	seg fs
	mov	2(bx),es

	pop	ax
	mov	fs,ax
	mov	gs,ax
	
	xor	ah,ah			! reset FDC 
	xor	dl,dl
	int 	0x13	

! load the setup-sectors directly after the bootblock.
! Note that 'es' is already set up.

load_setup:
	xor	dx, dx			! drive 0, head 0
	mov	cx,#0x0002		! sector 2, track 0
	mov	bx,#0x0200		! address = 512, in INITSEG
	mov	ax,#0x0200+SETUPLEN	! service 2, nr of sectors
	int	0x13			! read it
	jnc	ok_load_setup		! ok - continue

	push	ax			! dump error code
	call	print_nl
	mov	bp, sp
	call	print_hex
	pop	ax	
	
	xor	dl, dl			! reset FDC
	xor	ah, ah
	int	0x13
	j	load_setup

ok_load_setup:

! Get disk drive parameters, specifically nr of sectors/track

	xor	dl,dl
	mov	ah,#0x08		! AH=8 is get drive parameters
	int	0x13
	xor	ch,ch
	seg cs
	mov	sectors,cx
	mov	ax,#INITSEG
	mov	es,ax

! Print some inane message

	mov	ah,#0x03		! read cursor pos
	xor	bh,bh
	int	0x10
	
	mov	cx,#9
	mov	bx,#0x0007		! page 0, attribute 7 (normal)
	mov	bp,#msg1
	mov	ax,#0x1301		! write string, move cursor
	int	0x10

! ok, we've written the message, now
! we want to load the system (at 0x10000)

	mov	ax,#SYSSEG
	mov	es,ax		! segment of 0x010000
	call	read_it
	call	kill_motor
	call	print_nl

! After that we check which root-device to use. If the device is
! defined (!= 0), nothing is done and the given device is used.
! Otherwise, either /dev/PS0 (2,28) or /dev/at0 (2,8), depending
! on the number of sectors that the BIOS reports currently.

	seg cs
	mov	ax,root_dev
	or	ax,ax
	jne	root_defined
	seg cs
	mov	bx,sectors
	mov	ax,#0x0208		! /dev/ps0 - 1.2Mb
	cmp	bx,#15
	je	root_defined
	mov	ax,#0x021c		! /dev/PS0 - 1.44Mb
	cmp	bx,#18
	je	root_defined
undef_root:
	jmp undef_root
root_defined:
	seg cs
	mov	root_dev,ax

! after that (everyting loaded), we jump to
! the setup-routine loaded directly after
! the bootblock:

	jmpi	0,SETUPSEG

! This routine loads the system at address 0x10000, making sure
! no 64kB boundaries are crossed. We try to load it as fast as
! possible, loading whole tracks whenever we can.
!
! in:	es - starting address segment (normally 0x1000)
!
sread:	.word 1+SETUPLEN	! sectors read of current track
head:	.word 0			! current head
track:	.word 0			! current track

read_it:
	mov ax,es
	test ax,#0x0fff
die:	jne die			! es must be at 64kB boundary
	xor bx,bx		! bx is starting address within segment
rp_read:
	mov ax,es
	cmp ax,#ENDSEG		! have we loaded all yet?
	jb ok1_read
	ret
ok1_read:
	seg cs
	mov ax,sectors
	sub ax,sread
	mov cx,ax
	shl cx,#9
	add cx,bx
	jnc ok2_read
	je ok2_read
	xor ax,ax
	sub ax,bx
	shr ax,#9
ok2_read:
	call read_track
	mov cx,ax
	add ax,sread
	seg cs
	cmp ax,sectors
	jne ok3_read
	mov ax,#1
	sub ax,head
	jne ok4_read
	inc track
ok4_read:
	mov head,ax
	xor ax,ax
ok3_read:
	mov sread,ax
	shl cx,#9
	add bx,cx
	jnc rp_read
	mov ax,es
	add ah,#0x10
	mov es,ax
	xor bx,bx
	jmp rp_read

read_track:
	pusha
	pusha	
	mov	ax, #0xe2e 	! loading... message 2e = .
	mov	bx, #7
 	int	0x10
	popa		

	mov dx,track
	mov cx,sread
	inc cx
	mov ch,dl
	mov dx,head
	mov dh,dl
	and dx,#0x0100
	mov ah,#2
	
	push	dx				! save for error dump
	push	cx
	push	bx
	push	ax

	int 0x13
	jc bad_rt
	add	sp, #8   	
	popa
	ret

bad_rt:	push	ax				! save error code
	call	print_all			! ah = error, al = read
	
	
	xor ah,ah
	xor dl,dl
	int 0x13
	

	add	sp, #10
	popa	
	jmp read_track

/*
 *	print_all is for debugging purposes.  
 *	It will print out all of the registers.  The assumption is that this is
 *	called from a routine, with a stack frame like
 *	dx 
 *	cx
 *	bx
 *	ax
 *	error
 *	ret <- sp
 *
*/
 
print_all:
	mov	cx, #5		! error code + 4 registers
	mov	bp, sp	

print_loop:
	push	cx		! save count left
	call	print_nl	! nl for readability
	jae	no_reg		! see if register name is needed
	
	mov	ax, #0xe05 + 0x41 - 1
	sub	al, cl
	int	0x10

	mov	al, #0x58 	! X
	int	0x10

	mov	al, #0x3a 	! :
	int	0x10

no_reg:
	add	bp, #2		! next register
	call	print_hex	! print it
	pop	cx
	loop	print_loop
	ret

print_nl:
	mov	ax, #0xe0d	! CR
	int	0x10
	mov	al, #0xa	! LF
	int 	0x10
	ret

/*
 *	print_hex is for debugging purposes, and prints the word
 *	pointed to by ss:bp in hexadecmial.
*/

print_hex:
	mov	cx, #4		! 4 hex digits
	mov	dx, (bp)	! load word into dx
print_digit:
	rol	dx, #4		! rotate so that lowest 4 bits are used
	mov	ah, #0xe	
	mov	al, dl		! mask off so we have only next nibble
	and	al, #0xf
	add	al, #0x30	! convert to 0 based digit, '0'
	cmp	al, #0x39	! check for overflow
	jbe	good_digit
	add	al, #0x41 - 0x30 - 0xa 	! 'A' - '0' - 0xa

good_digit:
	int	0x10
	loop	print_digit
	ret


/*
 * This procedure turns off the floppy drive motor, so
 * that we enter the kernel in a known state, and
 * don't have to worry about it later.
 */
kill_motor:
	push dx
	mov dx,#0x3f2
	xor al, al
	outb
	pop dx
	ret

sectors:
	.word 0

msg1:
	.byte 13,10
	.ascii "Loading"

.org 506
swap_dev:
	.word SWAP_DEV
root_dev:
	.word ROOT_DEV
boot_flag:
	.word 0xAA55

.text
endtext:
.data
enddata:
.bss
endbss:


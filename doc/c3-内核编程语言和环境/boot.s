!
! boot.s -- bootsect.s 的框架程序。用代码 0x07 替换串 msg1 中 1 字符，然后在屏幕第 1 行上显示。
!
.globl begtext, begdata, begbss, endtext, enddata, endbss ! 全局标识符，供 ld86 链接使用；
.text ! 正文段；
begtext:
.data ! 数据段；
begdata:
.bss ! 未初始化数据段；
 begbss:
 .text ! 正文段；
 BOOTSEG = 0x07c0 ! BIOS 加载 bootsect 代码的原始段地址；

 entry start ! 告知链接程序，程序从 start 标号处开始执行。
 start:
 jmpi go,BOOTSEG ! 段间跳转。 INITSEG 指出跳转段地址， 标号 go 是偏移地址。
 go: mov ax,cs ! 段寄存器 cs 值-->ax，用于初始化数据段寄存器 ds 和 es。
 mov ds,ax
 mov es,ax
 mov [msg1+17],ah ! 0x07-->替换字符串中 1 个点符号，喇叭将会鸣一声。
 mov cx,#20 ! 共显示 20 个字符，包括回车换行符。
 mov dx,#0x1004 ! 字符串将显示在屏幕第 17 行、第 5 列处。
 mov bx,#0x000c ! 字符显示属性（红色）。
 mov bp,#msg1 ! 指向要显示的字符串（中断调用要求）。
 mov ax,#0x1301 ! 写字符串并移动光标到串结尾处。
 int 0x10 ! BIOS 中断调用 0x10，功能 0x13，子功能 01。
 loop1: jmp loop1 ! 死循环。
 msg1: .ascii "Loading system ..."! 调用 BIOS 中断显示的信息。共 20 个 ASCII 码字符。
 .byte 13,10
 .org 510 ! 表示以后语句从地址 510(0x1FE)开始存放。
 .word 0xAA55 ! 有效引导扇区标志， 供 BIOS 加载引导扇区使用。
 .text
 endtext:
 .data
 enddata:
 .bss
 endbss:
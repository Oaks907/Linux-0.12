/*
 *  linux/kernel/sched.c
 *
 *  (C) 1991  Linus Torvalds
 */

/*
 * 'sched.c' is the main kernel file. It contains scheduling primitives
 * (sleep_on, wakeup, schedule etc) as well as a number of simple system
 * call functions (type getpid(), which just extracts a field from
 * current-task
 */
/*
* 'sched.c'是主要的内核文件。其中包括有关调度的基本函数(sleep_on、 wakeup、 schedule 等)
* 以及一些简单的系统调用函数（比如 getpid()，仅从当前任务中获取一个字段）。
*/
// 下面是调度程序头文件。定义了任务结构 task_struct、第 1 个初始任务的数据。还有一些以宏
// 的形式定义的有关描述符参数设置和获取的嵌入式汇编函数程序。
#include <linux/sched.h>
#include <linux/kernel.h>	// 内核头文件。含有一些内核常用函数的原形定义。
#include <linux/sys.h>		// 系统调用头文件。含有 82 个系统调用 C 函数程序,以'sys_'开头。
#include <linux/fdreg.h>	// 软驱头文件。含有软盘控制器参数的一些定义
#include <asm/system.h>		// 系统头文件。定义了设置或修改描述符/中断门等的嵌入式汇编宏。
#include <asm/io.h>			// io 头文件。定义硬件端口输入/输出宏汇编语句。
#include <asm/segment.h>	// 段操作头文件。定义了有关段寄存器操作的嵌入式汇编函数。

#include <signal.h>			// 信号头文件。定义信号符号常量， sigaction 结构，操作函数原型

// 该宏取信号 nr 在信号位图中对应位的二进制数值。信号编号 1-32。
// 例如，信号 5 的位图数值等于 1<<(5-1) = 16 = 00010000b。
// 另外，除了 SIGKILL 和 SIGSTOP 信号以外其他信号都是可阻塞的(…1011,1111,1110,1111,1111b)
#define _S(nr) (1<<((nr)-1))
#define _BLOCKABLE (~(_S(SIGKILL) | _S(SIGSTOP)))

// 内核调试函数。显示任务号 nr 的进程号、进程状态、 内核堆栈空闲字节数（大约） 及其相关的
// 子进程和父进程信息。
// 因为任务结构的数据和任务的内核态栈在同一内存页面上，且任务内核态栈从页面末端开始，
// 因此， 28 行上的 j 即表示最大内核栈容量，或内核栈最低顶端位置。
// 参数：
// nr - 任务号； p - 任务结构指针。
void show_task(int nr,struct task_struct * p)
{
	int i,j = 4096-sizeof(struct task_struct);

	printk("%d: pid=%d, state=%d, father=%d, child=%d, ",nr,p->pid,
		p->state, p->p_pptr->pid, p->p_cptr ? p->p_cptr->pid : -1);
	i=0;
	while (i<j && !((char *)(p+1))[i])	// 检测指定任务数据结构以后等于 0 的字节数
		i++;
	printk("%d/%d chars free in kstack\n\r",i,j);
	printk("   PC=%08X.", *(1019 + (unsigned long *) p));
	if (p->p_ysptr || p->p_osptr) 
		printk("   Younger sib=%d, older sib=%d\n\r", 
			p->p_ysptr ? p->p_ysptr->pid : -1,
			p->p_osptr ? p->p_osptr->pid : -1);
	else
		printk("\n\r");
}

// 显示系统中所有任务的状态信息。
// NR_TASKS 是系统能容纳的最大任务数量(64 个)，定义在 include/linux/sched.h 第 6 行。
void show_state(void)
{
	int i;

	printk("\rTask-info:\n\r");
	for (i=0;i<NR_TASKS;i++)
		if (task[i])
			show_task(i,task[i]);
}

// PC 机 8253 计数/定时芯片的输入时钟频率约为 1.193180MHz。 Linux 内核希望定时器中断频率是
// 100Hz，也即每 10ms 发出一次时钟中断。因此这里 LATCH 是设置 8253 芯片的初值，参见 438 行。
#define LATCH (1193180/HZ)

extern void mem_use(void);	// [??]没有任何地方定义和引用该函数

extern int timer_interrupt(void);	// 定时中断程序（ kernel/system_call.s， 189）。
extern int system_call(void);		// 系统调用中断程序（ kernel/system_call.s， 84）。

// 每个任务（进程）在内核态运行时都有自己的内核态堆栈。这里定义了任务的内核态堆栈结构。
// 这里定义任务联合（任务结构成员和 stack 字符数组成员）。因为一个任务的数据结构与其内核
// 态堆栈放在同一内存页中，所以从堆栈段寄存器 ss 可以获得其数据段选择符。
// 67 行上设置初始任务的数据。初始数据在 include/linux/sched.h 中，第 156 行开始
union task_union {
	struct task_struct task;
	char stack[PAGE_SIZE];
};

static union task_union init_task = {INIT_TASK,};

// 从开机算起的滴答数（ 10ms/滴答）。系统时钟中断每发生一次即一个滴答。
// 前面的限定符 volatile，英文解释意思是易变的、不稳定的。 这个限定词的含义是向编译器
// 指明变量的内容可能会因被其他程序修改而变化。通常在程序中申明一个变量时， 编译器会
// 尽量把它存放在通用寄存器中，例如 EBX，以提高访问效率。 此后编译器就不会再关心该变量
// 在对应内存位置中原来的内容。若此时其他程序（例如内核程序或中断过程）修改了该变量
// 在对应内存位置处的值， EBX 中的值并不会随之更新。为了解决这种情况就创建了 volatile
// 限定符，让代码在引用该变量时一定要从指定内存位置中取得其值。 这里即要求 gcc 不要对
// jiffies 进行优化处理，也不要挪动位置，并且需要从内存中取其值。因为时钟中断处理过程
// 等程序会修改它的值。
unsigned long volatile jiffies=0;		// 内核脉搏（嘀嗒）
unsigned long startup_time=0;			// 开机时间。从 1970:0:0:0 开始计时的秒数。
int jiffies_offset = 0;		/* # clock ticks to add to get "true
				   time".  Should always be less than
				   1 second's worth.  For time fanatics
				   who like to syncronize their machines
				   to WWV :-) */
/* 为调整时钟而需要增加的时钟嘀嗒数，以获得“精确时间”。这些调整用嘀嗒数
* 的总和不应该超过 1 秒。这样做是为了那些对时间精确度要求苛刻的人，他们喜
* 欢自己的机器时间与 WWV 同步 :-)
*/

struct task_struct *current = &(init_task.task);	// 当前任务指针（初始化指向任务 0）。
struct task_struct *last_task_used_math = NULL;		// 使用过协处理器任务的指针。

// 定义任务指针数组。第 1 项被初始化指向初始任务（任务 0）的任务数据结构。
struct task_struct * task[NR_TASKS] = {&(init_task.task), };

// 定义用户堆栈（数组） ，共 1K 项，容量 4K 字节。在刚开始内核初始化操作过程中被用作内核栈，
// 初始化操作完成后将被用作任务 0 的用户态堆栈。在运行任务 0 之前它是内核栈，以后用作任务 0
// 和任务 1 的用户态栈。下面结构用于设置堆栈 SS:ESP（数据段选择符， 偏移），见 head.s， 23 行。
// SS 被设置为内核数据段选择符（ 0x10）， ESP 被设置为指向 user_stack 数组最后一项后面。这是
// 因为 Intel CPU 执行堆栈操作时总是先递减堆栈指针 ESP 值，然后在 ESP 指针处保存入栈内容
long user_stack [ PAGE_SIZE>>2 ] ;

struct {
	long * a;
	short b;
	} stack_start = { & user_stack [PAGE_SIZE>>2] , 0x10 };
/*
 *  'math_state_restore()' saves the current math information in the
 * old math state array, and gets the new ones from the current task
 */
/*
* 将当前协处理器内容保存到老协处理器状态数组中，并将当前任务的协处理器
* 内容加载进协处理器。
*/
// 当任务被调度交换过以后，该函数用以保存原任务的协处理器状态（上下文）并恢复新调度进
// 来的当前任务的协处理器执行状态。
void math_state_restore()
{
// 如果任务没变则返回(上一个任务就是当前任务)。这里"上一个任务"是指刚被交换出去的任务。
// 另外在发送协处理器命令之前要先发 WAIT 指令。如果上一个任务使用了协处理器，则保存其
// 状态至任务数据结构的 TSS 字段中。
	if (last_task_used_math == current)
		return;
	__asm__("fwait");
	if (last_task_used_math) {
		__asm__("fnsave %0"::"m" (last_task_used_math->tss.i387));
	}
// 现在， last_task_used_math 指向当前任务，以备当前任务被交换出去时使用。此时如果当前
// 任务用过协处理器，则恢复其状态。否则的话说明是第一次使用，于是就向协处理器发初始化
// 命令，并设置使用了协处理器标志。
	last_task_used_math=current;
	if (current->used_math) {
		__asm__("frstor %0"::"m" (current->tss.i387));
	} else {
		__asm__("fninit"::);	// 向协处理器发初始化命令
		current->used_math=1;	// 设置使用已协处理器标志。
	}
}

/*
 *  'schedule()' is the scheduler function. This is GOOD CODE! There
 * probably won't be any reason to change this, as it should work well
 * in all circumstances (ie gives IO-bound processes good response etc).
 * The one thing you might take a look at is the signal-handler code here.
 *
 *   NOTE!!  Task 0 is the 'idle' task, which gets called when no other
 * tasks can run. It can not be killed, and it cannot sleep. The 'state'
 * information in task[0] is never used.
 */
/*
* 'schedule()'是调度函数。这是个很好的代码！没有任何理由对它进行修改，因为
* 它可以在所有的环境下工作（比如能够对 IO-边界处理很好的响应等）。只有一件
* 事值得留意，那就是这里的信号处理代码。
*
* 注意！！任务 0 是个闲置('idle')任务，只有当没有其他任务可以运行时才调用
* 它。它不能被杀死，也不能睡眠。任务 0 中的状态信息'state'是从来不用的。
*/
void schedule(void)
{
	int i,next,c;
	struct task_struct ** p;

/* check alarm, wake up any interruptible tasks that have got a signal */
/* 检测 alarm（进程的报警定时值），唤醒任何已得到信号的可中断任务 */

// 从任务数组中最后一个任务开始循环检测 alarm。在循环时跳过空指针项
	for(p = &LAST_TASK ; p > &FIRST_TASK ; --p)
		if (*p) {
// 如果设置过任务超时定时值 timeout，并且已经超时，则复位超时定时值，并且如果任务处于可
// 中断睡眠状态 TASK_INTERRUPTIBLE 下，将其置为就绪状态（ TASK_RUNNING）。
			if ((*p)->timeout && (*p)->timeout < jiffies) {
				(*p)->timeout = 0;
				if ((*p)->state == TASK_INTERRUPTIBLE)
					(*p)->state = TASK_RUNNING;
			}
// 如果设置过任务的 SIGALRM 信号超时定时器值 alarm，并且已经过期(alarm<jiffies),则在信号
// 位图中置 SIGALRM 信号，即向任务发送 SIGALRM 信号。然后清 alarm。该信号的默认操作是终止
// 进程。 jiffies 是系统从开机开始算起的滴答数（ 10ms/滴答）。定义在 sched.h 第 139 行
			if ((*p)->alarm && (*p)->alarm < jiffies) {
				(*p)->signal |= (1<<(SIGALRM-1));
				(*p)->alarm = 0;
			}
// 如果信号位图中除被阻塞的信号外还有其他信号，并且任务处于可中断状态，则置任务为就绪
// 状态。其中'~(_BLOCKABLE & (*p)->blocked)'用于忽略被阻塞的信号，但 SIGKILL 和 SIGSTOP
// 不能被阻塞
			if (((*p)->signal & ~(_BLOCKABLE & (*p)->blocked)) &&
			(*p)->state==TASK_INTERRUPTIBLE)
				(*p)->state=TASK_RUNNING;	//置为就绪（可执行）状态
		}

/* this is the scheduler proper: */
/* 这里是调度程序的主要部分 */

	while (1) {
		c = -1;
		next = 0;
		i = NR_TASKS;
		p = &task[NR_TASKS];
// 这段代码也是从任务数组的最后一个任务开始循环处理，并跳过不含任务的数组槽。 它比较每个
// 就绪状态任务的 counter（任务运行时间的递减滴答计数）值，哪一个值大， 就表示相应任务的
// 运行时间还有很多， next 就指向哪个的任务号。
		while (--i) {
			// 不含任务
			if (!*--p)
				continue;
			if ((*p)->state == TASK_RUNNING && (*p)->counter > c)
				c = (*p)->counter, next = i;
		}
// 如果比较得出有 counter 值不等于 0 的结果，或者系统中没有一个可运行的任务存在（此时 c 仍
// 然为-1， next=0），则退出外层 while 循环(144 行)， 并执行后面的任务切换操作（ 161 行） 。
// 否则就根据每个任务的优先权值，更新每一个任务的 counter 值，然后再回到 144 行重新比较。
// counter 值的计算方式为 counter = counter/2 + priority
// 注意，这里计算过程不考虑进程的状态
		if (c) break;
		for(p = &LAST_TASK ; p > &FIRST_TASK ; --p)
			if (*p)
				(*p)->counter = ((*p)->counter >> 1) +
						(*p)->priority;
	}
// 下面宏（ 在 sched.h 中）把上面选出来的任务 next 作为当前任务 current， 并切换到该任务
// 中运行。 因为 next 在 146 行上被初始化为 0， 因此若系统中没有任何其他任务可运行时，则
// next 始终为 0。 结果是调度函数会在系统空闲时去执行任务 0。此时任务 0 仅执行 pause()
// 系统调用，并又会调用本函数。
	switch_to(next);	// 切换到任务号为 next 的任务，并运行之。
}

//// 下面是 pause()系统调用，用于转换当前任务的状态为可中断的等待状态，并重新调度。
// 该系统调用将导致进程进入睡眠状态，直到收到一个信号。该信号用于终止进程或者使进程
// 调用一个信号捕获函数。只有当捕获了一个信号，并且信号捕获处理函数返回， pause()才
// 会返回。此时 pause()返回值应该是 -1，并且 errno 被置为 EINTR。这里还没有完全实现
// （直到 0.95 版）。
int sys_pause(void)
{
	current->state = TASK_INTERRUPTIBLE;
	schedule();
	return 0;
}

// 下面函数把当前任务置为可中断的或不可中断的睡眠状态，并让睡眠队列头指针指向当前任务。
// 函数参数 p 是等待任务队列头指针。指针是含有一个变量地址的变量。这里参数 p 使用了指针的
// 指针形式 '**p'，这是因为 C 函数参数只能传值，没有直接的方式让被调用函数改变调用该函数
// 程序中变量的值。但是指针'*p'指向的目标（这里是任务结构）会改变，因此为了能修改调用该
// 函数程序中原来就是指针变量的值，就需要传递指针'*p'的指针，即'**p'。参见程序前示例图中
// p 指针的使用情况。
// 参数 state 是任务睡眠使用的状态： TASK_UNINTERRUPTIBLE 或 TASK_INTERRUPTIBLE。处于不可
// 中断睡眠状态（ TASK_UNINTERRUPTIBLE）的任务需要内核程序利用 wake_up()函数明确唤醒之。
// 处于可中断睡眠状态（ TASK_INTERRUPTIBLE） 的任务可以通过信号、任务超时等手段唤醒
//（置为就绪状态 TASK_RUNNING）。
// *** 注意，由于本内核代码不是很成熟，因此下列与睡眠相关的代码还存在一些问题。
static inline void __sleep_on(struct task_struct **p, int state)
{
	struct task_struct *tmp;

// 若指针无效，则退出。（指针所指的对象可以是 NULL，但指针本身不会为 0)。
// 如果当前任务是任务 0，则死机(impossible!)。
	if (!p)
		return;
	if (current == &(init_task.task))
		panic("task[0] trying to sleep");
// 让 tmp 指向已经在等待队列上的任务(如果有的话)，例如 inode->i_wait， 并且将睡眠队列头
// 的指针指向当前任务。这样就把当前任务插入到 *p 的等待队列中。然后将当前任务置为指定
// 的等待状态，并执行重新调度。
	tmp = *p;
	*p = current;
	current->state = state;
repeat:	schedule();
// 只有当这个等待任务被唤醒时，程序才又会从这里继续执行。 表示进程已被明确地唤醒并执行。
// 如果队列中还有等待的任务，并且队列头指针 *p 所指向的任务不是当前任务， 则说明在本任务
// 插入队列后还有任务进入队列， 于是我们应该也要唤醒这些后续进入队列的任务， 因此这里将队
// 列头所指任务先置为就绪状态，而自己则置为不可中断等待状态，即要等待这些后续进入队列的
// 任务被唤醒后才用 wake_up()唤醒本任务。然后跳转至 repeat 标号处重新执行调度函数。
	if (*p && *p != current) {
		(**p).state = 0;
		current->state = TASK_UNINTERRUPTIBLE;
		goto repeat;
	}
// 执行到这里，说明任务被真正被唤醒执行。此时等待队列头指针应该指向本任务。 若它为空，
// 则表明调度有问题，于是显示警告信息。最后我们让头指针指向在我们的前面进入队列的任务
//（ *p = tmp）。 若确实存在这样一个任务，即队列中还有任务（ tmp 不为空），就唤醒之。因
// 此， 最先进入队列的任务在唤醒后运行时最终会把等待队列头指针置成 NULL。
	if (!*p)
		printk("Warning: *P = NULL\n\r");
	if (*p = tmp)
		tmp->state=0;
}
// 将当前任务置为可中断的等待状态（ TASK_INTERRUPTIBLE） ， 并被放入头指针*p 指定的等待
// 队列中。 这种等待状态的任务可以通过信号、任务超时等手段唤醒。
void interruptible_sleep_on(struct task_struct **p)
{
	__sleep_on(p,TASK_INTERRUPTIBLE);
}
// 把当前任务置为不可中断的等待状态（ TASK_UNINTERRUPTIBLE） ，并让睡眠队列头指针指向
// 当前任务。 这种等待状态的任务需要利用 wake_up()函数来明确唤醒。 该函数提供了进程与中
// 断处理程序之间的同步机制。
void sleep_on(struct task_struct **p)
{
	__sleep_on(p,TASK_UNINTERRUPTIBLE);
}
// 唤醒不可中断等待任务。 *p 是任务等待队列头指针。由于新等待任务是插入在等待队列头指针
// 处的，因此唤醒的是最后进入等待队列的任务。若该任务已经处于停止或僵死状态，则显示告警信息
void wake_up(struct task_struct **p)
{
	if (p && *p) {
		if ((**p).state == TASK_STOPPED)	// 处于停止状态
			printk("wake_up: TASK_STOPPED");
		if ((**p).state == TASK_ZOMBIE)		// 处于僵死状态。
			printk("wake_up: TASK_ZOMBIE");
		(**p).state=0;						// 置为就绪状态 TASK_RUNNING
	}
}

/*
 * OK, here are some floppy things that shouldn't be in the kernel
 * proper. They are here because the floppy needs a timer, and this
 * was the easiest way of doing it.
 */
/*
* 好了，从这里开始是一些有关软盘的子程序，本不应该放在内核的主要部分
* 中的。将它们放在这里是因为软驱需要定时处理，而放在这里是最方便的。
*/
// 下面 220 -- 281 行代码用于处理软驱定时。在阅读这段代码之前请先看一下块设备一章中有关
// 软盘驱动程序（ floppy.c） 后面的说明， 或者到阅读软盘块设备驱动程序时再来看这段代码。
//
// 数组 wait_motor[]用于存放等待马达启动到正常转速的进程指针。数组索引 0-3 分别对应软驱 A-D。
// 数组 mon_timer[]存放各软驱马达启动所需要的滴答数。默认启动时间为 50 个滴答（ 0.5 秒）。
// 数组 moff_timer[]存放各软驱在马达停转之前需维持的时间。程序中设定为 10000 个滴答（ 100 秒）。
static struct task_struct * wait_motor[4] = {NULL,NULL,NULL,NULL};
static int  mon_timer[4]={0,0,0,0};
static int moff_timer[4]={0,0,0,0};
// 下面变量对应软驱控制器中当前数字输出寄存器（ DOR） 。该寄存器每位的定义如下：
// 位 7-4：分别控制驱动器 D-A 马达的启动。 1 - 启动； 0 - 关闭。
// 位 3 ： 1 - 允许 DMA 和中断请求； 0 - 禁止 DMA 和中断请求。
// 位 2 ： 1 - 启动软盘控制器； 0 - 复位软盘控制器。
// 位 1-0： 00b-11b 用于选择软驱 A-D。
// 这里设置初值为：允许 DMA 和中断请求、启动 FDC。
unsigned char current_DOR = 0x0C;

// 指定软驱启动到正常运转状态所需等待时间。
// 参数 nr -- 软驱号(0--3)，返回值为滴答数。
// 变量 selected 是选中软驱标志（ blk_drv/floppy.c， 123 行）。 mask 是所选软驱对应的
// 数字输出寄存器 DOR 中启动马达比特位。 mask 高 4 位是各软驱启动马达标志。
int ticks_to_floppy_on(unsigned int nr)
{
	extern unsigned char selected;
	unsigned char mask = 0x10 << nr;

// 系统最多有 4 个软驱。首先预先设置好指定软驱 nr 停转之前需要经过的时间（ 100 秒）。然后
// 取当前数字输出寄存器 DOR 值到临时变量 mask 中，并把指定软驱的马达启动标志置位
	if (nr>3)
		panic("floppy_on: nr>3");
	moff_timer[nr]=10000;		/* 100 s = very big :-) */
	cli();				/* use floppy_off to turn it off */		// 关中断
	mask |= current_DOR;
// 如果当前没有选择软驱，则首先复位其他软驱的选择位，然后置指定软驱选择位。
	if (!selected) {
		mask &= 0xFC;
		mask |= nr;
	}
// 如果数字输出寄存器 DOR 的当前值与要求值不同，则向 FDC 数字输出端口 FD_DOR 输出新值(mask)，
// 并且如果要求启动的马达还没有启动，则置相应软驱的马达启动定时器值（ HZ/2 = 0.5 秒或 50 个
// 滴答）。 若已经启动，则再设置启动定时为 2 个滴答，能满足下面 do_floppy_timer()中先递减后
// 判断的要求。此后更新当前数字输出寄存器 current_DOR。
// 240 行上的 FD_DOR 是软驱数字输出寄存器 DOR 的端口（ 0x3F2） ,参见程序后的说明。
	if (mask != current_DOR) {
		outb(mask,FD_DOR);
		if ((mask ^ current_DOR) & 0xf0)
			mon_timer[nr] = HZ/2;
		else if (mon_timer[nr] < 2)
			mon_timer[nr] = 2;
		current_DOR = mask;
	}
	sti();					// 开中断。
	return mon_timer[nr];	// 最后返回启动马达所需的时间值。
}

// 等待指定软驱马达启动所需的一段时间。
// 设置指定软驱的马达启动到正常转速所需的延时，然后睡眠等待。在定时中断过程中会一直
// 递减判断这里设定的延时值。当延时到期，就会唤醒这里的等待进程。
void floppy_on(unsigned int nr)
{
// 关中断。 如果马达启动定时还没到，就一直把当前进程置为不可中断睡眠状态并放入等待马达
// 运行的队列中。然后开中断。
	cli();
	while (ticks_to_floppy_on(nr))
		sleep_on(nr+wait_motor);
	sti();
}

// 设置关闭相应软驱马达停转定时器（ 3 秒）。
// 若不使用该函数明确关闭指定的软驱马达，则在马达开启 100 秒之后也会被关闭
void floppy_off(unsigned int nr)
{
	moff_timer[nr]=3*HZ;
}

// 软盘定时器处理子程序。更新马达启动定时值和马达关闭停转计时值。该子程序会在时钟定时
// 中断过程中被调用，因此系统每经过一个滴答(10ms)就会被调用一次，随时更新马达开启或停
// 转定时器的值。如果某一个马达停转定时到，则将数字输出寄存器马达启动位复位。
void do_floppy_timer(void)
{
	int i;
	unsigned char mask = 0x10;

// 针对系统中具有的 4 个软驱，逐一检查使用中的软驱。如果不是 DOR 指定的马达， 则跳过。
	for (i=0 ; i<4 ; i++,mask <<= 1) {
		if (!(mask & current_DOR))		// 如果不是 DOR 指定的马达则跳过。
			continue;
		if (mon_timer[i]) {				// 如果马达启动定时到则唤醒进程。
			if (!--mon_timer[i])
				wake_up(i+wait_motor);
		} else if (!moff_timer[i]) {	// 如果马达停转定时到则
			current_DOR &= ~mask;		// 复位相应马达启动位，并且
			outb(current_DOR,FD_DOR);	// 更新数字输出寄存器。
		} else
			moff_timer[i]--;			// 否则马达停转计时递减。
	}
}

// 下面是关于内核定时器的代码。最多可有 64 个定时器。
#define TIME_REQUESTS 64

// 定时器链表结构和定时器数组。该定时器链表专用于供软驱关闭马达和启动马达定时操作。
// 这种类型定时器类似现代 Linux 系统中的动态定时器（ Dynamic Timer），仅供内核使用
static struct timer_list {
	long jiffies;				// 定时滴答数。
	void (*fn)();				// 定时处理程序。
	struct timer_list * next;	// 链接指向下一个定时器。
} timer_list[TIME_REQUESTS], * next_timer = NULL;	// next_timer 是定时器队列头指针。

// 添加定时器。输入参数为指定的定时值(滴答数)和相应的处理程序指针。
// 软盘驱动程序（ floppy.c） 利用该函数执行启动或关闭马达的延时操作。
// 参数 jiffies – 以 10 毫秒计的滴答数； *fn()- 定时时间到时执行的函数。
void add_timer(long jiffies, void (*fn)(void))
{
	struct timer_list * p;

// 如果定时处理程序指针为空，则退出。否则关中断
	if (!fn)
		return;
	cli();
	// 如果定时值<=0，则立刻调用其处理程序。并且该定时器不加入链表中
	if (jiffies <= 0)
		(fn)();
	else {
		// 否则从定时器数组中，找一个空闲项。
		for (p = timer_list ; p < timer_list + TIME_REQUESTS ; p++)
			if (!p->fn)
				break;
// 如果已经用完了定时器数组，则系统崩溃。否则向定时器数据结构填入相应信息，并链入
// 链表头。
		if (p >= timer_list + TIME_REQUESTS)
			panic("No more time requests free");
		p->fn = fn;
		p->jiffies = jiffies;
		p->next = next_timer;
		next_timer = p;
// 链表项按定时值从小到大排序。在排序时减去排在前面需要的滴答数，这样在处理定时器时
// 只要查看链表头的第一项的定时是否到期即可
		while (p->next && p->next->jiffies < p->jiffies) {
			p->jiffies -= p->next->jiffies;
			fn = p->fn;
			p->fn = p->next->fn;
			p->next->fn = fn;
			jiffies = p->jiffies;
			p->jiffies = p->next->jiffies;
			p->next->jiffies = jiffies;
			p = p->next;
		}
	}
	sti();
}

//// 定时器中断 C 函数处理程序，在 sys_call.s 中的_timer_interrupt（ 189 行） 中被调用。
// 参数 cpl 是当前特权级 0 或 3， 它是时钟中断发生时正被执行的代码选择符中的特权级。
// cpl=0 时表示中断发生时正在执行内核代码； cpl=3 时表示中断发生时正在执行用户代码。
// 对于一个任务，若其执行时间片用完，则进行任务切换。同时函数执行一个计时更新工作。
void do_timer(long cpl)
{
	static int blanked = 0;

// 首先判断是否需要执行黑屏（ blankout） 操作。如果 blankcount 计数不为零， 或者黑屏
// 延时间隔时间 blankinterval 为 0 的话，那么若已经处于黑屏状态（黑屏标志 blanked =1），
// 则让屏幕恢复显示。若 blankcount 计数不为零，则递减之，并且复位黑屏标志。
	if (blankcount || !blankinterval) {
		if (blanked)
			unblank_screen();
		if (blankcount)
			blankcount--;
		blanked = 0;
		// 否则的话若黑屏标志未置位，则让屏幕黑屏，并且设置黑屏标志。
	} else if (!blanked) {
		blank_screen();
		blanked = 1;
	}
// 接着处理硬盘操作超时问题。如果硬盘超时计数递减之后为 0，则进行硬盘访问超时处理
	if (hd_timeout)
		if (!--hd_timeout)
			hd_times_out();	// 硬盘访问超时处理（ blk_drv/hdc， 318 行）。

// 如果发声计数次数到，则关闭发声。 (向 0x61 口发送命令，复位位 0 和 1。位 0 控制 8253
// 计数器 2 的工作，位 1 控制扬声器)。
	if (beepcount)				// 扬声器发声时间滴答数（ chr_drv/console.c,950 行）
		if (!--beepcount)		 // 停止扬声器发生（ chr_drv/console.c, 944 行） 。
			sysbeepstop();

// 如果当前特权级(cpl)为 0（最高，表示是内核程序在工作），则将内核代码运行时间 stime
// 递增； [ Linus 把内核程序统称为超级用户(supervisor)的程序，见 sys_call.s， 207 行
// 上的英文注释。这种称呼来自于 Intel CPU 手册。 ] 如果 cpl > 0，则表示是一般用户程序
// 在工作，增加 utime
	if (cpl)
		current->utime++;
	else
		current->stime++;
// 如果有定时器存在，则将链表第 1 个定时器的值减 1。如果已等于 0，则调用相应的处理程序，
// 并将该处理程序指针置为空。然后去掉该项定时器
	if (next_timer) {				// 定时器链表的头指针。
		next_timer->jiffies--;
		while (next_timer && next_timer->jiffies <= 0) {
			void (*fn)(void);		// 这里插入了一个函数指针定义！！
			
			fn = next_timer->fn;
			next_timer->fn = NULL;
			next_timer = next_timer->next;
			(fn)();					// 调用定时处理函数
		}
	}
// 如果当前软盘控制器 FDC 的数字输出寄存器 DOR 中马达启动位有置位的，则执行软盘定时程序
	if (current_DOR & 0xf0)
		do_floppy_timer();
// 如果任务运行时间还没用完，则退出这里继续运行该任务。否则置当前任务运行计数值为 0。并且
// 若发生时钟中断时正在内核代码中运行则返回，否则表示在执行用户程序，于是调用调度函数尝试
// 执行任务切换操作。
	if ((--current->counter)>0) return;
	current->counter=0;
	if (!cpl) return;	// 对于内核态程序，不依赖 counter 值进行调度。
	schedule();
}

// 系统调用功能 - 设置报警定时器时间值(秒)。
// 若参数 seconds > 0，则设置新定时时间值，并返回原定时时间刻还剩余的间隔时间， 否则返回 0。
// 进程数据结构中报警字段 alarm 的单位是系统滴答，它是系统开机运行到现在的嘀嗒数 jiffies
// 与定时值之和，即'jiffies + HZ*定时秒值'，其中常数 HZ = 100。 本函数的主要功能是设置 alarm
// 字段和进行两种单位之间的转换。
int sys_alarm(long seconds)
{
	int old = current->alarm;

	if (old)
		old = (old - jiffies) / HZ;
	current->alarm = (seconds>0)?(jiffies+HZ*seconds):0;
	return (old);
}

// 取当前进程号 pid。
int sys_getpid(void)
{
	return current->pid;
}

// 取父进程号 ppid。
int sys_getppid(void)
{
	return current->p_pptr->pid;
}

// 取用户号 uid。
int sys_getuid(void)
{
	return current->uid;
}

// 取有效的用户号 euid
int sys_geteuid(void)
{
	return current->euid;
}

// 取组号 gid。
int sys_getgid(void)
{
	return current->gid;
}

// 取有效的组号 egid
int sys_getegid(void)
{
	return current->egid;
}

// 系统调用功能 -- 降低对 CPU 的使用优先权（有人会用吗？ ） 。
// 应该限制 increment 为大于 0 的值，否则可使优先权增大
int sys_nice(long increment)
{
	if (current->priority-increment>0)
		current->priority -= increment;
	return 0;
}

// 内核调度程序的初始化子程序
void sched_init(void)
{
	int i;
	struct desc_struct * p;					// 描述符表结构指针。

// Linux 系统开发之初，内核不成熟。内核代码会经常被修改。 Linus 怕自己无意中修改了这些
// 关键性的数据结构，造成与 POSIX 标准的不兼容。 这里加入下面这个判断语句并无必要，纯粹
// 是为了提醒自己以及其他修改内核代码的人。
	if (sizeof(struct sigaction) != 16)				// sigaction 是存放有关信号状态的结构。
		panic("Struct sigaction MUST be 16 bytes");
// 在全局描述符表 GDT 中设置初始任务（任务 0）的任务状态段 TSS 描述符和局部数据表 LDT 描述符。
// FIRST_TSS_ENTRY 和 FIRST_LDT_ENTRY 的值分别是 4 和 5，定义在 include/linux/sched.h 中； gdt
// 是一个描述符表数组（ include/linux/head.h ），实际上对应程序 head.s 中第 234 行上的全局描
// 述符表基址（ _gdt）。因此 gdt+FIRST_TSS_ENTRY 即为 gdt[FIRST_TSS_ENTRY]（即是 gdt[4]），
// 也即 gdt 数组第 4 项的地址。 参见 include/asm/system.h,第 65 行开始
	set_tss_desc(gdt+FIRST_TSS_ENTRY,&(init_task.task.tss));
	set_ldt_desc(gdt+FIRST_LDT_ENTRY,&(init_task.task.ldt));
// 清任务数组和描述符表项（注意从 i=1 开始，所以初始任务的描述符还在）。描述符项结构
// 定义在文件 include/linux/head.h 中。	
	p = gdt+2+FIRST_TSS_ENTRY;
	for(i=1;i<NR_TASKS;i++) {
		task[i] = NULL;
		p->a=p->b=0;
		p++;
		p->a=p->b=0;
		p++;
	}
/* Clear NT, so that we won't have troubles with that later on */
/* Clear NT, so that we won't have troubles with that later on */
/* 清除标志寄存器中的位 NT，这样以后就不会有麻烦 */
// EFLAGS 中的 NT 标志位用于控制任务的嵌套调用。当 NT 位置位时，那么当前中断任务执行
// IRET 指令时就会引起任务切换。 NT 指出 TSS 中的 back_link 字段是否有效。 NT=0 时无效。
	__asm__("pushfl ; andl $0xffffbfff,(%esp) ; popfl");
	ltr(0);					// 定义在 include/linux/sched.h 第 157-158 行。
	lldt(0);				// 其中参数（ 0）是任务号。
// 下面代码用于初始化 8253 定时器。通道 0，选择工作方式 3，二进制计数方式。通道 0 的
// 输出引脚接在中断控制主芯片的 IRQ0 上，它每 10 毫秒发出一个 IRQ0 请求。 LATCH 是初始
// 定时计数值。
	outb_p(0x36,0x43);		/* binary, mode 3, LSB/MSB, ch 0 */
	outb_p(LATCH & 0xff , 0x40);	/* LSB */	// 定时值低字节。
	outb(LATCH >> 8 , 0x40);	/* MSB */		// 定时值高字节。
// 设置定时器中断处理程序句柄（设置时钟中断门）。 修改中断控制器屏蔽码，允许时钟中断。
// 然后设置系统调用中断门。这两个设置中断描述符表 IDT 中描述符的宏定义在文件
// include/asm/system.h 中第 33、 39 行处。两者的区别参见 system.h 文件开始处的说明
	set_intr_gate(0x20,&timer_interrupt);
	outb(inb_p(0x21)&~0x01,0x21);				// 修改屏蔽码，允许定时器中断。
	set_system_gate(0x80,&system_call);
}

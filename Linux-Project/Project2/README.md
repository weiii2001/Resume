# Project2
> 第 7 組
> 112522099 許俊偉
> 112522115 林語潔
> 112526001 許仁覺

## Outline
[TOC]

## Enviroment
* VMware 16.2.4
* Ubuntu 16.04 , 64位元
* Kernel 4.15.1

## Compile Kernel
1. 下載 linux kernel 並解壓縮
    `tar -xvf ~/linux-4.15.1.tar.gz`
    `cd linux-4.15.1/`
	下載網址: https://cdn.kernel.org/pub/linux/kernel/v4.x/
2. 在目錄 linux-4.15.1/ 下
    `mkdir mycall`
    `cd mycall`
    
3. 新增一個檔案 my_set_priority.c，後面有system call 的程式
    
4. 在目錄 linux-4.15.1/mycall/ 下
    `gedit Makefile`
    `obj-y := my_set_priority.o`
    
5. 在目錄 linux-4.15.1/ 下修改檔案 Makefile 
    這是為了告訴它，新增的 system call 的 source files 是在 mycall 目錄下
    `gedit Makefile`
   ```
   core-y += kernel/ mm/ fs/ ipc/ security/ crypto/ block/
   找到這行，在最後面新增 mycall/

   core-y += kernel/ mm/ fs/ ipc/ security/ crypto/ block/ mycall/
   ```
6. 新增 system call 到 system call table 中
    `gedit arch/x86/entry/syscalls/syscall_64.tbl`
    ```
    在最後面新增一行
    333	common	my_set_priority		sys_my_set_priority
    ```
7. 修改 system call header
    `gedit include/linux/syscalls.h`
    
    ```
    在最下面 (#endif前) 新增一行
    asmlinkage long sys_my_set_priority(int number) ;

    這定義了我們 system call 的 prototype
    asmlinkage 代表我們的參數都可以在 stack 裡取用
    ```

8. 安裝編譯 kernel 所需的套件
	`sudo apt install build-essential libncurses-dev libssl-dev libelf-dev bison flex -y`

9. 設定檔
    `sudo make menuconfig`

10. 開始編譯
    `sudo make -j4`
    `sudo make modules_install install -j4`
    `sudo make install -j4`

11. 修改 grub
    `sudo gedit /etc/default/grub`
	修改成:
    ```
	GRUB_TIMEOUT_STYLE=menu
	GRUB_TIMEOUT=3
    ```
	更新 grub:
    `sudo grub-mkconfig -o /boot/grub/grub.cfg`
12. 重開機
    `reboot`
## Add Syscall
### 設定 my_fixed_priority
```c=
#include <linux/sched.h>
#include <asm/current.h>
#include <linux/printk.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>

SYSCALL_DEFINE1(my_set_priority, int, number){
        printk("SYSCALL333 | number: %ld\n", number);
	current -> my_fixed_priority = number ;
	if (current -> my_fixed_priority == number) {
		return 1;
	}
	return 0;
}
```
### 查看 static_prio
```c=
#include <linux/sched.h>
#include <asm/current.h>
#include <linux/printk.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>

SYSCALL_DEFINE0(check_static_prio){
	printk("SYSCALL334 | static_prio: %ld\n", current -> static_prio);
}
```
## Kernel Space 
1. **在 task_struct 新增欄位**
   > 路徑: include/linux/sched.h
   ![Linux-proj2-1](https://hackmd.io/_uploads/HJelJeHu6.jpg)

2. **在 copy_process() 初始化新增的欄位**
   > 路徑: kernel/fork.c
   ![Linux-proj2-2](https://hackmd.io/_uploads/S1rxJgH_6.jpg)

3. **在 __schedule() 中更改 static->prio**
   > 路徑: kernel/sched/core.c
   > ![sched](https://hackmd.io/_uploads/SJI1hbadT.jpg)
## User Space
```c=
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <syscall.h>

#define TOTAL_ITERATION_NUM  100000000
#define NUM_OF_PRIORITIES_TESTED  40
 
int main(){
	int index=0;
	int priority,i;
	struct timeval start[NUM_OF_PRIORITIES_TESTED], end[NUM_OF_PRIORITIES_TESTED];

	gettimeofday(&start[index], NULL);           //begin
	for(i=1;i<=TOTAL_ITERATION_NUM;i++)
	  rand();
	gettimeofday(&end[index], NULL);             //end 

	/*================================================================================*/

	for(index=1, priority=101;priority<=139;++priority,++index) {
	  if(!syscall(333, priority))
		printf("Cannot set priority %d.\n", priority);  
	  
	  gettimeofday(&start[index], NULL);  //begin
	  for(i=1;i<=TOTAL_ITERATION_NUM;i++)
		  rand();
		  
	  gettimeofday(&end[index], NULL);   //end     
	  syscall(334);                      // 查看 static_prio是否有成功修改
	}

	/*================================================================================*/

	printf("The process spent %ld uses to execute when priority is not adjusted.\n", ((end[0].tv_sec * 1000000 + end[0].tv_usec) - (start[0].tv_sec * 1000000 + start[0].tv_usec)));

	for(i=1;i<=NUM_OF_PRIORITIES_TESTED-1;i++)
	  printf("The process spent %ld uses to execute when priority is %d.\n", ((end[i].tv_sec * 1000000 + end[i].tv_usec) - (start[i].tv_sec * 1000000 + start[i].tv_usec)), i+100);
	  
	return 0;
}
```
* 使用 dmesg 查看
![image](https://hackmd.io/_uploads/rJq6hWTd6.png)

  > line1: SYSCALL333 設定 my_fixed_priority 為 130
  > line2: 在 core.c __schedule() 中，更改 static_prio 之前
  > line3: 在 core.c __schedule() 中，更改 static_prio 之後
  > line4: SYSCALL334 使用 system call 查看 static_prio 是否有成功修改
* 程式輸出
  ![image](https://hackmd.io/_uploads/S1Eh1xrua.png =80%x)
  
  > 為了防止新process佔用cpu時間，place_entity會對新process的vruntime進行處罰，故雖然改變的static_prior，但因為對新process權重改變的vruntime處罰，實際運行的時間並無隨prior有趨勢上的變化。
  
## 補充
### 四種Scheduling class及priority
透過Next 把各Scheduling classes 串起來
![image](https://hackmd.io/_uploads/r1kNWo8OT.png =60%x)


### task_struct 四種優先級成員變數
[/include/linux/sched.h](https://elixir.bootlin.com/linux/v4.15.1/source/include/linux/sched.h#L520)
在與task有關的task_struct 裡，因應不同的scheuling定義了四種優先級的成員變數
```c=
struct task_struct {
    ...
        int		prio;
	int		static_prio;
	int		normal_prio;
	unsigned int	rt_priority;
    ...
```
> prio
> 動態優先級，系統會針對此prio值去選擇scheduling class，為調度器真正使用的優先度
> 初始值由sched_fork()或由effective_prio()決定
```c=
static int effective_prio(struct task_struct *p)
{
	p->normal_prio = normal_prio(p);
	/*
	 * If we are RT tasks or we were boosted to RT priority,
	 * keep the priority unchanged. Otherwise, update priority
	 * to the normal priority:
	 */
	if (!rt_prio(p->prio))
		return p->normal_prio;
	return p->prio;
}
```

> static_prio
> 静態優先級，為process 被創造時sched_fork()產生
> 可以透過nice、renice或者setpriority()來做修改
> 用戶透過輸入nice到NICE_TO_PRIO修改static_prio值


>normal_prio
>是基於static_prio和調度策略計算出來的優先權，在建立process時會繼承父行程normal_prio。
>normal tasks：normal_prio = static_prio；
>real time task：normal_prio = 99 - rt_priority;
```c=
static inline int __normal_prio(struct task_struct *p)
{
	return p->static_prio;
}

static inline int normal_prio(struct task_struct *p)
{
	int prio;

	if (task_has_dl_policy(p))
		prio = MAX_DL_PRIO-1;     //Deadline: 固定為-1
	else if (task_has_rt_policy(p))
		prio = MAX_RT_PRIO-1 - p->rt_priority; //Real time: 固定為100-1-rt_priority
	else
		prio = __normal_prio(p);    //Normal: prior=static_prio
	return prio;
}
```

> rt_priority
> Realtime優先級，在被rl調度器調度的process中才會有意義, 範圍0-99

### Prior(Kernel)與Nice(User)轉換
> 主要透過 NICE_TO_PRIO 和 PRIO_TO_NICE 進行轉換

![image](https://hackmd.io/_uploads/B1CQHRIdp.png)
```c=
#define MAX_USER_RT_PRIO    100
#define DEFAULT_PRIO        (MAX_RT_PRIO + NICE_WIDTH / 2)

#define NICE_TO_PRIO(nice)    ((nice) + DEFAULT_PRIO)
#define PRIO_TO_NICE(prio)    ((prio) - DEFAULT_PRIO)
```

### 新建process 流程
從 do_fork-->copy_process 進行 process 調度的初始化
```c=
copy_process()
  sched_fork()
    __sched_fork()
    fair_sched_class->task_fork()->task_fork_fair()
      __set_task_cpu()
      update_curr()
      place_entity()
  wake_up_new_task()
    activate_task()
      enqueue_task
        fair_sched_class->enqueue_task-->enqueue_task_fair()
```
sched_fork()
>呼叫 __sched_fork() 對 task_struct 進行 default 值的初始化(ex: se.vruntime=0)
>將父 process 的 normal_prio 設目前 process 的 prio 
>透過 process 的 prio 分配 sched_class
```c=
int sched_fork(unsigned long clone_flags, struct task_struct *p)
{
	unsigned long flags;
	int cpu = get_cpu();

	__sched_fork(clone_flags, p);

	p->state = TASK_NEW;         /* TASK_NEW 表示新prcoess */

	p->prio = current->normal_prio; /* 將父process的normal_prio設子process 的prio */

	if (unlikely(p->sched_reset_on_fork)) {  /* 如果重置設定=True，重置policy、static_prio、prio、weight、inv_weight等。 */
		if (task_has_dl_policy(p) || task_has_rt_policy(p)) {
			p->policy = SCHED_NORMAL;
			p->static_prio = NICE_TO_PRIO(0);
			p->rt_priority = 0;
		} else if (PRIO_TO_NICE(p->static_prio) < 0)
			p->static_prio = NICE_TO_PRIO(0);
            
		p->prio = p->normal_prio = __normal_prio(p);
		set_load_weight(p, false);


		p->sched_reset_on_fork = 0;
	}

	if (dl_prio(p->prio)) {
		put_cpu();
		return -EAGAIN;
	} else if (rt_prio(p->prio)) {
		p->sched_class = &rt_sched_class;
	} else {
		p->sched_class = &fair_sched_class; /* 設定子process的調度器為CFS */
	}

	init_entity_runnable_average(&p->se);
    
	raw_spin_lock_irqsave(&p->pi_lock, flags);
        ...
        
        if (p->sched_class->task_fork)
		p->sched_class->task_fork(p); /*使用task_fork方法，CFS 對應task_fork_fair() */
        ...
}
```

task_fork_fair
> 更新 curr 資訊
> 設定目前 process 的 vuntime 為新 process vruntime
> 呼叫 place_entity 對 process vruntime 進行懲罰(因为新 process導致 CFS run queue 權重發生變化)
```c=
static void task_fork_fair(struct task_struct *p)
{
	struct cfs_rq *cfs_rq;
	struct sched_entity *se = &p->se, *curr;
	struct rq *rq = this_rq();
	struct rq_flags rf;

	rq_lock(rq, &rf);
	update_rq_clock(rq);

	cfs_rq = task_cfs_rq(current); /* 取得目前process所在cpu的cfs_rq */
	curr = cfs_rq->curr;
	if (curr) {
		update_curr(cfs_rq); /*更新目前调度實體cfs_rq->curr資訊 ex: 運行時間vruntime */

		se->vruntime = curr->vruntime; /*新process的vrumtime使用目前process的vruntime */
	}
	place_entity(cfs_rq, se, 1); /*對新process的vruntime 進行懲罰 */

	if (sysctl_sched_child_runs_first && curr && entity_before(curr, se)) {
		swap(curr->vruntime, se->vruntime);
		resched_curr(rq);
	}

	se->vruntime -= cfs_rq->min_vruntime;
	rq_unlock(rq, &rf);
}
```

### 計算vruntime
> CFS 中所謂的 fair 是 vruntime 的，而不是實際時間的平等。
> CFS 調度器拋棄先前固定時間片和固定調度週期的演算法，而採用 process 權重值的比重來量化和計算實際運行時間。
> 引入虛擬時鐘概念，每個 process 虛擬時間是實際運行時間相對 Nice 值為0的權重比例值。
> Nice 值小的 process，優先順序高且權重大，其虛擬時鐘比真實時鐘跑得慢，所以可以獲得更多的實際運行時間。
> 反之，Nice 值大的 process，優先權低權重小，獲得的實際運行時間也更少。

>CFS選擇 vruntime 跑得慢的進程(Nice 低)，而不是實際運行時間運行的少的進程。

>vruntime=delta_exec*nice_0_weight/weight
>vruntime表示進程的虛擬運行時間，delta_exec表示進程實際運行時間，nice_0_weight表示nice為0權重值，weight表示該進程的權重值，可以透過prio_to_weight[]取得。

### Process 調度流程
```c=
__schedule()
  ->pick_next_task()
    ->pick_next_task_fair()
  ->context_switch()
    ->switch_mm()
      ->cpu_v7_switch_mm()
    ->switch_to()
      ->__switch_to
```
__schedule
> 呼叫pick_next_task()讓process調度器從run_queue中選擇一個最適合的next process，然後context_switch()切換到next process運行。

```c=
static void __sched notrace __schedule(bool preempt)
{
	struct task_struct *prev, *next;
	unsigned long *switch_count;
	struct rq_flags rf;
	struct rq *rq;
	int cpu;

	cpu = smp_processor_id();
	rq = cpu_rq(cpu);
	prev = rq->curr;

	...
    
    /* 呼叫pick_next_task_fair()從rq上選擇適當的process傳回next。*/
	next = pick_next_task(rq, prev, &rf); 
	clear_tsk_need_resched(prev);
	clear_preempt_need_resched();

    /* 如果目前process和next不同，那麼呼叫context_switch()進行上下文切換。 */
	if (likely(prev != next)) {
		rq->nr_switches++;
		rq->curr = next;

		++*switch_count;

		trace_sched_switch(preempt, prev, next);

		/* Also unlocks the rq: */
		rq = context_switch(rq, prev, next, &rf);
	} else {
		rq->clock_update_flags &= ~(RQCF_ACT_SKIP|RQCF_REQ_SKIP);
		rq_unlock_irq(rq, &rf);
	}

	balance_callback(rq);
}
```


## Problem
1. 編譯時缺少 gelf.h 
   ![image](https://hackmd.io/_uploads/rJmPnbau6.png)
   * solution: `sudo apt-get install libelf-dev`
   * reference: [Ubuntu编译出现：gelf.h: No such file or directory](https://blog.csdn.net/weixin_44260459/article/details/123035958)

2. 在寫copy_process時放錯位置
    ![image](https://hackmd.io/_uploads/ryg_mF8_6.png)
    * solution: 放在最後一行
    ![Linux-proj2-2](https://hackmd.io/_uploads/S1rxJgH_6.jpg)

## Reference
1. [Source code: /include/linux/sched.h](https://elixir.bootlin.com/linux/v4.15.1/source/include/linux/sched.h#L520)
2. [Source code: /kernel/fork.c](https://elixir.bootlin.com/linux/v4.15.1/source/kernel/fork.c#L1534)
3. [Source code: /kernel/sched/core.c](https://elixir.bootlin.com/linux/v4.15.1/source/kernel/sched/core.c)
4. [四種任務優先度 (prio、static_prio、rt_priority 與 normal_prio) 之間的差異](https://hackmd.io/@kurokuo/r1naFPjRV)
5. [__schedule __setscheduler_params __sched_setscheduler](https://hackmd.io/@LJP/rJVFJsi1E#__setscheduler)
6. [sched_fork过程分析](https://blog.csdn.net/lizhijun_buaa/article/details/129346719)
7. [Linux进程管理 (2)CFS调度器](https://www.cnblogs.com/arnoldlu/p/8465034.html)
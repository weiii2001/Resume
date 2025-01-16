# Linux Project1
> 第 7 組
> 112522099 許俊偉
> 112522115 林語潔
> 112526001 許仁覺

## Introduction
[題目說明](https://staff.csie.ncu.edu.tw/hsufh/COURSES/FALL2023/linux_project_1.html)  
Write a new system call void * my_get_physical_addresses(void *) so that a process can use it to get the physical address of a virtual address of a process.

## Enviroment
* VMware 16.2.4
* Ubuntu 16.04 , 64位元
* Kernel 4.15.1
## Compile Kernel
1. 下載 linux kernel 並解壓縮  
    `tar -xvf ~/linux-4.15.1.tar.gz`  
    `cd linux-4.15.1/`
    
	[下載網址](https://cdn.kernel.org/pub/linux/kernel/v4.x/)
3. 在目錄 linux-4.15.1/ 下  
    `mkdir mycall`  
    `cd mycall`
    
4. 新增一個檔案 virtophy.c，後面有system call 的程式  
    
5. 在目錄 linux-4.15.1/mycall/ 下  
    `gedit Makefile`  
    `obj-y := virtophy.o`
    
6. 在目錄 linux-4.15.1/ 下修改檔案 Makefile 
    這是為了告訴它，新增的 system call 的 source files 是在 mycall 目錄下  
    `gedit Makefile`  
   ```
   core-y += kernel/ mm/ fs/ ipc/ security/ crypto/ block/
   找到這行，在最後面新增 mycall/

   core-y += kernel/ mm/ fs/ ipc/ security/ crypto/ block/ mycall/
   ```
7. 新增 system call 到 system call table 中  
    `gedit arch/x86/entry/syscalls/syscall_64.tbl`  
    ```
    在最後面新增一行
    333	common	virtophy		sys_get_physical_address
    ```
8. 修改 system call header  
    `gedit include/linux/syscalls.h`
    
    ```
    在最下面 (#endif前) 新增一行
    asmlinkage long sys_get_physical_address(unsigned long* initial, unsigned long * result, int n) ;

    這定義了我們 system call 的 prototype
    asmlinkage 代表我們的參數都可以在 stack 裡取用
    ```

9. 安裝編譯 kernel 所需的套件  
    `sudo apt-get install libncurses5-dev libssl-dev`  
	`sudo apt install build-essential libncurses-dev libssl-dev libelf-dev bison flex -y`


10. 設定檔  
    `sudo make menuconfig`

11. 開始編譯  
    `sudo make -j2`  
    `sudo make modules_install install -j2`  
    `sudo make install -j2`

12. 修改 grub  
    `sudo update-grub`  
13. 重開機  
    `reboot`
## System Call
### Linux page table layout
![image](https://hackmd.io/_uploads/rywRrPs46.png)

>
>64-bits電腦在Linear address可依序拆分為pgd、p4d、pud、pmd、pte table，  
>但實際上有用到的僅pgd、pud、pmd、pte table，而p4d僅單純傳遞pgd的值，  
>每個table裡，各entry型態為pgd_t、p4d_t、pud_t、pmd_t、pte_t。

>pgd_offset可以透過mm_struct和virtual address，向PGD裡的對應index entry，而entry裡內容  
>會再指向下一層p4d table起始位置。  
>p4d_offset找到p4d中對應的index entry，entry裡內容會再指向下一層pud table起始位置。  
>pud_offset找到pud中對應的index entry，entry裡內容會再指向下一層pmd table起始位置。  
>pmd_offset找到pmd中對應的index entry，entry裡內容會再指向下一層pte table起始位置。  
>pte_offset找到pte中對應的index entry，entry裡內容是pte base address。

>page address透過pte base address和PASK_MASK取得，  
>page offset透過virtual address和~PASK_MASK取得，  
>最終page address 結合 page offset 組成 physical address。


### kernel space code
```c=
#include <linux/syscalls.h>
#include <linux/printk.h>
#include <linux/mm_types.h>
#include <linux/slab.h>

SYSCALL_DEFINE3(get_physical_address, unsigned long*, initial, unsigned long*, result, int, n)
{
        pgd_t* pgd;
        pud_t* pud;
        pmd_t* pmd;
        pte_t* pte;

        unsigned long page_addr = 0;
        unsigned long page_offset = 0;
        unsigned long* va;
        unsigned long* pa;
        int i;
        long ret = 0;
        va = (unsigned long*)kmalloc(sizeof(unsigned long)*n,GFP_KERNEL);
        pa = (unsigned long*)kmalloc(sizeof(unsigned long)*n,GFP_KERNEL);
        ret = copy_from_user(va, initial, sizeof(*va)*n);
        for (i = 0; i < n; i++)
        {
                pgd = pgd_offset(current->mm, va[i]);
                    printk("--------i = %lu----------\n", i);
                    printk("pgd_val = 0x%lx\n", pgd_val(*pgd));
		     printk("pgd_index = %lu\n", pgd_index(va[i]));
		     if (pgd_none(*pgd)) {
				        printk("Not mapped in pgd!\n");
					return 0;
				}
				
                pud = pud_offset((p4d_t*)pgd, va[i]);
                    printk("pud_val = 0x%lx\n", pud_val(*pud));
                    printk("pud_index = %lu\n", pud_index(va[i]));
		     if (pud_none(*pud)) {
					printk("Not mapped in pud!\n");
					return 0;
				}
                pmd = pmd_offset(pud, va[i]);
                     printk("pmd_val = 0x%lx\n", pmd_val(*pmd));
		      printk("pmd_index = %lu\n", pmd_index(va[i]));
	              if (pmd_none(*pmd)) {
				        printk("Not mapped in pmd!\n");
					return 0;
				}
                
                pte = pte_offset_kernel(pmd, va[i]);
                      printk("pte_val = 0x%lx\n", pte_val(*pte));
		       printk("pte_index = %lu\n", pte_index(va[i]));
		       if (pte_none(*pte)) {
					printk("Not mapped in pte!\n");
					return 0;
				}

                page_addr = pte_val(*pte) & PAGE_MASK;
                page_offset = va[i] & ~PAGE_MASK;
                pa[i] = page_addr | page_offset;
                
                printk("-----------physical address:i=%lu--------------\n", i);
		 printk("pte_val = 0x%lx, PAGE_MASK = 0x%lx, page_addr = 0x%lx\n", pte_val(*pte), PAGE_MASK, page_addr);
		 printk("va = 0x%lx, ~PAGE_MASK = 0x%lx, page_offset = 0x%lx\n", va[i], ~PAGE_MASK, page_offset);
		 printk("page_addr = 0x%lx, page_offset = 0x%lx, physical_address = 0x%lx\n", page_addr, page_offset, pa[i]);
		 printk("------------------------------\n");
        }
        ret = copy_to_user(result, pa, sizeof(*pa)*n);
        kfree(va);
        kfree(pa);
        return ret;
}
```

### 程式碼解釋
```
SYSCALL_DEFINE3(get_physical_address, unsigned long*, initial, unsigned long*, result, int, n)
```
> SYSCALL_DEFINE3: 輸入變數是3，故是DEFINE3  
> get_physical_address: function, 但此處沒用到    
> unsigned long*, initial: virtual address  
> unsigned long*, result: physical address  
> int, n: 傳入陣列的大小


```
va = (unsigned long*)kmalloc(sizeof(unsigned long)*n,GFP_KERNEL);
pa = (unsigned long*)kmalloc(sizeof(unsigned long)*n,GFP_KERNEL);
```
> The kmalloc() function is a simple interface for obtaining kernel memory in byte-sized chunks.The function returns a pointer to a region of memory that is at least size bytes in length. The region of memory allocated is physically contiguous.  
> GFP_KERNEL is normal allocation flag.  

```
ret = copy_from_user(va, initial, sizeof(*va)*n);
ret = copy_to_user(result, pa, sizeof(*pa)*n);
```
[/include/linux/uaccess.h](https://https://elixir.bootlin.com/linux/v4.15.10/source/include/linux/uaccess.h#L144)  
![image](https://hackmd.io/_uploads/rJnh6w1Hp.png)  
> 可以看出來copy_from_user是從user space copy virutal address到kernel space，  
> 而copy_to_user是從kernel space copy physical address到user space。

```
kfree(va);
kfree(pa);
```
> 釋放kmalloc所分配的memory，需確保kmalloc有成功分配才可使用。

```
pgd = pgd_offset(current->mm, va[i]);
```


```
/* PAGE_SHIFT determines the page size */
#define PAGE_SHIFT	13 

/* Entries per page directory level */
#define PTRS_PER_PGD	(1UL << (PAGE_SHIFT-3))

/* PGDIR_SHIFT determines what a third-level page table entry can map */
#define PGDIR_SHIFT	(PAGE_SHIFT + 2*(PAGE_SHIFT-3))

/* to find an entry in a page-table-directory. */
#define pgd_index(address)	(((address) >> PGDIR_SHIFT) & (PTRS_PER_PGD-1))
#define pgd_offset(mm, address)	((mm)->pgd+pgd_index(address))

```
> PAGE_SHIFT: Page size, 2**13=8K bytes  
> PTRS_PER_PGD: PGD中的entry數量，1左移10位(13-3)個  
> pgd_index: 輸入virtual address 先找到PGD base address，再找到是哪個index的entry  
> 最後pgd_offset得到對應index的entry， entry內容會指向下一層




## User Space Code
``` c=
#include <syscall.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <stdlib.h>

struct data_ {
  int  id ;
  char name[16] ;
} ;
typedef struct data_ sdata ;
static __thread  sdata  tx ;  //thread local variable
int initGlobal = 123 ;
int notInitGlobal ;

long ret = 0;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

// ----------------------------------------------------------------------
void * ourFunc() {
  pthread_mutex_lock( &mutex1 );
  int localVar = 30;
  int * heapAddress = malloc(sizeof(int));
  
  size_t* va[7] = {
    (size_t*)&ourFunc,
	(size_t*)&initGlobal,
	(size_t*)&notInitGlobal,
	(size_t*)heapAddress,
	(size_t*)printf,
	(size_t*)&localVar,
	(size_t*)&tx
  };
  
  size_t* pa[7];
  ret = syscall(333, va, pa, 7);
  printf("\nPid of the Thread is = %p, \nPid of the process is = %d\nAddresses info:", (size_t*)pthread_self(), getpid());
  char printArray [7][20] = {"Code", "Data", "BSS", "Heap Seg", "Library", "Stack", "Thread"};
  for(int i=0;i<7;i++){
    printf("\n %d) %8s (va/pa) = %16p / %20p", i+1, printArray[i], va[i], pa[i]);
  } // for
  
  printf("\n\n");
  pthread_mutex_unlock( &mutex1 );
  pthread_exit(NULL);	

} // ourFunc()

int main(){
  int localVar = 30;
  int* heapAddress = malloc(sizeof(int));
  char test = '\n';
  printf("start! %c\n", test);
  
  size_t* va[7] = {
    (size_t*)&ourFunc,
	(size_t*)&initGlobal,
	(size_t*)&notInitGlobal,
	(size_t*)heapAddress,
	(size_t*)printf,
	(size_t*)&localVar,
	(size_t*)&tx
  };
  
  size_t* pa[7];
  ret = syscall(333, va, pa, 7);
  printf("Main thread\n");
  printf("\nPid of the Thread is = %p, \nPid of the process is = %d\nAddresses info:", (size_t*)pthread_self(), getpid());
  char printArray [7][20] = {"Code", "Data", "BSS", "Heap Seg", "Library", "Stack", "Thread"};
  for(int i=0;i<7;i++){
    printf("\n %d) %8s (va/pa) = %16p / %20p", i+1, printArray[i], va[i], pa[i]);
  } // for

  printf("\n\n");
  pthread_t threads[2];
  for (int i = 0; i < 2; i++) {
    pthread_create(&threads[i], NULL, ourFunc, NULL);
  } // for
  
  sleep(2);
  printf("\n");
  for(int i = 0; i < 3; i++){
    long size = (long)&threads[i+1]-(long)&threads[i];
    printf("threads[%d]:\nsize:%d\nstart address:%p\nend address:%p\n\n", i, (int)size, &threads[i], (size_t*)((long)&threads[i]+size-1));
  } // for

  for (int i = 0; i < 2; i++) {
    pthread_join(threads[i], NULL);
  }

  if(ret > 0) printf("error!!!QQQQQQQ");

  return 0;
}
```
* **程式設計流程**
  >1. 使用 pthread_create 定義 3 個 threads  
  >2. 使用 pthread_mutex_t 避免 thread 之間的衝突  
  >3. 宣告 7 個變數，分別儲存在記憶體不同位置  
  >4. 將 7 個變數透過一個陣列餵給 system call，並產生 physical address  
  >5. 輸出轉址結果
  >6. 輸出 3 個 threads 的 start address、end address、size
  >7. 最後，使用 pthread_join 等待 thread 執行結束
* **mutex 的用法**
  > 在 multi-thread 下使用 mutex 避免衝突  
  > 定義以及初始化 mutex `pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;`  
  > 將 mutex 設為鎖定狀態 `pthread_mutex_lock( &mutex1 );`  
  > 將 mutex 設為解除鎖定狀態 `pthread_mutex_unlock( &mutex1 );`  
* **使用資料型態 size_t\* 存放 address**  
  > size_t* 是指向 size_t 的指標型別，用於存儲 size_t 變數的地址  
  > 如果系統是 32 位元，size_t 是 unsigned int  
  > 如果系統是 64 位元，size_t 是 unsigned long 型別  
  > size_t 的定義在: `/usr/include/linux/types.h`  
  > 
    ```C=
    #define _SIZE_T
    typedef __kernel_size_t     size_t;
    ```

## 執行結果
![multi-thread output](https://hackmd.io/_uploads/BkA63rRNa.png)
### 變數的記憶體位置
>1. Code segment  
    ourFunc 在Main thread, Thread1, Thread2 指向相同 physical address(0x1900e0a36)  
    => 共用 Code segment
>2. Data segment  
    變數 initGlobal 在 Main thread, Thread1, Thread2 中皆指向相同 physical address(0x80000001884e40a0)  
    => 共用 Data segment
>3. BSS segment  
    變數 notInitGlobal 在 Main thread, Thread1, Thread2 中皆指向相同 physical address(0x80000001884e4128)  
    => 共用 BSS segment
>4. Library  
    printf 在 Main thread, Thread1, Thread2 中皆指向相同 physical address(0x1900e0890)  
    => 共用 Library
>5. Stack segment  
    localVal 在Main thread, Thread1, Thread2 指向 Stack 的physical address皆不同(分別為0x80000001868e6e9c, 0x80000001c3278dd0, 0x8000000186f48dd0)，表示各自建立自己的stack  
    => 不共用 Stack segment
>6. Thread local storage  
     Thread local 變數:tx 在Main thread, Thread1, Thread2 指向 Thread local storage 的physical address皆不同(分別為0x80000001d052572c, 0x8000000191df56ec, 0x800000018c61f6ec)，表示各自建立自己的Thread local storage  
    => 不共用 Thread local storage
    
### Heap segment
> 雖然 Main thread, Thread1, Thread2 所指向 heap 的 virtual address 和 physical address 皆不同，代表三個 thread 各自 malloc() 不同的記憶體位址，不過 physical address 其實是指到相同的 heap段

## Result
![multi-thread memory layout](https://hackmd.io/_uploads/Skm6On2N6.png)


## 遇到問題
1.VMWare 17, Ubuntu無法正常開機  
> 解決：安裝 VMWare 16版


2.Ubuntu和Kernel版相容問題  
問題：加system call並compile, 重開會黑屏無法使用
> 嘗試過版本：(有問題版本)  
> a.Ubuntu 14.03 + Kernel 3.19 32位元  
> b.Ubuntu 16.04 + Kernel 3.10 32位元

> 解決：  
> 先看下載後的Ubuntu Kernel 版本是什，找比正常版稍微新的  
> Ubuntu 16.04 + Kernel 4.15.1 64位元



## Reference
1. [Adding A System Call To The Linux Kernel (5.8.1) In Ubuntu (20.04 LTS)](https://dev.to/jasper/adding-a-system-call-to-the-linux-kernel-5-8-1-in-ubuntu-20-04-lts-2ga8)
2. [Chapter 3  Page Table Management](https://www.kernel.org/doc/gorman/html/understand/understand006.html)
3. [kmalloc&kfree](https://litux.nl/mirror/kerneldevelopment/0672327201/ch11lev1sec4.html)
4. [使用互斥旗標](https://www.ibm.com/docs/zh-tw/aix/7.3?topic=programming-using-mutexes)
5. [Linux Kernel Souce Code - size_t](https://docs.huihoo.com/doxygen/linux/kernel/3.7/include_2linux_2types_8h_source.html#l00053)
6. [Linux 数据类型： size_t 与 ssize_t 的理解](https://blog.51cto.com/zhangsz0516/6103238)

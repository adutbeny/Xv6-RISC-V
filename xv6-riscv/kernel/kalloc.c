/* Physical memory allocator, for user processes, */
/* kernel stacks, page-table pages, */
/* and pipe buffers. Allocates whole 4096-byte pages. */

/*Adut Beny
  oae862
  11349460

  Mustafa Saqib
  xvp605
  11330309
*/


#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"

void freerange(void *pa_start, void *pa_end);

extern char end[]; /* first address after kernel. */
                   /* defined by kernel.ld. */

struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  struct run *freelist;
} kmem;


/* CMPT 332 GROUP 48 Change, Fall 2024 */
struct {
  struct spinlock refLock;
} refCount;

int maxPages[PHYSTOP/PGSIZE];

void
kinit()
{
  initlock(&kmem.lock, "kmem");
  initlock(&refCount.refLock, "refCount");
  memset(maxPages, 0, sizeof(maxPages));
  freerange(end, (void*)PHYSTOP);
}

void
freerange(void *pa_start, void *pa_end)
{
  char *p;
  p = (char*)PGROUNDUP((uint64)pa_start);
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE)
    kfree(p);
}

/* Free the page of physical memory pointed at by pa, */
/* which normally should have been returned by a */
/* call to kalloc().  (The exception is when */
/* initializing the allocator; see kinit above.) */
void
kfree(void *pa)
{
  struct run *r;

  if(((uint64)pa % PGSIZE) != 0 || (char*)pa < end || (uint64)pa >= PHYSTOP)
    panic("kfree");

  /* Fill with junk to catch dangling refs. */
  decreaseRefCount(pa);
  if (maxPages[(uint64)pa/PGSIZE] <= 0) {
    
    memset(pa, 1, PGSIZE);

    r = (struct run*)pa;

    acquire(&kmem.lock);
    r->next = kmem.freelist;
    kmem.freelist = r;
    release(&kmem.lock);
  }
}

/* Allocate one 4096-byte page of physical memory. */
/* Returns a pointer that the kernel can use. */
/* Returns 0 if the memory cannot be allocated. */
void *
kalloc(void)
{
  struct run *r;

  acquire(&kmem.lock);
  r = kmem.freelist;
  /* CMPT 332 GROUP 48 Change, Fall 2024 */
  if(r) {
    uint64 index = (uint64) r / PGSIZE;
    kmem.freelist = r->next;
    if (maxPages[index] < 1)
      maxPages[index] = 1;
  }
  release(&kmem.lock);

  if(r){ 
    memset((char*)r, 5, PGSIZE); /* fill with junk */
  }
  return (void*)r;
}

/* CMPT 332 GROUP 48 Change, Fall 2024 */
/* Returns the number of free pages */
int
freePages(void)
{
  int count = 0;
  struct run *r;
  acquire(&kmem.lock);
  r = kmem.freelist;
  while (r){
    count++;
    r = r->next;
  }
  release(&kmem.lock);
  return count;
}

/* Increases the reference count of a page */
void 
increaseRefCount(void* pa)
{
  acquire(&refCount.refLock);
  maxPages[(uint64)pa/PGSIZE]++;
  release(&refCount.refLock);   

}

/* Decreases the reference count of a page */
void
decreaseRefCount(void* pa)
{
  acquire(&refCount.refLock);
  maxPages[(uint64)pa/PGSIZE]--;
  release(&refCount.refLock);
  
}

/* Gets number of reference count of page */
uint64 
getRefCount(void* pa)
{
   uint64 index = (uint64) pa /PGSIZE;
   acquire(&refCount.refLock);
   uint64 count = maxPages[index];
   release(&refCount.refLock);
   return count;
}



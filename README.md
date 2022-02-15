Implement several UNIX programs for xv6

## #1 sleep

### 1. Add $U/sleep to UPROGS in Makefile

For using user program commands, add `$/command` in Makefile.
```
UPROGS=\
  $U/_sleep\
  ...
```

### 2. Implement `sleep` in `user/sleep.c`

> The command-line argument is passed as a string; you can convert it to an integer using atoi (see user/ulib.c).

`atoi` is simple in xv6:

```c
int
atoi(const char *s)
{
  int n;

  n = 0;
  while('0' <= *s && *s <= '9')
    n = n*10 + *s++ - '0';
  return n;
}
```

When user programs calling system call `sleep`, it refers to `sys_sleep` in `kernel/sysproc.c`.
> A tick is a notion of time defined by the xv6 kernel, namely the time between two interrupts from the timer chip.

```c
uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}
```

Use `atoi` and `sleep` to write this user programs in `user/sleep.c`.

```c
#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{

  if(argc < 2){
    // stdin, stdout, stderr = 0, 1, 2
    fprintf(2, "Usage: sleep duration...\n");
    exit(1);
  }
  sleep(atoi(argv[1]));
  exit(0);
}
```

## #2 pingpong
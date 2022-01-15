#include "kernel/types.h"
#include "user/user.h"

void redirect(int k, int p[]) {
  close(k);
  dup(p[k]);
  close(p[0]);
  close(p[1]);
}
// main:2->ps1:print(2)
void makeChain() {
  int p[2];
  int i;

  // 先從fd0讀取上個來源傳來的數字
  // 因為fork()來的child會複製parent的fd table
  if (read(0, &i, sizeof(i))) {
    // 傳來的值經過前面數字的filter必定是prime
    printf("prime %d\n", i);
    pipe(p);
    if (fork()==0) { // child
      redirect(0, p);
      // go make child recursively
      makeChain();
      
    } else { // parent
      // filter and write to p[1]
      redirect(1, p);
      int j;
      while (read(0, &j, sizeof(j))) {
        if ( j%i != 0) {
          write(1, &j, sizeof(j));
        }
      }
      close(1);
      wait(0);
      exit(0);
    }
  }
  exit(0);
}

int main(int argc, char *argv[]) {
  if(argc > 1){
    fprintf(2, "primes command use no arguments\n");
    exit(1);
  }

  // init a pipe p
  int p[2];
  pipe(p); 

  if (fork()==0) { // child
    redirect(0, p);
    makeChain();
  } else {
    redirect(1, p);
    for (int i = 2; i < 36; i++) {
      write(1, &i, sizeof(i));
    }
    close(1);
    wait(0);
    exit(0);
  }
  return 0;
}
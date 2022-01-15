#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{

    char c;
    int n;
    if (argc != 1) {
        // stdin, stdout, stderr = 0, 1, 2
        fprintf(2, "pingpong use no args\n");
        exit(1);
    }

    // when creating a pipe, read/write file descriptors are put in p[0] and p[1]
    int parent_to_child[2];
    int child_to_parent[2];

    pipe(parent_to_child);
    pipe(child_to_parent);

    if (fork() == 0) { // child process
        close(parent_to_child[1]);
        close(child_to_parent[0]);
        n = read(parent_to_child[0], &c, 1);
        if (n != 1) {
            fprintf(2, "read from parent pipe error\n");
            exit(1);
        }
        printf("%d: received ping\n", getpid());
        write(child_to_parent[1], &c, 1);
        
        close(parent_to_child[0]);
        close(child_to_parent[1]);
        exit(0);
    } else { // parent process
        close(parent_to_child[0]);
        close(child_to_parent[1]);
        write(parent_to_child[1], &c, 1);
        n = read(child_to_parent[0], &c, 1);
        if (n != 1) {
            fprintf(2, "read from child pipe error\n");
            exit(1);
        }
        printf("%d: received ping\n", getpid());
        
        
        close(child_to_parent[0]);
        close(parent_to_child[1]);
        wait(0);
        exit(0);
    }
}
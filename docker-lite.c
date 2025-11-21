#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <sys/mount.h>
#include <sys/utsname.h>

#define STACK_SIZE (1024 * 1024)
static char container_stack[STACK_SIZE];

static int container_main(void *arg) {
    char **argv = (char **)arg;
    char *container_root = argv[1];
    char *program = argv[2];
    
    printf("Child: Inside container...\n");
    printf("Child: Container root: %s\n", container_root);
    printf("Child: Running program: %s\n", program);

    // Set hostname for UTS namespace
    if (sethostname("docker-lite", 11) != 0) {
        perror("sethostname failed");
    }

    // Change to container directory FIRST
    if (chdir(container_root) != 0) {
        perror("chdir failed");
        printf("Child: Current directory: ");
        system("pwd");
    }

    // Mount /proc for PID namespace to work properly
    if (mount("proc", "/proc", "proc", 0, NULL) != 0) {
        perror("mount proc failed");
    }

    // Set display for graphics
    setenv("DISPLAY", ":0", 1);

    // Count arguments
    int arg_count = 0;
    for (int i = 2; argv[i] != NULL && i < 10; i++) {
        arg_count++;
        printf("Child: Arg %d: %s\n", i-1, argv[i]);
    }


    printf("Child: Executing program...\n");

    // Execute the program
    if (arg_count > 1) {
        // We have multiple arguments
        char **program_args = &argv[2];
        execvp(program_args[0], program_args);
    } else {
        // Single argument
        execlp(program, program, NULL);
    }

    perror("exec failed");
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <container-dir> <program> [args...]\n", argv[0]);
        printf("Example: %s ./my-container-root python3 app/ChessMain.py\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    printf("Parent: Starting container...\n");
    printf("Parent: Container dir: %s\n", argv[1]);
    printf("Parent: Program: %s\n", argv[2]);

    // Check if container directory exists
    if (access(argv[1], F_OK) != 0) {
        printf("Parent: ERROR: Container directory %s does not exist!\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    // Flags for all Linux namespaces
    int flags = CLONE_NEWNS |    // Mount namespace (filesystem isolation)
                CLONE_NEWUTS |   // UTS namespace (hostname isolation)
                CLONE_NEWIPC |   // IPC namespace (Inter-process communication)
                CLONE_NEWPID |   // PID namespace (process ID isolation)
                CLONE_NEWNET |   // Network namespace (network isolation)
                CLONE_NEWUSER |  // User namespace (user ID isolation)
                CLONE_NEWCGROUP; // Cgroup namespace (resource control isolation)

    pid_t pid = clone(container_main, container_stack + STACK_SIZE, 
                     flags | SIGCHLD, argv);
    
    if (pid < 0) {
        perror("clone");
        printf("Parent: Clone failed. Trying with fork as fallback...\n");
        
        // Fallback to fork if clone fails (especially for user namespace)
        pid = fork();
        if (pid == 0) {
            container_main(argv);
            exit(1);
        } else if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
    }

    printf("Parent: Container process created with PID: %d\n", pid);

    int status;
    waitpid(pid, &status, 0);
    
    if (WIFEXITED(status)) {
        printf("Parent: Container stopped with exit code: %d\n", WEXITSTATUS(status));
    } else {
        printf("Parent: Container stopped.\n");
    }

    return 0;
}
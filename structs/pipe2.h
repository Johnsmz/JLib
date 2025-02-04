#ifndef _IXIMIUZ_POPEN2
#define _IXIMIUZ_POPEN2

#include<stdio.h>
#include<errno.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#define CLEANUP_PIPE(pipe) close((pipe)[0]); close((pipe)[1])

struct pipe2 {
    FILE *in;
    FILE *out;
};

typedef struct pipe2 pipe2;

struct __IXIMIUZ_files_chain_t {
    pipe2 files;
    pid_t pid;
    struct __IXIMIUZ_files_chain_t *next;
};
typedef struct __IXIMIUZ_files_chain_t __IXIMIUZ_files_chain_t;

static __IXIMIUZ_files_chain_t *files_chain;

static int _do_popen2(__IXIMIUZ_files_chain_t *link, const char *command){
    int child_in[2];
    int child_out[2];
    if (0 != pipe(child_in)) {
        return -1;
    }
    if (0 != pipe(child_out)) {
        CLEANUP_PIPE(child_in);
        return -1;
    }

    pid_t cpid = link->pid = fork();
    if (0 > cpid) {
       CLEANUP_PIPE(child_in);
       CLEANUP_PIPE(child_out);
       return -1;
    }
    if (0 == cpid) {
        if (0 > dup2(child_in[0], 0) || 0 > dup2(child_out[1], 1)) {
            _Exit(127);
        }
        CLEANUP_PIPE(child_in);
        CLEANUP_PIPE(child_out);

        for (__IXIMIUZ_files_chain_t *p = files_chain; p; p = p->next) {
            int fd_in = fileno(p->files.in);
            if (fd_in != 0) {
                close(fd_in);
            }
            int fd_out = fileno(p->files.out);
            if (fd_out != 1) {
                close(fd_out);
            }
        }

        execl("/bin/sh", "sh", "-c", command, (char *) NULL);
        _Exit(127);
    }

    close(child_in[0]);
    close(child_out[1]);
    link->files.in = fdopen(child_in[1], "w");
    link->files.out = fdopen(child_out[0], "r");
    return 0;
}

/**
 * NAME
 *     popen2 -- bidirectional popen()
 *
 * DESCRIPTION
 *     popen2(const char *command) opens two pipes, forks a child process,
 *     then binds the pipes to its stdin and stdout and execve shell to
 *     execute given command.
 *
 * RETURN VALUES:
 *     On success it returns a pointer to the struct with two fields
 *     { FILE *in; FILE *out; }. The struct should be released via pclose2()
 *     call. On failure returns NULL, check errno for more informaion about
 *     the error.
 */
pipe2 *popen2(const char *command) {
    __IXIMIUZ_files_chain_t *link = (__IXIMIUZ_files_chain_t *) malloc(sizeof (__IXIMIUZ_files_chain_t));
    if (NULL == link) {
        return NULL;
    }

    if (0 > _do_popen2(link, command)) {
        free(link);
        return NULL;
    }

    link->next = files_chain;
    files_chain = link;
    return (pipe2 *) link;
}

int pclose2(pipe2 *fp) {
    __IXIMIUZ_files_chain_t **p = &files_chain;
    int found = 0;
    while (*p) { if (*p == (__IXIMIUZ_files_chain_t *) fp) { *p = (*p)->next; found = 1; break; } p = &(*p)->next; }
    if (!found) { return -1; }
    if (0 > fclose(fp->in) || 0 > fclose(fp->out)) { free((__IXIMIUZ_files_chain_t *) fp); return -1; }
    int status = -1;
    pid_t wait_pid;
    do { wait_pid = waitpid(((__IXIMIUZ_files_chain_t *) fp)->pid, &status, 0); } while (-1 == wait_pid && EINTR == errno);
    free((__IXIMIUZ_files_chain_t *) fp);
    if (wait_pid == -1) { return -1; }
    return status;
}


#endif  // _IXIMIUZ_POPEN2v

#define _POSIX_C_SOURCE 200809L
#include "pipeline.h"

#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "exec.h"

struct pipe_pair
{
    int fds[2];
};

/* Create pipes */
static int create_pipes(struct pipe_pair *pipes, int count)
{
    for (int i = 0; i < count - 1; i++)
    {
        if (pipe(pipes[i].fds) == -1)
            return -1;
    }
    return 0;
}

/* Helper for close all pipes */
static void close_all_pipes(struct pipe_pair *pipes, int count)
{
    for (int i = 0; i < count - 1; i++)
    {
        close(pipes[i].fds[0]);
        close(pipes[i].fds[1]);
    }
}

/* Helper for setup child pipes */
static void setup_child_pipes(struct pipe_pair *pipes, int i, int count)
{
    if (i > 0)
        dup2(pipes[i - 1].fds[0], STDIN_FILENO);
    if (i < count - 1)
        dup2(pipes[i].fds[1], STDOUT_FILENO);
    close_all_pipes(pipes, count);
}

/* Helper for wait for children */
static int wait_for_children(int *pids, int count)
{
    int last_status = 0;
    for (int i = 0; i < count; i++)
    {
        int status;
        waitpid(pids[i], &status, 0);
        if (i == count - 1)
            last_status = WIFEXITED(status) ? WEXITSTATUS(status) : 1;
    }
    return last_status;
}

/* Helper for fork pipeline commands */
static int fork_pipeline_commands(struct pipeline_node *pipeline,
                                  struct pipe_pair *pipes, int *pids)
{
    for (int i = 0; i < pipeline->count; i++)
    {
        pid_t pid = fork();
        if (pid == -1)
            return -1;
        if (pid == 0)
        {
            setup_child_pipes(pipes, i, pipeline->count);
            exit(exec_ast(pipeline->commands[i]));
        }
        pids[i] = pid;
    }
    return 0;
}

/*
 * Execute pipeline
 * @param struct pipeline_node *pipeline
 * @return int
 */

int exec_pipeline(struct pipeline_node *pipeline)
{
    if (pipeline->count == 0)
        return 0;
    if (pipeline->count == 1)
        return exec_ast(pipeline->commands[0]);

    int *pids = malloc(sizeof(int) * pipeline->count);
    struct pipe_pair *pipes =
        malloc(sizeof(struct pipe_pair) * (pipeline->count - 1));
    if (create_pipes(pipes, pipeline->count) == -1)
        return (free(pids), free(pipes), 1);

    if (fork_pipeline_commands(pipeline, pipes, pids) == -1)
        return (free(pids), free(pipes), 1);

    close_all_pipes(pipes, pipeline->count);
    int result = wait_for_children(pids, pipeline->count);
    free(pids);
    free(pipes);
    return result;
}

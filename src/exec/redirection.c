#define _POSIX_C_SOURCE 200809L
#include "redirection.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Get default fd */
static int get_default_fd(struct redirection *r)
{
    if (r->io_number != -1)
        return r->io_number;

    if (r->redir_type == TOKEN_REDIR_IN || r->redir_type == TOKEN_REDIR_DUP_IN)
        return 0;

    return 1;
}

/* Helper for cleanup saved fds */
static void cleanup_saved_fds(struct saved_fd_pair *saved, int count)
{
    for (int j = 0; j < count; j++)
        close(saved[j].saved_fd);
    free(saved);
}

/* Helper for save original fd */
static int save_original_fd(int fd, struct saved_fd_pair *saved)
{
    saved->original_fd = fd;
    saved->saved_fd = dup(fd);
    if (saved->saved_fd == -1)
        return -1;
    fcntl(saved->saved_fd, F_SETFD, FD_CLOEXEC);
    return 0;
}

/* Handle fd duplication */
static int handle_fd_duplication(struct redirection *r, int fd,
                                 struct saved_fd_pair *saved, int i)
{
    if (strcmp(r->target, "-") == 0)
        return (close(fd), 0);

    int target_fd = atoi(r->target);
    if (dup2(target_fd, fd) == -1)
        return (cleanup_saved_fds(saved, i + 1), -1);

    return 0;
}

/* Get open flags */
static int get_open_flags(enum token_type type)
{
    switch (type)
    {
    case TOKEN_REDIR_IN:
        return O_RDONLY;
    case TOKEN_REDIR_OUT:
    case TOKEN_REDIR_CLOBBER:
        return O_WRONLY | O_CREAT | O_TRUNC;
    case TOKEN_REDIR_APPEND:
        return O_WRONLY | O_CREAT | O_APPEND;
    case TOKEN_REDIR_RDWR:
        return O_RDWR | O_CREAT;
    default:
        return -1;
    }
}

/* Handle file redirection */
static int handle_file_redirection(struct redirection *r, int fd,
                                   struct saved_fd_pair *saved, int i)
{
    int flags = get_open_flags(r->redir_type);
    if (flags == -1)
        return (cleanup_saved_fds(saved, i + 1), -1);

    int target_fd = open(r->target, flags, 0644);
    if (target_fd == -1)
        return (cleanup_saved_fds(saved, i + 1), -1);

    if (dup2(target_fd, fd) == -1)
        return (close(target_fd), cleanup_saved_fds(saved, i + 1), -1);

    close(target_fd);
    return 0;
}

/* Helper for setup single redirection */
static int setup_single_redirection(struct redirection *r,
                                    struct saved_fd_pair *saved, int i)
{
    int fd = get_default_fd(r);

    if (save_original_fd(fd, &saved[i]) == -1)
        return (cleanup_saved_fds(saved, i), -1);

    if (r->redir_type == TOKEN_REDIR_DUP_OUT
        || r->redir_type == TOKEN_REDIR_DUP_IN)
        return handle_fd_duplication(r, fd, saved, i);

    return handle_file_redirection(r, fd, saved, i);
}

/*
 * Handle setup redirections
 * @param struct redirection **redirs, int count,                       struct
 * saved_fd_pair **saved
 * @return int
 */

int setup_redirections(struct redirection **redirs, int count,
                       struct saved_fd_pair **saved)
{
    *saved = malloc(sizeof(struct saved_fd_pair) * count);
    if (!*saved)
        return -1;

    for (int i = 0; i < count; i++)
    {
        if (setup_single_redirection(redirs[i], *saved, i) == -1)
            return -1;
    }
    return 0;
}

/*
 * Handle restore redirections
 * @param struct saved_fd_pair *saved, int count
 * @return void
 */

void restore_redirections(struct saved_fd_pair *saved, int count)
{
    for (int i = count - 1; i >= 0; i--)
    {
        if (saved[i].saved_fd != -1)
        {
            dup2(saved[i].saved_fd, saved[i].original_fd);
            close(saved[i].saved_fd);
        }
    }
    free(saved);
}

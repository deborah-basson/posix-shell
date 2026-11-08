#ifndef REDIRECTION_H
#define REDIRECTION_H

#include "../ast/ast.h"

struct saved_fd_pair
{
    int original_fd;
    int saved_fd;
};

int setup_redirections(struct redirection **redirs, int count,
                       struct saved_fd_pair **saved);
void restore_redirections(struct saved_fd_pair *saved, int count);

#endif /* !REDIRECTION_H */

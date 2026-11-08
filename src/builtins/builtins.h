#ifndef BUILTINS_H
#define BUILTINS_H

int builtin_echo(char **argv, int argc);
int builtin_true(char **argv, int argc);
int builtin_false(char **argv, int argc);
int builtin_break(char **argv, int argc);
int builtin_continue(char **argv, int argc);
int builtin_exit(char **argv, int argc);
int builtin_cd(char **argv, int argc);
int builtin_export(char **argv, int argc);
int builtin_dot(char **argv, int argc);
int builtin_exec(char **argv, int argc);
int builtin_unset(char **argv, int argc);
int builtin_colon(char **argv, int argc);
int builtin_alias(char **argv, int argc);
int builtin_unalias(char **argv, int argc);

struct builtin_state
{
    int break_level;
    int continue_flag;
    int should_exit;
    int exit_code;
    int loop_depth;
};

extern struct builtin_state g_builtin_state;

#endif /* !BUILTINS_H */

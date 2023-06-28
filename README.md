# minishell
School42 project. This project is about creating an own bash-like system.

### Required implementations
- Implement redirections:  
  - ``<`` should redirect input.
  - ``>`` should redirect output.
  - ``<<`` should be given a delimiter.
  - ``>>`` should redirect output in append mode.
- Implement pipes (`|` character). 
- Handle environment variables ($ followed by a sequence of characters)
- Handle `$?`
- Handle ctrl-C, ctrl-D and ctrl-\ which should behave like in bash.
  - ctrl-C displays a new prompt on a new line.
  - ctrl-D exits the shell.
  - ctrl-\ does nothing.
- Implement the following builtins:
  - `echo` with option -n
  - `cd` with only a relative or absolute path
  - `pwd` with no options
  - `export` with no options
  - `unset` with no options
  - `env` with no options or arguments
  - `exit` with no options
- Display a prompt when waiting for a new command.
- Have a working history.
- Handle `’` which should prevent the shell from interpreting the metacharacters in the quoted sequence.
- Handle `"` which should prevent the shell from interpreting the metacharacters in the quoted sequence except for $ (dollar sign).

### Project restrictions
- Can only use C
- Not use more than one global variable.
- Not interpret unclosed quotes or special characters which are not required by thesubject such as `\` or `;`.
- Can use only following library functions:
  - readline, rl_clear_history, rl_on_new_line, rl_replace_line, rl_redisplay, add_history
  - malloc, free
  - printf, write, access, open, read, close
  - execve, fork, wait, waitpid, wait3, wait4, dup, dup2, pipe
  - signal, sigaction, sigemptyset, sigaddset, kill, exit
  - getenv, getcwd, chdir, unlink, opendir, readdir, closedir
  - strerror, perror


## Launch the program

- Clone the project and access the folder
```
git clone https://github.com/maryana-la/minishell && cd minishell/
```

- Build project (readline library is required for this project, guide to intall on [Mac] and [Linux]. Path to the library should be corrected in Makefile)
```
make
```

- After building the source, run  from the project root
```
./minishell
```

[Mac]: https://macappstore.org/readline/
[Linux]: https://ubuntu.pkgs.org/20.04/ubuntu-main-arm64/readline-common_8.0-4_all.deb.html

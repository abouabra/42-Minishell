# Minishell

## Description

Minishell is a simple shell implementation in C. It is a project from the 42 school curriculum. The goal of this project is to create a simple shell that can handle basic commands, environment variables, and signal handling, it also has to be able to handle redirections and pipes.
it also has a group of built-in commands that are implemented in C.

## Installation

1. Clone the repository:
```bash
git clone https://github.com/abouabra/42-Minishell.git
```
2. Change the current working directory:
```bash
cd 42-Minishell
```
3. Compile the project:
```bash
make
```
4. Run the program:
```bash
./minishell
```
## Features

Minishell provides the following features:

- Command execution: Minishell can execute commands entered by the user, including built-in commands and external programs.
- Quoting: Minishell supports single and double quotes to group arguments together.
- Built-in commands: Minishell supports built-in commands such as `cd`, `echo`, `pwd`, `export`, `unset`, `env`, and `exit`.
- Environment variables: Minishell can manage environment variables it including displaying using the `$` symbol, exporting, and unsetting variables.
- Input/output redirection: Minishell supports input/output redirection using `<`, `>`, and `>>` symbols.
- Pipes: Minishell supports piping multiple commands together using the `|` symbol.
- Signal handling: Minishell handles signals such as `Ctrl+C` (SIGINT) and `Ctrl+\` (SIGQUIT) gracefully.
- Error handling: Minishell provides informative error messages when commands fail to execute.
- Command history: Minishell supports command history using the `up` and `down` arrow keys.
- support for wildcard expansion using the `*` symbol.
- `&&` and `||` operators for conditional execution of commands.

## Usage

Once you have compiled the `minishell` program and run it, you can start using it as follows:


1. Enter commands at the prompt and press Enter to execute them. For example:
```bash
$ ls -l
```
2. Use built-in commands such as `cd`, `echo`, `exit`, and `env` as needed. For example:
```bash
$ cd <directory>
```

3. Use input/output redirection and pipes to perform more complex operations. For example:
```bash
$ ls -l > output.txt
$ cat input.txt | grep "pattern"
```

4. use the * wildcard to match any character in a file name. For example:
```bash
$ ls *.c
```

5. use the && and || operators for conditional execution of commands. For example:
```bash
$ ls -l && echo "ls succeeded" || echo "ls failed"
```

6. To exit Minishell, use the `exit` command:
```bash
$ exit
```



## Final Note
If you have any suggestions or improvements, feel free to open an issue or submit a pull request.

# minishell - A Minimal UNIX Shell

## 🛠️ Project Overview

`minishell` is a minimalistic UNIX shell built as part of the 42 curriculum. This project replicates key features of Bash, allowing users to execute commands, manage processes, and handle environment variables. The goal is to understand how a shell interacts with the operating system while implementing core shell functionalities.

## 🚀 Features
- **Command Execution** - Supports built-in and external commands.
- **Pipes (`|`)** - Enables command chaining through pipes.
- **Redirections (`>`, `<`, `>>`, `<<`)** - Implements file input/output redirections.
- **Environment Variables (`export`, `$VAR`)** - Allows variable manipulation.
- **Signal Handling** - Handles `Ctrl+C`, `Ctrl+D`, and `Ctrl+\` for process control.
- **Process Management** - Supports foreground and background execution.
- **Basic Parsing & Tokenization** - Splits and interprets user commands.

## 🕹️ Usage

### Running Minishell
1. Clone the repository:
   ```sh
   git clone https://github.com/zekmaro/minishell.git
   cd minishell
   ```
2. Compile the shell:
   ```sh
   make
   ```
3. Start the shell:
   ```sh
   ./minishell
   ```

### Built-in Commands
| Command | Description |
|---------|------------|
| `echo` | Prints text to standard output |
| `cd` | Changes the current directory |
| `pwd` | Displays the current directory |
| `export` | Sets an environment variable |
| `unset` | Unsets an environment variable |
| `env` | Displays environment variables |
| `exit` | Exits the shell |

### Example Usage
```sh
$ echo "Hello, World!"
Hello, World!
$ export VAR=42
$ echo $VAR
42
$ ls -l | grep minishell
$ cat file.txt > output.txt
$ ./script.sh &
```


## 🏆 Credits
- **Developer:** [zekmaro](https://github.com/zekmaro)
- **Project:** Part of the 42 School curriculum
- **Inspiration:** UNIX Shells (Bash, Zsh)

---
💻 Enjoy hacking around with `minishell`! Contributions and feedback are welcome.

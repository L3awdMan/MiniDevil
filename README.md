*This project has been created as part of the 42 curriculum by zotaj-di, baelgadi.*

<p align="center">
  <img src="doc/theme/minishellm.png" width="150" alt="Minishell Badge">
</p>

<p align="center">
  <img src="https://img.shields.io/badge/42-School_Project-000000?style=flat-square&logo=42&logoColor=white" alt="42"/>
  <img src="https://img.shields.io/badge/Language-C-00599C?style=flat-square&logo=c&logoColor=white" alt="C"/>
  <img src="https://img.shields.io/badge/Library-Readline-purple?style=flat-square" alt="Readline"/>
</p>

<p align="center">
  <img src="doc/theme/Minidevil_banner.gif" alt="MiniDevil Banner">
</p>

# Description

MiniDevil is a POSIX inspired shell written in C as part of the 42 school minishell project.\
It reads commands from the terminal, tokenizes & parses them into an AST (Abstract Syntax Tree) then executes the tree: handling pipes, redirections, environment variables and more, just like bash.

# Showcase

<p align="center">
  <img src="doc/theme/Minidevil_Showcase.gif" alt="MiniDevil Showcase">
</p>

<p align="center">
  <em>MiniDevil in action: pipes, redirections, heredoc and the UI mode</em>
</p>

# Quick start

```bash
git clone https://github.com/L3awdMan/MiniDevil.git
cd MiniDevil
make
./minishell
```

Want the extra UI mode (not in subject)?

```bash
./minishell --ui
```


> [!NOTE]
> The `cd` builtin command is not supported through the UI.
## Features

### ➤ Mandatory

- Interactive prompt with command history (readline)
- PATH based executable search and absolute OR relative path execution
- Single and double quote handling:
  - `'` prevents all interpretation
  - `"` allows for `$` expansion
- Redirections:
  - `<` (input)
  - `>` (output)
  - `>>` (append)
  - `<<` (heredoc)
- Pipes (`|`) connecting STDOUT to STDIN across commands
- Environment variable expansion (`$VAR`, `$?` ...)
- Signal handling:
  - `CTRL C` (new prompt)
  - `CTRL D` (exit)
  - `CTRL \` (ignored)
- Builtins: `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`

### ➤ Bonus

- `&&` and `||` operators
- Parentheses `()` for subshell
- Wildcard `*` globbing (in the current directory)

## Instructions

### ➤ Requirements

> [!NOTE]
> `libft` is vendored under `./libft/` and builds automatically as part of `make` — no manual setup needed.

- GCC or CC compiler
- GNU Make
- readline library (`libreadline-dev` on Debian/Ubuntu)

### ➤ Build

```sh
make          # builds the mandatory part
make bonus    # builds the bonus part
```

### ➤ Run

```sh
./minishell

# UI Mode (extra, not in subject)
./minishell --ui
```

### ➤ Clean

```sh
make clean    # remove object and dependency files
make fclean   # clean + remove binary
make re       # full rebuild
```

## Architecture

### ➤ Mandatory

<p align="center">
  <img src="doc/theme/Pipe-Mandatory.png" alt="Mandatory pipeline">
</p>

### ➤ Bonus

<p align="center">
  <img src="doc/theme/Pipe-Bonus.png" alt="Bonus pipeline">
</p>

Further information is provided in the documentation.

## Documentation

> [!CAUTION]
> The explanations in this repository are not intended to encourage cheating or any behavior that goes against 42's rules. Their purpose is to support the peer-to-peer learning system. Bader and I do not encourage, support, or take responsibility for any form of cheating related to this repository.

> [!IMPORTANT]
> If you have read our explanations carefully, you will understand that they are not enough on their own .. you still need to do your own research.

## Resources

- [Bash Reference Manual (GNU)](https://www.gnu.org/software/bash/manual/bash.html)
- [GNU Readline Library](https://tiswww.case.edu/php/chet/readline/rltop.html)
- [Valgrind Memcheck: Different ways to lose your memory](https://developers.redhat.com/blog/2021/04/23/valgrind-memcheck-different-ways-to-lose-your-memory)

### ➤ AI usage

AI tools (Claude) were used during this project for:

- Debugging assistance and code review
- Better understand core concepts and some edge cases
- Acting as a rubber duck colleague (making sure the Doxygen comments and pages were understandable and accurate)
- Greatly improved documentation by helping correct and refine it (Doxyfile & Doxygen xml/css/shell script)
- Helping structure the bonus architecture in an efficient way

## Authors

- **zotaj-di** — [42 intra](https://profile.intra.42.fr/users/zotaj-di) | [GitHub](https://github.com/L3awdMan)
- **baelgadi** — [42 intra](https://profile.intra.42.fr/users/baelgadi) | [GitHub](https://github.com/baderelg)

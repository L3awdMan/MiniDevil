# Doxygen quick guide

## What is Doxygen?

Doxygen reads special comments in the source code and generates HTML documentation with references, graphs and file structure. (also allows for adding entire custom pages as .dox files)

## Generate the docs

```sh
cd doc
doxygen Doxyfile
```

This creates `doc/html/`.\
Just open `doc/html/index.html` in your browser of choice.

> [!IMPORTANT]
> To make the SVG graphs prettier (and actually readable), you can run the shell script `sh theme/theme_graphs.sh ./html`\
> Or just directly `make doc` in the root folder which will generate the pages AND run the shell script at once

## Comment syntax

### ➤ Function documentation

Simply place a `/** ... */` block right above the function:

```c
/**
 * @brief Summary of what the function does
 *
 * Longer description if some further details are needed.
 *
 * @param name Description of the parameter
 * @return What the function returns
 */
int my_function(char *name, t_env **env)
```

You can also add some other options
- `@note` extra info that doesn't fit in the description
- `@warning` for something important
...

And to hide functions from showing on the pages, you can wrap them in `@cond` / `@endcond` (as we did for some header files)

### ➤ File header (optional)

At the top of a `.c` file, before any includes, it's possible to add:

```c
/**
 * @file executor.c
 * @brief Command execution logic
 */
```

Or, as we did, you can dedicate a .dox file for that: [files.dox](pages/files.dox)

## Some key settings

Defined in [Doxyfile](./Doxyfile)

| Setting | What it does |
|---------|-------------|
| `PROJECT_NAME` | Name shown in the header |
| `INPUT` | Directories to scan for source files |
| `FILE_PATTERNS` | Which files to parse (`*.c *.h`) |
| `RECURSIVE` | `YES` to scan subdirectories |
| `EXTRACT_STATIC` | `YES` to include static functions |
| `HAVE_DOT` | `YES` to generate SVG graphs (needs graphviz) |
| `CALL_GRAPH` | `YES` to show what each function calls |
| `CALLER_GRAPH` | `YES` to show who calls each function |
| `HTML_EXTRA_STYLESHEET` | Custom CSS file [minidevil.css](theme/minidevil.css) |
| `HTML_EXTRA_FILES` | Extra files copied to output (logo, images) |
| `LAYOUT_FILE` | Control the layout of the pages [layout.xml](theme/layout.xml) |

## Custom pages

Simply create a `.dox` file and make sure it's included in the Doxyfile.

```c
/**
 * @page my_page Page Title
 *
 * Content goes here. (Markdown can be used)
 */
```

You can also link to it from anywhere with `@ref my_page "Click here"`.

## Troubleshooting

- If you're missing the graphs, install graphviz (`sudo apt install graphviz`)
- In case you can't generate them, the pages are also available [right here](https://baderelg.github.io/42docs/Minishell/)
- For the full Doxygen documentation, see [doxygen.nl/manual](https://www.doxygen.nl/manual/index.html)

# rat 🐀

> A bad alternative of `cat` that belongs to sewers.

`rat` is a lightweight, scuffed command-line utility written in C. It streams files, concatenates them, and slaps numbers on lines. It doesn't have the decades of polish of GNU `cat` or the fancy Rust-powered wings of `bat`, but it does its best work down here in the drainage pipes.

## Features

* **Constant Memory Streaming:** Processes large files effortlessly using a fixed 4KB stack buffer without bloating RAM.
* **Standard Output Routing:** Spits data straight to `stdout`, letting your native OS shell handle file generation, overwriting, and appending.
* **Continuous Line Numbering (`-n`):** Injects ANSI-colored sequential numbers that track accurately across multiple files.

## Compilation

Since this is a raw C project, you'll need a C compiler (like `gcc`) installed on your system. Open your terminal and run:

```bash
gcc rat.c -o rat

```

*(Note: If you are on Windows, the output will automatically be `rat.exe`)*

## Usage

Because `rat` follows the Unix philosophy, it prints exclusively to the terminal stream. You can combine it with standard shell operators (`>` to overwrite, `>>` to append) to route data wherever you want.

**1. View a single file or stream multiple together:**

```bash
./rat filename.txt
./rat file1.txt file2.txt file3.txt

```

**2. Print with custom-styled line numbers:**

```bash
./rat -n filename.txt

```

**3. Concatenate files and save the output (Overwrite):**

```bash
./rat file1.txt file2.txt > combined.txt

```

**4. Append file contents onto an existing document:**

```bash
./rat file2.txt >> existing_file.txt

```

*(Note for Windows PowerShell users: Use `| Out-File -FilePath file.txt -Append -Encoding utf8` instead of `>>` to prevent character encoding issues!)*

## Why does this exist?

Because sometimes you just need to practice pointer math, buffer streaming, and escaping pointer traps in C, and building a rogue version of `cat` is a rite of passage.

## Contributing

If you find a bug... yes, you did. It's a rat. It belongs in the sewers.


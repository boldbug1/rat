# rat 🐀

> A bad alternative of `cat` that belongs to sewers.

`rat` is a lightweight, scuffed command-line utility written in C. It reads files, concatenates them, and slaps numbers on lines. It doesn't have the decades of polish of GNU `cat` or the fancy Rust-powered wings of `bat`, but it does its best work down here in the drainage pipes.

## Features

* **Standard Output:** Spits out file contents straight to your terminal.
* **Numbered Lines (`-n`):** Prints your file with line numbers (now safely using dynamic memory allocation and string concatenation without segfaulting).
* **Concatenate (`-a`):** Smashes two files together. 

## Compilation

Since this is a raw C project, you'll need a C compiler (like `gcc`) installed on your system. Open your terminal and run:

```bash
gcc rat.c -o rat

```

*(Note: If you are on Windows, the output will automatically be `rat.exe`)*

## Usage

**1. Basic file reading:**
Spit a single file or a stream of multiple sequential files directly to your stdout.
```bash
./rat filename.txt
./rat file1.txt file2.txt file3.txt

```

**2. Print with custom-styled line numbers (`-n`):**
Prints your file with sequential line numbers that accurately track across multiple files without resetting.

```bash
./rat -n filename.txt

```

**3. Direct output redirection (`-o`):**
Write the output of your files directly into a new target file rather than flooding your terminal screen.

```bash
./rat -o output.txt input1.txt input2.txt

```

**4. Append output to an existing file (`-a`):**
Safely dump your files onto the tail-end of an existing document without wiping out its current contents.

```bash
./rat -a logs.txt new_logs.txt

```

## Why does this exist?

Because sometimes you just need to practice pointer math, memory allocation, and file I/O in C, and building a rogue version of `cat` is a rite of passage.

## Contributing

If you find a bug... yes, you did. It's a rat. It belongs in the sewers.

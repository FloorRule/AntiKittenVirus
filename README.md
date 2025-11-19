# Anti-KittenVirus – Signature-Based Detection
This project implements a **C-based virus scanner** that detects malware inside files by searching for a given byte signature.
The program recursively scans a directory, reads each file, and checks whether the binary contains the signature pattern using a custom `memmem` implementation.

The scanner also supports **Normal Scan** and **Quick Scan** modes and logs all results to `Log.txt`.

---

## Features

* Recursively scans a directory for files
* Reads a virus signature file (`.sig`)
* Detects presence of the signature inside each file
* Supports:

  * **Normal Scan** – Full file scanning
  * **Quick Scan** – Only checks the first and last 20% of the file
* Automatically generates **Log.txt** with scan results
* Uses a custom implementation of `memmem` for pattern matching
* Prints infected or clean status for each file

---

## Usage

```
virus_scan.exe <directory_path> <signature_file>
```

### Arguments

* `<directory_path>` – Folder to scan (recursively)
* `<signature_file>` – Signature file containing the virus pattern (binary data)

### Example

```
virus_scan.exe scanFolder virus_beef.sig
```

---

## Scan Modes

After launching, the program displays:

```
Welcome to my Virus Scan!

====================================
Folder to scan: <path>
Virus signature: <signature>
====================================

Press 0 for Normal Scan or any key for a Quick Scan:
```

### Normal Scan (0)

* Scans the **entire contents** of each file.

### Quick Scan (any other key)

* Only scans:

  * First 20% of the file
  * Last 20% of the file
* Useful for faster detection when files are large.

---

## How It Works

1. Opens the directory using `opendir` / `readdir`.
2. For each file:

   * Reads the entire file into memory
   * Reads the virus signature
   * Calls a custom `memmem` function:

```c
int memmem(char *haystack, size_t hlen, char *needle, size_t nlen);
```

3. Determines whether the signature is present:

   * If found → **Infected**
   * Otherwise → **Clean**
4. Logs result to `Log.txt` in the scanned directory.

---

## Output Example

When scanning an infected test folder:

```
Scanning began...
This process may or may not take several minutes...

Scanning:
toScan/kitten_frog.jpg - Infected!
toScan/insideFolder/kitten_frog.jpg - Infected!
toScan/insideFolder/insideFolder2/kitten_frog.jpg - Infected!
```

At the end:

```
Scan Completed.
See log path for results: <path>/Log.txt
```

---

## Log File (`Log.txt`)

Generated automatically in the scanned folder.
Includes:

* Folder being scanned
* Signature file used
* Scan mode
* Detailed results for each file

Example entry:

```
Anti-virus began! Welcome!

Folder to scan:
scanFolder

Virus signature:
KittenVirusSign

Scanning option:
Quick Scan

Results:
toScan/kitten_frog.jpg  Infected!
toScan/CleanFile            Clean
...
```

---

## Compilation

### Windows (MinGW)

```
gcc -o virus_scan.exe find_sig.c
```

### Linux

```
gcc -o virus_scan find_sig.c
```

---

## Custom `memmem` Implementation

The project includes a full implementation of `memmem` for systems where it is unavailable.
It scans memory for a byte pattern, returning a pointer to the first match or `NULL` if none exists.

Special behavior:

* Detects whether the match appears in the first 20% or last 20% of the file for Quick Scan mode.

---

## Project Structure

```
.
├── prog.c            # Main scanner source code
├── KittenVirusSign       # Sample signature
├── toScan/           # Folder with test files
└── README.md
```

---

## Notes

* This scanner does **not** disinfect files; it only reports infection status.
* Works for binary signatures of any length.
* Designed as a learning exercise for directory traversal, file I/O, and byte-pattern scanning in C.

# 8cc on SQLite3

This is a SQLite3 port of [8cc](https://github.com/rui314/8cc)
built on [ELVM](https://github.com/shinh/elvm).

The compiler is implemented in a SELECT statement
using recursive common table expression.


## Requirements

SQLite3 shell (ver.3.9.0 or later) with JSON1 extension enabled


## Build

```
$ git clone https://github.com/youz/8cc-sqlite3.git
$ make
````

You'll get a database binary file `elvm.db3`.
Or get the pre-built binary from [here](https://github.com/youz/8cc-sqlite3/releases).

## DB objects
### Tables
- `src(b BLOB)` - C source table
- `eir(b BLOB)` - ELVM IR table
- `option(target TEXT)` - Option table

### Views
- `elvm_8cc(stdout TEXT)` - C compiler (compiles C to ELVM IR)
- `elvm_elc(stdout TEXT)` - ELVM IR assembler
- `elvm_8cc_elc(stdout TEXT)` - pipelining 8cc & elc


## Usage
### Import C source code

```sql
$ sqlite3 elvm.db3
sqlite3> DELETE FROM src;
sqlite3> INSERT INTO src(b) VALUES('
int putchar(int);
int main() {
  char* p = "Hello, world!\n";
  for (; *p; p++) {
    putchar(*p);
  }
  return 0;
}');
```

or use `readfile` function.

```sql
sqlite3> DELETE FROM src;
sqlite3> INSERT INTO src(b) VALUES(readfile('samples/hello.c'));
```

**Caution: Don't use #include directive in the C source** 

### Compile C to ELVM IR

Select `stdout` from the view `elvm_8cc` and insert it into the table `eir(b BLOB)`.

```sql
sqlite3> DELETE FROM eir;
sqlite3> INSERT INTO eir(b) SELECT stdout FROM elvm_8cc;
```
**Caution: It takes several tens of minutes to compile the helloworld program**


### Assemble ELVM IR to a target language

Set an alias-string for a target language to `option.target`.
See [Language List](#supported-target-languages) for details of alias strings.

Then select `stdout` from the view `elvm_elc`.

```sql
sqlite3> UPDATE option SET target = 'rb';
sqlite3> SELECT writefile('hello.rb', stdout) FROM elvm_elc;
```

### Compile and assmeble at one time

Use the view `elvm_8cc_elc`

```sql
sqlite3> DELETE FROM src;
sqlite3> INSERT INTO src(b) VALUES(readfile('source.c'));
sqlite3> UPDATE option SET target = 'js';
sqlite3> SELECT writefile('output.js', stdout) FROM elvm_8cc_elc;
```

## Supported target languages

| alias   | target language        |
|---------|------------------------|
| arm     | arm-linux              |
| bef     | Befunge                |
| bf      | Brainfuck              |
| c       | C                      |
| cl      | CommonLisp             |
| cpp     | C++14 (compile-time)   |
| cr      | Crystal (compile-time) |
| el      | Emacs Lisp             |
| forth   | Forth                  |
| go      | Go                     |
| i       | C-INTERCAL             |
| java    | Java                   |
| js      | JavaScript             |
| php     | PHP                    |
| piet    | Piet                   |
| pietasm | Piet Assembler         |
| pl      | Perl5                  |
| py      | Python                 |
| rb      | Ruby                   |
| sed     | sed                    |
| sh      | Bash                   |
| sqlite3 | SQLite3                |
| swift   | Swift                  |
| tex     | TeX                    |
| tf      | TensorFlow (WIP)       |
| tm      | Turing machine         |
| unl     | Unlambda               |
| vim     | Vim script             |
| ws      | Whitespace             |
| x86     | i386-linux             |

This list is based on [ELVM](https://github.com/shinh/elvm)
revision `e99b36e53bfd9c5721c579d143ac052cf94cdb17`.

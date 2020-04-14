# C REPL
Read, eval, print loop for C

### Dependencies
* POSIX system
* dlfcn.h
* regex.h
* gcc

### How to use:

Compile: `./build.sh`

Run: `./bin/main`

Enter C statements in the repl enviroment. `quit;` to exit the shell or `clear;` to clear the scope.

### Easy printf:
Use `%` followed by a format specifier (just like printf) and variable name to print the variable value.

```
>>> int k = 5;
>>> %d k;
k = 5
>>> double x = 0.6;
>>> x *= k;
>>> %f x;
```
Use just the variable name for a default format.

```
>>> int k = 5;
>>> k;
k = 5
>>> double x = 0.6;
>>> x *= k;
>>> x;
x = 3.000000
```

### Built with:
* C
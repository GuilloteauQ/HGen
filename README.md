# HGen

Header file generator for C files

## Presentation

This is a small tool to build a header file from a C file.


## Usage

```
./hgen file.c file.h
```

The functions of the C file must follow this convention:
```C
void name_of_the_function(int param1, int param2) {
    ...
}
```

and not

```C
void name_of_the_function(int param1, int param2)
{
    ...
}
```




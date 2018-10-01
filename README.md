# HGen

Header file generator for C files

## Presentation

This is a small tool to build a header file from a C file.


## Usage

### Calling

```
./hgen file1.c file2.c .. filen.c
```
This will create the header file as ```fileX.h```

### Functions

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

### Structures and others

If you need to copy a full structure (or something else), use the following syntax:

In the .c file:
```C
/* [raw]
struct btree {
    int value;
    struct btree* left;
    struct btree* right;
}; */
```
(it will actually copy everything between the ```/* [raw]``` and the ```};*/``` lines.)

Result in the .h file:
```C
struct btree {
    int value;
    struct btree* left;
    struct btree* right;
};
```


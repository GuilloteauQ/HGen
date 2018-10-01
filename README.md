# HGen

Header file generator for C files

## Presentation

This is a small tool to build a header file from a C file.


## Usage

### Calling

```
./hgen file.c
```
This will create the header file as ```file.h```

If you want to give a diffrent name to the header file:

```
./hgen file.c header_file.h
```
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

### Structures

If you need to have a structure in your header file, please use the following syntax, so the program will know what to copy in the h file:

In the .c file:
```C
/* [struct]
struct btree {
    int value;
    struct btree* left;
    struct btree* right;
}; */
```
(it will actually copy everything between the ```/* [struct]``` and the ```};*/``` lines.)

Result in the .h file:
```C
struct btree {
    int value;
    struct btree* left;
    struct btree* right;
};
```


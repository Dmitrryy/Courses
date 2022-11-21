# GLang

## Content

1. [Introduction](#introduction)
    * [Install](#install)
        * [Requirements](#requirements)
        * [Build](#build)
        * [Usage](#usage)
2. [Language](#language)
    * [Builtins](#builtins)
    * [Operators](#operators)
    * [Examples](#examples)

## Introduction
An object-oriented language. Remember only one rule: all you see are objects.
The syntax is a bit like a family .NET. 

## Install

### Requirements
* cmake 3.10
* LLVM ~14
* GNU bison 3.8
* GNU flex

### Build
```
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build .
```

### Usage

Perhaps need to write a tool that will throw it all by itself, but so far so
```
$ cd build
$ glang <file.cs>
$ clang++ dump.ll ../../glib/SEngine.cpp -lsfml-graphics -lsfml-window -lsfml-system
```

## Language

### Builtins
**Integer:** \
represent int64_t. Methods:
* Print()
* Set(Integer) \
All arifmethic methods dosn't change 'this'. Result will be returned:
* Add(Integer)
* Sub(Integer)
* Mult(Integer)
* Minus()
* Div(Integer)
* Equal(Integer)
* NotEqual(Integer)
* Greater(Integer)
* GreaterEqual(Integer)
* Less(Integer)
* LessEqual(Integer)
* Or(Integer)
* Xor(Integer)
* Not()
* And(Integer)
* Shl(Integer)
* AShr(Integer)
* SRem(Integer)

**Graphisc:** \
Object with methods to manage window. Methods:
* Init(width: Integer, heigth: Integer) - create window
* WindowIsOpen() : Integer
* Flush()
* PutPixel(x: Integer, y: Integer, rgba: Integer)
* GetTime() : Integer


### Operators

All operators (+, -, ==, & etc.) are abbreviations for calling the corresponding method from 'lhs' with the 'rhs' argument. That is, to define the '+' operator for castom type, you need to define the 'Add' method.

### Examples

See [tests folder](./tests/).
# WinPython

## Introduction

WinPython is a simple Python virtual machine, which implements a subset of modern Python language.

This project was my final assignment for the software engineering course at my university, where I majored in computer science. 

Now since the course is over, I have decided to open source it.

I hope this project will help my classmates to better understand how Python works, and inspire them to further explore the related computer science knowledge.

## Useage Method

In Command Line (`cmd.exe`) of Windows10 or Windows11, you can run this command to run your Python script (`.py` file) through the executable file built from the source code of WinPython:

```
WinPython <the path of your .py file>
```

At the same time, WinPython can run the binary byte code file (`.pyd` file) generated from a Python script file by CPython 2.7 or WinPython itself:

```
WinPython -d <the path of your .pyc file>
```

## Build

You can use Microsoft Visual Studio 2022 to build WinPython, which is very easy. 

Note that, since WinPython doesn't have its own compiler to convert the Python source code into byte code, it relies on the official Python compiler to do this. 

You can download the source code of official Python compiler [here](https://github.com/WU-SUNFLOWER/Python-Core), and compile it by Microsoft Visual Studio 2022 as well.

Then you will get a dll file. Please rename it to `WinPython.dll` and copy it to the same directory as the WinPython executable file you built, and finally you can run WinPython on your computer.

## Test Cases

I have made some test cases in the `test_cases` dictionary, which can test the basic features of WinPython. 

If you want to run these test cases in bulk, the Python script `dev_tools/run_test_cases.py` will help. Of course, since WinPython only implements a subset of Python and doesn't support bootstrapping, you'll need to run this script with the official CPython 3.8 or higher version.

## Logo

[Professor Zhang Weiwei](https://en.wikipedia.org/wiki/Zhang_Weiwei_(professor)) is the brand spokesman for WinPython, and he believes that the world situation will change in favour of an Eastern country.

![logo](https://raw.githubusercontent.com/WU-SUNFLOWER/WinPython/2024.05.21/docs/logo.png)

# json_lib
A simple and portable JSON-Parser written in C

## Description
This is a simple JSON-Parser written in C. I wrote this because i needed a simple parser without dependencies on a MC. 
Feel free to use it for anything u want.

## Advantages
- Simple to use (just a few lines of code)
- No additional dependencies
- Portable

## Disadvantages (Work to do)
- A JSON-List is just supported with JSON-Objects inside yet - i will fix this in future
- Not optimized for anything
- Just basic tests yet
- Access to values isn't perfect yet (as many checks to NULL). Best to write some wrappers for this
- No code comments yet. 

Feel free to improve anything you want.

## Usage:
You need to add the files in json directory to your project.
You also need to include the header file "json.h"

The file example.c provides a description of the usage of the library. In general it is really simple.
One give in a json-string and two pointers to JSON_OBJECT* and JSON_ARRAY* and then can iterate over these objects.

A JSON_OBJECT consists of a list of Key-Value-Pairs whereas a JSON_ARRAY consists of a list of JSON_OBJECT. 
You can see an explanation and short code examples in example.c

Be aware: The file example.c DOES NOT work even if it compiles. It will result in a coredump. This is for simplicity reasons. 
I wanted to keep the example as simple as possible, but still providing all necessary information and therefore just included the code-examples without additional checks or sth like that.
If u wonder why there is a coredump: It is because there is an iteration over the json_arr which in fact is NULL (due to an object as input).

## Author
Felix Froehling <felix.froehling1@gmail.com>



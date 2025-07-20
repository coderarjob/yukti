<p align="center">
    <img src="./docs/images/logo.png"/>
</p>

## About

Yukti is a single header testing framework for C/C++ projects. It has no 3rd party dependencies and
is fully contained within one single header file.

It provides most of the features available in other popular testing framework but in a small (< 1000
LOC) single header library.

* [Examples](#examples)
* [Documentation](#documentation)
    * [Test macros](#test-macros)
    * [Assertion macros](#assertion-macros)
    * [Interaction/behaviour validation macros](#interaction-or-behaviour-validation-macros)
    * [Mock or Fake function creation macros](#mock-or-fake-function-declaration)
* [Feedback](#feedback)

## Goals

- [X] **State testing**
    - [X] State/scalar value assertion macros
    - [X] Continuous data like array, string assertion macros
- [X] **Parameterised testing macros**
- [X] **Faking/Mocking external functions**
    - [X] Macros to fake external functions
    - [X] Behaviour modification of faked functions using custom Handler functions
- [X] **Interaction testing**
    - [X] Assert if an external function was called with expected arguments
    - [X] Assert if an external function was called with optional arguments
    - [X] Assert if an external function was never called
- [X] **Reporting**
    - [X] Report line numbers and source file of failed expectations
    - [X] Report list of all the tests which failed
    - [X] Tests executables exit with non-zero code if any of its tests fails

## Examples

Different examples are placed in the [example](./example) folder.

## Documentation

### Test macros

`YT_TEST` & `YT_TESTP` macros are used to define a single non-parameterised test and a parameterised
test respectively. Tests functions are identified by their name, that is the 2nd argument in these
macros. These tests functions need to be called in the `main()` function explicitly. Arguments for
parameterised tests are given when calling them in the `main()`, non-parameterised tests do not take
any argument.

Each test function must end with `YT_END()` macro. If omitted will result in compilation errors.

See [Parameterised test example](./example/basic/add_parameterised_test.c) example

### Assertion macros

Assertions macros check state expectations from an SUT (System Under Test). These are several of
these macros.

| Macro name             | Validates                                           |
|------------------------|-----------------------------------------------------|
| `YT_EQ_SCALAR(a, b)`   | a == b                                              |
| `YT_NEQ_SCALAR(a, b)`  | a != b                                              |
| `YT_GEQ_SCALAR(a, b)`  | a >= b                                              |
| `YT_LEQ_SCALAR(a, b)`  | a <= b                                              |
| `YT_GRT_SCALAR(a, b)`  | a > b                                               |
| `YT_LES_SCALAR(a, b)`  | a < b                                               |
| `YT_EQ_MEM(a, b, sz)`  | First `sz` bytes in buffers `a` & `b` are equal     |
| `YT_NEQ_MEM(a, b, sz)` | First `sz` bytes in buffers `a` & `b` are not equal |
| `YT_EQ_STRING(a, b)`   | String `a` and `b` are equal                        |
| `YT_NEQ_STRING(a, b)`  | String `a` and `b` are not equal                    |

#### Interaction or behaviour validation macros

More complex files work in conjunction with mocked functions. They help in validating interaction
between SUT and external functions. They help in determining if these external functions were called
in what order and which what parameters.

| Macro name                                        | Validates                                                                                 |
|---------------------------------------------------|-------------------------------------------------------------------------------------------|
| `YT_MUST_CALL_IN_ORDER(f, ...)`                   | Function `f` is called with the given arguments at least once in an particular order      |
| `YT_MUST_CALL_IN_ORDER_ATLEAST_TIMES(n, f, ...)`  | Function `f` is called with the given arguments at least `n` times in an particular order |
| `YT_MUST_CALL_ANY_ORDER(f, ...)`                  | Function `f` is called with the given arguments at least once in no particular order      |
| `YT_MUST_CALL_ANY_ORDER_ATLEAST_TIMES(n, f, ...)` | Function `f` is called with the given arguments at least `n` times in no particular order |
| `YT_MUST_NEVER_CALL(f, ...)`                      | Function `f` with the given arguments is never called                                     |

### Mock or Fake function declaration

When unittesting it might be required to provide a fake definitions of external functions. This is
where these macros come in. These fake functions also enable the above mentioned interaction
validations and one can modify the behaviour of these fake functions in various ways.

| Macro name                     | Validates                                                                                             |
|--------------------------------|-------------------------------------------------------------------------------------------------------|
| `YT_DECLARE_FUNC(rt, f, ...)`  | Declaration for fake function `f` which takes any number of arguments returns some non void type `rt` |
| `YT_DECLARE_FUNC_VOID(f, ...)` | Declaration for fake function `f` which takes any number of arguments returns void                    |
| `YT_DEFINE_FUNC(rt, f, ...)`   | Definition for fake function `f` previously declared using `YT_DECLARE_FUNC`.                         |
| `YT_DEFINE_FUNC_void(f, ...)`  | Definition for fake function `f` previously declared using `YT_DECLARE_FUNC_VOID`.                    |
| `YT_RESET_MOCK(f)`             | Resets internal state of a fake function previously defined using `YT_DEFINE_FUNC*`.                  |

## Feedback

Open a GitHub issue or drop a email at arjobmukherjee@gmail.com. I would love to hear your
suggestions and feedbacks.


# CGraph Code of Conduct

## Coding Guidelines

### C++
Please ensure your code follows the CGraph coding guidelines. Consistent coding style helps maintain the codebase.

Based on Google style guide, with some differences(perhaps):

- Function naming: First letter lowercase, camelCase
  ```
  int getCode() {
    return 0;
  }
  ```

- Class and struct naming: First letter uppercase, camelCase
  ```
  class MyClass {
  }

  struct MyStruct {
  }
  ```

- Variable naming: First letter lowercase, camelCase
  ```
  int myVar = 0;
  ```

- Member variable naming: First letter lowercase, words connected by underscore, ends with underscore
  ```
  class MyClass {
    int my_var_;
  }

  struct MyStruct {
    int my_var_;
  }
  ```

- Macro naming: All uppercase, words connected by underscore
  ```
  #define MY_MACRO (1)
  ```

- Enum naming: All uppercase, words connected by underscore
  ```
  enum MyEnum {
    MY_ENUM_VALUE1,
    MY_ENUM_VALUE2,
    MY_ENUM_VALUE3,
  };
  ```

----

### Python

Based on the Google Python Style Guide, the basic requirements are as follows:

1. Files and Encoding
    - **File Encoding**: Files should be encoded in UTF-8 and usually do not need an encoding declaration at the top of the file (Python 3 uses UTF-8 by default).
    - **File Naming**: First letter uppercase, camelCase, e.g., `MyModule.py`.

2. Comments
    - **Module Comments**: Each module should start with a module-level docstring describing the module's purpose, functionality, and main classes or functions. For example:
      ```python
      """
      This module provides utility functions for handling strings.

      It includes functions for string formatting, splitting, and joining.
      """
      ```
    - **Function and Method Comments**: Functions and methods also require docstrings explaining their purpose, parameters, return values, and possible exceptions. For example:
      ```python
      def addNumbers(a: int, b: int):
          """
          Add two numbers together.

          Args:
              a (int): The first number.
              b (int): The second number.
 
          Returns:
              int: The sum of a and b.
          """
          return a + b
      ```
    - **Inline Comments**: Use inline comments in the code to explain complex or non-intuitive parts, but don't overuse them. Comments should be concise and clear.

3. Classes and Objects
    - **Class Naming**: Class names should use CapWords convention, e.g., `MyClass`.
    - **Class Docstrings**: Classes should include a docstring describing their purpose and main functionality.
      ```python
      class MyClass:
          """
          A simple class that represents a person.
 
          Attributes:
              name (str): The name of the person.
              age (int): The age of the person.
          """
          def __init__(self, name: str, age: int):
              self.name = name
              self.age = age
 
          def getInfo(self):
              """
              Get the person's information.
 
              Returns:
                  str: A string containing the person's name and age.
              """
              return f"Name: {self.name}, Age: {self.age}"
      ```
    - **Member Variable and Method Naming**: Member variables and method names should use lowercase with words separated by underscores, e.g., `my_variable` and `my_method`.

4. Functions and Methods
    - **Function Naming**: Function names should use lowercase first letter, camelCase, e.g., `checkResult()`.
    - **Parameter Naming**: Parameter names should also use lowercase first letter, camelCase, and be descriptive, e.g., `myResultValue`.
    - **Function Length**: Functions should be kept short and focused on a single task. Avoid excessively long functions.

5. Code Layout
    - **Indentation**: Use 4 spaces for indentation, not tabs.
    - **Line Length**: Limit each line of code to a maximum of 120 characters. If it exceeds, break the line while maintaining code readability. Usually break after operators. For example:
      ```python
      result = getResult(arg1, arg2,
                         arg3, arg4)
      ```
    - **Blank Lines**: Use blank lines to separate logical sections, e.g., two blank lines between functions and classes, one blank line between methods within a class.

6. Import Statements
    - **Import Order**: Group import statements in the order of standard library, third-party libraries, and local modules, with a blank line between each group. For example:
      ```python
      import os
      import sys

      import requests

      from MyModule import MyClass
      ```
    - **Avoid Wildcard Imports**: Avoid using wildcard imports like `from module import *` as they can lead to naming conflicts and reduced code readability.

7. Exception Handling
    - **Specific Exception Types**: When catching exceptions, specify the exact exception type instead of using a generic `except` statement. For example:
      ```python
      try:
          result = 1 / 0
      except ZeroDivisionError:
          print("Division by zero occurred.")
      ```

8. Testing
    - **Write Unit Tests**: Write unit tests for your code to ensure correctness and stability. You can use Python's `unittest` or `pytest` testing frameworks.

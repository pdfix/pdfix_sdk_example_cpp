# PDFix SDK example C++
Example project demonstrating the capabilities of PDFix SDK written in C++
language.

## Description

For more information please visit [https://pdfix.net](https://pdfix.net).

## Code integration
```cpp
#include "Pdfix.h" // main header

Pdfix_statics;     // static definitions 

int main() {
    // initialize Pdfix. You can change the Pdfix_MODULE_NAME macro to path to the pdfix dll/dylib/so
    if (!Pdfix_init(Pdfix_MODULE_NAME))
      throw std::runtime_error("Pdfix library not found. Copy pdfix binaries into application directory.");
    
    // get the main object
    Pdfix* pdfix = GetPdfix();
    
    // your code ...
    
    pdfix->Destroy();
    return 0;
}
```

## Prerequisites
### All platforms
- CMake 3.10.0+

### Linux
Supported linux distributions:
- Ubuntu 16.04
- CentOS
- Debian

### MacOS
- Xcode 12.0+

### Windows
- MS Visual Studio 2017 or latest

## Download
Clone the repository:  
`git clone https://github.com/pdfix/pdfix_sdk_example_cpp`  

...or download as .zip:  
[Download as .zip](https://github.com/pdfix/pdfix_sdk_example_cpp/archive/master.zip)

## Build
Run the following commands in the pdfix_sdk_example_cpp folder:

1. `cmake -S . -B build`  
2. `cmake --build build`

## Run the example

From pdfix_sdk_exmple_cpp dir:

### MacOS
 `./bin/darwin/example`

### Linux
 `./bin/linux/example`

### Windows
 `./bin/x86/example`  
 `./bin/x64/example`

## Have a question? Need help?
Let us know and we’ll get back to you. Write us to support@pdfix.net or fill the
[contact form](https://pdfix.net/support/).

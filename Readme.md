# Template for a empty C++ application

This is a minimal template for creating application in C++ 

# Folder structure

```
.
├── 3rdparty
│   ├── CMakeLists.googletest.in
│   ├── CMakeLists.googletest.txt
│   └── < add your 3rd party libraries >
├── CMakeLists.txt
├── config.h.in
├── src
│   ├── CMakeLists.txt
│   ├── launcher
│   │   ├── cmake
│   │   │   ├── plugins.cfg.in
│   │   │   └── resources.cfg.in
│   │   ├── CMakeLists.txt
│   │   └── main.cpp
│   └── < add other libraries here >
└── tests
    ├── CMakeLists.txt
    ├── sample_test
    │   ├── CMakeLists.txt
    │   ├── main.cpp
    │   └── simpletest.cpp
    └── < add other tests here >
```
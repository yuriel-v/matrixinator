# The Matrixinator v1.0
The Matrixinator is a small utility I made for my lab assistant Joshu.

He once asked me about an application that could help him predict 8 values,
named BS22, BS15, BS3, BS8, BS27, BS84, BS18 and BS278. These would be
calculated off a similarity matrix generated from a similarity dendrogram,
loaded from a metadata file in .csv format and a dendrogram export in .xml format.

So, being the kind soul I am, I offered my help and thus developed this utility
in C++ for him to use in his lab. It doesn't seem like he understands the language
this program is written in... so he's attempting to recreate it in Python.

Please notice this program was compiled using MSVC and won't do too well if compiled in GCC.

Future plans include:
- [x] Improve execution time (52 mins is too long!);
- [x] Provide basic initialization file support (typing directories is annoying);
- [x] Migrate to CMake and enable use of Boost libraries;
- [x] An automatic way of finding .xml and .csv files for use (through <filesystem> from C++ 17 standard library);
- [x] Made it a sensitive program that does not like to be called names or told off. It'll turn highly offensive so don't do it!;
- [ ] Getting started on the Python version of this, kindly nicknamed "The Pythrixinator";
- [ ] A prettier UI in a far, remote future.

> Leo, 12-Feb-2020
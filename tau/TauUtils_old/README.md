# TauUtils

This contains lots of common utilities used throughout most of the other projects.

Some things this contains:

- Alignment helpers

- Array List

  - A dynamically expanding array using page based allocation

- Atomic Intrinsics

- Type Safe Console Printing

- Dynamic Array

  - Non expanding arrays that manage allocation and deallocation

- Endian Helpers

- Enum Bit Fields

  - Operators and functions to treat enumerations as a set of flags

- Various numerical values

  - pi, e, tau, etc

- Numerical Types

  - Basic sized integers, wrappers for `size_t`, min and max value helpers

- Class Macros

  - Default and delete construction, copy, and destruction macros

- Reference Counting Pointers

- Basic RTTI

  - O(1) time, uses simple integers

- Various Safety Helpers

  - Macros for settings optional error code pointers
  - Some unused Expected types

- Strings

- Type Name Helpers

- Various Math Functions

  - min/max
  - Epsilon equivalency checking 
  - Leading/Trailing 0 counting
  - log2i

- Various Other Macros

  - DLL Export/Import
  - Unused helpers
  - NoVtable (Win32 Only)
  - Restrict
  - Breakpoint
  - null

- Variable length array

  - Uses either compiler extension or `alloca`

- Fixed Block Allocator

  - 3 different modes of tracking
  - 2 different operating modes
    - Arena (Everything is released at once)
    - Normal (Destroyed blocks can be reused)

- Free List Allocator

  - Specialty allocator used to release a lot of shared pointers at once
  - This is primarily used by command lists in Tau Engine

- Page Allocator

  - Some helper functions for allocating pages

- AVL Tree

  - Contains a typical tree with pointers and a streamed (DOP) tree where every element is broken up into its own array

  


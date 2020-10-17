# Tau Reflection Generator

A small tool inspired by Unreal Engine's Header Tool for automatically generating reflection information. Currently it's in the very early stages of development, but it does support generating `get` and `set` attributes (`const` and non-`const` pointers).

This uses Clang's LibTooling to help with the parsing. First it has a preprocessor callback that will record every time one of our macros is expanded. The tool will keep track of the source location and will process all of its attributes. Each instance is pushed into a queue. Then later when walking the AST look at every time a field is declared, check to see if it is after a macro, if it is it will pop off that macro from the queue, then insert the property into another list, now with the properties name. After all of the file has been parsed it will generate a new file (currently fixed in the code) which will contain the macro `GENERATED_BODY()`. This macro will contain an entire class, implementing the reflection class (currently some `getProperty` functions), as well as a static function containing a static instance of the new class, and an instance function for getting the class, which will forward its call to the static function.

## Interface

Currently you can use `TAU_PROPERTY()` macros directly before a field declaration and specify `get` and/or `set` in the argument list. At some point in the class you need to call the macro `GENERATED_BODY()`.

After everything's been generated you can either call `GetStaticClass()` on the type or `getClass()` on the instance to get an instance of reflection class. Within that you have a couple of variations on `getProperty(_T* object, const char* propertyName)`, mainly some templates to have the function return a specific type, and some differentiation between `const` and non-`const` data.


@echo off
conan create . -s build_type=Release
conan create . -s build_type=Debug

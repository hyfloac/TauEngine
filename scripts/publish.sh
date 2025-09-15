#!/usr/bin/env bash

set -e

for release in Release Debug
do
  for shared in True False
  do
    for useTauUtils in True False
    do
      rm -r ./build/
      conan install . --build missing -s compiler.cppstd=20 -s build_type=$release -o:a "&:shared=$shared" -o:a "&:useTauUtils=$useTauUtils"
      cmake --preset conan-${release,,}
      cmake --build --preset conan-${release,,}
      conan create . -s build_type=$release -o:a "&:shared=$shared" -o:a "&:useTauUtils=$useTauUtils"
      conan export-pkg . -s build_type=$release -o:a "&:shared=$shared" -o:a "&:useTauUtils=$useTauUtils"
    done
  done
done

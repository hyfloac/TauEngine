# TauMathLib

Contains a lot of vector math classes written in LLVM IR.

This was mostly written to improve code gen for vector maths. It was also used as a convenient way to learn more about LLVM IR. Another nice benefit is it makes it very easy to do higher dimension vectors; while uncommon, I have heard of game engines having to use 5 component matrices for special occlusion culling.

This all came about when analyzing the output for some vector maths and I noticed that the compiler was making some rather odd choices. In this example we can see that we're adding 3 floats, these vector types are going to be passed into the `XMM` registers.

```c++
struct Vec3 final
{
    float x, y, z;
};

Vec3 add(const Vec3 a, const Vec3 b)
{
    Vec3 ret = { a.x + b.x, a.y + b.y, a.z + b.z };
    return ret;
}
```

Here we can see that as an output it generated a packed add and a single add. This is odd because you can fit 4 floats into an `XMM` register.

```assembly
add(Vec3, Vec3):
    addps   xmm0, xmm2
    addss   xmm1, xmm3
    ret
```



All of this gets even more strange for a Vector 4.

```c++
struct Vec4 final
{
    float x, y, z, w;
};

Vec4 add(const Vec4 a, const Vec4 b)
{
    Vec4 ret = { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
    return ret;
}
```

Here we get two packed adds. This could be a single packed add.

```assembly
add(Vec4, Vec4):
    addps   xmm0, xmm2
    addps   xmm1, xmm3
    ret
```

The LLVM IR generation for this is even more strange. Instead of everything being passed a vector of 4 floats, it is passed in at 2 vectors of 2 floats, and is returning a structure of 2 vectors of 2 floats.

```llvm
define dso_local { <2 x float>, <2 x float> } @vec4_add(<2 x float> %0, <2 x float> %1, <2 x float> %2, <2 x float> %3) local_unnamed_addr #0 {
  %5 = fadd <2 x float> %0, %2
  %6 = fadd <2 x float> %1, %3
  %7 = insertvalue { <2 x float>, <2 x float> } undef, <2 x float> %5, 0
  %8 = insertvalue { <2 x float>, <2 x float> } %7, <2 x float> %6, 1
  ret { <2 x float>, <2 x float> } %8
}

attributes #0 = { norecurse nounwind readnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="64" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

```



Instead I've chosen to go for a much cleaner approach. This is the function being used for the Vector3f add. We take in 2 vectors of 3 floats, add them and return a vector of 3 floats. All of these values are going to be passed through the `XMM` registers.

```llvm
define <3 x float> @vec3_add(<3 x float> %a, <3 x float> %b) local_unnamed_addr #0
{
    %ret = fadd <3 x float> %a, %b
    ret <3 x float> %ret
}

attributes #0 = { inlinehint nofree norecurse willreturn nosync nounwind optsize readnone }
```

Thus this has the very clean output of

```assembly
vec3_add:
    addps   xmm0, xmm1
    ret
```


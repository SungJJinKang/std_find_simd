# std::vector.find simd version
std::vector.find simd version

std::find doesn't use simd intrinsics.      
So i thought simd can make std::find faster.       
You don't need worry about alignment problem. ( we handle it )          

## Requirement

- std::vector's value type must be arithmetic type or pointer type
- require avx2 option

## Usage
```
std::vector<short> a;
a.reserve(1001);
for (unsigned short i = 0; i <= 1000; i++)
{
  a.push_back(i);
}

for (short i = 0; i < 1000; i++)
{
  fast_vector_find::find_simd(a, (short)i);
}
```

## Benchmark 

![20211128031529](https://user-images.githubusercontent.com/33873804/143701373-1c8aafbe-6131-4538-9d60-5432b84cd87c.png)

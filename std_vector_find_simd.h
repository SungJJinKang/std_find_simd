#ifndef __AVX2__

#error "Please enable AVX2 option"

#endif

#include <vector>
#include <immintrin.h>
#include <type_traits>
#include "builtin.h"


#if defined(__GNUC__)  || defined( __clang__)
#  define _FAST_VECTOR_FIND_CURRENT_CPP_VERSION __cplusplus
#elif defined(_MSC_VER)
#  define _FAST_VECTOR_FIND_CURRENT_CPP_VERSION _MSVC_LANG 
#endif

#if (_FAST_VECTOR_FIND_CURRENT_CPP_VERSION == 201703L) || (_FAST_VECTOR_FIND_CURRENT_CPP_VERSION == 202002L)
#define _FAST_VECTOR_FIND_CONSTEXPR constexpr
#else
#define _FAST_VECTOR_FIND_CONSTEXPR
#endif


#define _FAST_VECTOR_FIND_IS_ALIGNED(POINTER, BYTE_COUNT) \
    (((uintptr_t)(const void *)(POINTER)) % (BYTE_COUNT) == 0)




namespace fast_vector_find
{
    // TODO : remove #include <type_traits>, implement custom is_arithmetic<T>, is_pointer<T>

    template <typename T>
    struct is_find_simd_capable
    {
        static _FAST_VECTOR_FIND_CONSTEXPR const bool value = 
            ((std::is_arithmetic<T>::value == true) || (std::is_pointer<T>::value == true)) &&
            (sizeof(T) == 1 || sizeof(T) == 2 || sizeof(T) == 4 || sizeof(T) == 8)
    	;
    };

    // RandomAccessIterator is not allowed as function parameter
	// because msvc prohibit dereference value initialized iterator
    
    // FUNCTION TEMPLATE find_if_not
    template <typename T, typename Allocator>
    extern typename std::vector<T, Allocator>::iterator find_simd(std::vector<T, Allocator>& vector, const T value)
	{
        static_assert( is_find_simd_capable<T>::value == true, "unsupported type ( vector element should be arithmetic or pointer type )");

        using vector_value_type = typename std::vector<T, Allocator>::value_type;

        static_assert(
            (sizeof(vector_value_type) == 1) || 
            (sizeof(vector_value_type) == 2) ||
            (sizeof(vector_value_type) == 4) ||
            (sizeof(vector_value_type) == 8),
            "unsupported type ( vectore element's type size should be 1 or 2 or 4 or 8 )"
            );

        const vector_value_type* const begin = vector.data();
        const vector_value_type* const end = vector.data() + vector.size();
        vector_value_type* compare = vector.data();
       

        while( ( compare != end ) && _FAST_VECTOR_FIND_IS_ALIGNED(compare, 32) == false)
        {// scalar compare until aligned to 32 byte ( AVX2, 256bit )
	        if(*compare == value)
	        {
                return vector.begin() + (((uintptr_t)compare - (uintptr_t)begin) / sizeof(vector_value_type)) + (((uintptr_t)compare - (uintptr_t)begin) % sizeof(vector_value_type));
	        }
            compare++;
        }

        // now address of compare is aligned to 32 byte

        if _FAST_VECTOR_FIND_CONSTEXPR (sizeof(vector_value_type) == 1)
        {
            while(compare + 32 < end)
            {
                const __m256i compareSIMDValue = _mm256_set1_epi8(*(char*)(&value));
                const __m256i cmp = _mm256_cmpeq_epi8(*(__m256i*)compare, compareSIMDValue);
                const int z = _mm256_movemask_epi8(cmp);
                if (z)
                {
                    const int first_1_pos = psnip_builtin_ffs(z) - 1;
                    return vector.begin() + ((uintptr_t)compare + (uintptr_t)first_1_pos - (uintptr_t)begin);
                }

                compare += 32;
            }
        }
        else if _FAST_VECTOR_FIND_CONSTEXPR (sizeof(vector_value_type) == 2)
        {
            while (compare + 16 < end)
            {
                const __m256i compareSIMDValue = _mm256_set1_epi16(*(short*)(&value));
                const __m256i cmp = _mm256_cmpeq_epi16(*(__m256i*)compare, compareSIMDValue);

                const __m256i IncludeSlotsShifted = _mm256_srli_epi16(cmp, 8);
                const int z = _mm256_movemask_epi8(IncludeSlotsShifted);
                if (z)
                {
                    const int first_1_pos = psnip_builtin_ffs(z) - 1;
                    return vector.begin() + (((uintptr_t)compare + (uintptr_t)first_1_pos - (uintptr_t)begin) >> 1);
                }

                compare += 16;
            }
        }
        else if _FAST_VECTOR_FIND_CONSTEXPR (sizeof(vector_value_type) == 4)
        {
            while (compare + 8 < end)
            {
                const __m256i compareSIMDValue = _mm256_set1_epi32(*(int*)(&value));
                const __m256i cmp = _mm256_cmpeq_epi32(*(__m256i*)compare, compareSIMDValue);
                const int z = _mm256_movemask_ps(*(__m256*)(&cmp));
                if (z)
                {
                    const int first_1_pos = psnip_builtin_ffs(z) - 1;
                    return vector.begin() + (((uintptr_t)compare - (uintptr_t)begin) >> 2) + first_1_pos;
                }

                compare += 8;
            }
        }
        else if _FAST_VECTOR_FIND_CONSTEXPR (sizeof(vector_value_type) == 8)
        {
            while (compare + 2 < end)
            {
                const __m256i compareSIMDValue = _mm256_set1_epi64x(*(long long*)(&value));
                const __m256i cmp = _mm256_cmpeq_epi64(*(__m256i*)compare, compareSIMDValue);
                const int z = _mm256_movemask_pd(*(__m256d*)(&cmp));
                if (z)
                {
                    const int first_1_pos = psnip_builtin_ffs(z) - 1;
                    return vector.begin() + (((uintptr_t)compare - (uintptr_t)begin) >> 3) + first_1_pos;
                }

                compare += 4;
            }
        }


        while (compare != end)
        {
            if (*compare == value)
            {
                return vector.begin() + (((uintptr_t)compare - (uintptr_t)begin) / sizeof(vector_value_type)) + (((uintptr_t)compare - (uintptr_t)begin) % sizeof(vector_value_type));
            }
            compare++;
        }

        return vector.end();

    }

    template <typename T, typename Allocator>
    extern typename std::vector<T, Allocator>::const_iterator find_simd(const std::vector<T, Allocator>& vector, const T value)
    {
        return find_simd(const_cast<std::vector<T, Allocator>&>(vector), value);
    }
}
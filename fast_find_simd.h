#if ( !defined(__AVX2__) && ( !defined(__AVX512BW__) || !defined(__AVX512VL__) ) )

#error "Please enable AVX2 or ( __AVX512BW__ and __AVX512VL__ )option"

#endif

#include <iterator>
#include <immintrin.h>
#include <type_traits>
#include "builtin.h"

#if defined(__GNUC__)  || defined( __clang__)
#  define FAST_FIND_SIMD_CURRENT_CPP_VERSION __cplusplus
#elif defined(_MSC_VER)
#  define FAST_FIND_SIMD_CURRENT_CPP_VERSION _MSVC_LANG 
#endif

#if (FAST_FIND_SIMD_CURRENT_CPP_VERSION == 201703L) || (FAST_FIND_SIMD_CURRENT_CPP_VERSION == 202002L)
#define FAST_FIND_SIMD_CONSTEXPR constexpr
#else
#define FAST_FIND_SIMD_CONSTEXPR
#endif


#define FAST_FIND_SIMD_IS_ALIGNED(POINTER, BYTE_COUNT) \
    (((uintptr_t)(const void *)(POINTER)) % (BYTE_COUNT) == 0)




namespace fast_find_simd
{
    // TODO : remove #include <type_traits>, implement custom is_scalar<T>

    namespace details
    {
        template<typename T, typename = void>
        struct is_iterator
        {
            static constexpr bool value = false;
        };

        template<typename T>
        struct is_iterator<T, typename std::enable_if<!std::is_same<typename std::iterator_traits<T>::value_type, void>::value>::type>
        {
            static FAST_FIND_SIMD_CONSTEXPR const bool value = true;
        };
    }

    template <typename ITERATOR_TYPE>
    struct is_iteratype_type_find_simd_capable
    {
        static_assert(details::is_iterator<ITERATOR_TYPE>::value == true, "Please pass iterator type");

        using iterator_value_type = typename std::iterator_traits<ITERATOR_TYPE>::value_type;

        static FAST_FIND_SIMD_CONSTEXPR const bool value =
            (std::is_same<typename std::iterator_traits<ITERATOR_TYPE>::iterator_category, std::random_access_iterator_tag>::value == true) &&
            (std::is_scalar<iterator_value_type>::value == true) &&
            (sizeof(iterator_value_type) == 1 || sizeof(iterator_value_type) == 2 || sizeof(iterator_value_type) == 4 || sizeof(iterator_value_type) == 8)
    	;
    };
    

    template <typename LEGACY_RANDOM_ITERATOR>
    extern typename LEGACY_RANDOM_ITERATOR find_simd(LEGACY_RANDOM_ITERATOR beginIter, LEGACY_RANDOM_ITERATOR endIter, const typename std::iterator_traits<LEGACY_RANDOM_ITERATOR>::value_type value)
    {
        static_assert(details::is_iterator<LEGACY_RANDOM_ITERATOR>::value == true, "Please pass iterator type");

        using iterator_value_type = typename std::iterator_traits<LEGACY_RANDOM_ITERATOR>::value_type;

        static_assert(std::is_same<typename std::iterator_traits<LEGACY_RANDOM_ITERATOR>::iterator_category, std::random_access_iterator_tag>::value == true, "unsupported type ( iterator should be random iterator )");

    	static_assert(std::is_scalar<iterator_value_type>::value == true, "unsupported type ( iterator's value type should be scalar type )");

        static_assert(
            (sizeof(iterator_value_type) == 1) ||
            (sizeof(iterator_value_type) == 2) ||
            (sizeof(iterator_value_type) == 4) ||
            (sizeof(iterator_value_type) == 8),
            "unsupported type ( iterator's value type size should be 1 or 2 or 4 or 8 )"
            );


        iterator_value_type* compare = &(*beginIter);

        const iterator_value_type* const begin = compare;
        const iterator_value_type* const end = &(*(endIter - 1)) + 1; // dereferencing end iterator make assertion


        while ((compare != end) && FAST_FIND_SIMD_IS_ALIGNED(compare, 32) == false)
        {// scalar compare until aligned to 32 byte ( AVX2, 256bit )
            if (*compare == value)
            {
                return beginIter + (((uintptr_t)compare - (uintptr_t)begin) / sizeof(iterator_value_type)) + (((uintptr_t)compare - (uintptr_t)begin) % sizeof(iterator_value_type));
            }
            compare++;
        }

        // now address in compare variable is aligned to 32 byte

        if FAST_FIND_SIMD_CONSTEXPR(sizeof(iterator_value_type) == 1)
        {
            while (compare + 32 <= end)
            {
                const __m256i compareSIMDValue = _mm256_set1_epi8(*(char*)(&value)); // maybe compiler will cache this variable.

#if ( defined(__AVX512BW__) && defined(__AVX512VL__) )
                const __mmask32 z = _mm256_cmpeq_epi8_mask(*(__m256i*)compare, compareSIMDValue);
#elif defined(__AVX2__)
            	const __m256i cmp = _mm256_cmpeq_epi8(*(__m256i*)compare, compareSIMDValue);
                const int z = _mm256_movemask_epi8(cmp);
#endif

                if (z)
                {
                    const int first_1_pos = psnip_builtin_ffs(*(int*)&z) - 1;
                    return beginIter + ((uintptr_t)compare + (uintptr_t)first_1_pos - (uintptr_t)begin);
                }

                compare += 32;
            }
        }
        else if FAST_FIND_SIMD_CONSTEXPR(sizeof(iterator_value_type) == 2)
        {
            while (compare + 16 <= end)
            {
                const __m256i compareSIMDValue = _mm256_set1_epi16(*(short*)(&value));


#if ( defined(__AVX512BW__) && defined(__AVX512VL__) )
                const __mmask16 z = _mm256_cmpeq_epi16_mask(*(__m256i*)compare, compareSIMDValue);
#elif defined(__AVX2__)
                const __m256i cmp = _mm256_cmpeq_epi16(*(__m256i*)compare, compareSIMDValue);
                const __m256i IncludeSlotsShifted = _mm256_srli_epi16(cmp, 8);
                const int z = _mm256_movemask_epi8(IncludeSlotsShifted);
#endif
                if (z)
                {
                    const int first_1_pos = psnip_builtin_ffs(*(int*)&z) - 1;
                    return beginIter + (((uintptr_t)compare + (uintptr_t)first_1_pos - (uintptr_t)begin) >> 1);
                }

                compare += 16;
            }
        }
        else if FAST_FIND_SIMD_CONSTEXPR(sizeof(iterator_value_type) == 4)
        {
            while (compare + 8 <= end)
            {
                const __m256i compareSIMDValue = _mm256_set1_epi32(*(int*)(&value));

#if ( defined(__AVX512BW__) && defined(__AVX512VL__) )
                const __mmask8 z = _mm256_cmpeq_epi32_mask(*(__m256i*)compare, compareSIMDValue);
#elif defined(__AVX2__)
                const __m256i cmp = _mm256_cmpeq_epi32(*(__m256i*)compare, compareSIMDValue);
                const int z = _mm256_movemask_ps(*(__m256*)(&cmp));
#endif
                if (z)
                {
                    const int first_1_pos = psnip_builtin_ffs(*(int*)&z) - 1;
                    return beginIter + (((uintptr_t)compare - (uintptr_t)begin) >> 2) + first_1_pos;
                }

                compare += 8;
            }
        }
        else if FAST_FIND_SIMD_CONSTEXPR(sizeof(iterator_value_type) == 8)
        {
            while (compare + 2 <= end)
            {
                const __m256i compareSIMDValue = _mm256_set1_epi64x(*(long long*)(&value));

#if ( defined(__AVX512BW__) && defined(__AVX512VL__) )
                const __mmask8 z = _mm256_cmpeq_epi64_mask(*(__m256i*)compare, compareSIMDValue);
#elif defined(__AVX2__)
                const __m256i cmp = _mm256_cmpeq_epi64(*(__m256i*)compare, compareSIMDValue);
                const int z = _mm256_movemask_pd(*(__m256d*)(&cmp));
#endif
                if (z)
                {
                    const int first_1_pos = psnip_builtin_ffs(*(int*)&z) - 1;
                    return beginIter + (((uintptr_t)compare - (uintptr_t)begin) >> 3) + first_1_pos;
                }

                compare += 4;
            }
        }

        //scalar compare tails
        while (compare != end)
        {
            if (*compare == value)
            {
                return beginIter + (((uintptr_t)compare - (uintptr_t)begin) / sizeof(iterator_value_type)) + (((uintptr_t)compare - (uintptr_t)begin) % sizeof(iterator_value_type));
            }
            compare++;
        }

        return endIter;

    }    
}

#undef PSNIP_BUILTIN_H
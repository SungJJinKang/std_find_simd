#include "../std_vector_find_simd.h"
#include <cassert>
#include <numeric>
int main()
{
	{
		std::vector<char> a;
		
		a.reserve(128);
		for (char i = 0; i <= 126; i++)
		{
			a.push_back(i);
		}


		for(char i = 0 ; i <= 126 ; i++)
		{
			auto iter = fast_vector_find::find_simd(a, (char)i);
			assert(iter == a.begin() + i);
		}
	}

	{
		std::vector<long long> a;
		a.reserve(10000001);
		for (unsigned long long i = 0; i <= 10000000; i++)
		{
			a.push_back(i);
		}


		{
			auto iter = fast_vector_find::find_simd(a, (long long)0);
			assert(iter == a.begin() + 0);
		}

		{
			auto iter = fast_vector_find::find_simd(a, (long long)1);
			assert(iter == a.begin() + 1);
		}


		for(long long i = 500000 ; i < 500050 ; i++)
		{
			auto iter = fast_vector_find::find_simd(a, (long long)i);
			assert(iter == a.begin() + i);
		}


		for (long long i = 0; i < 2000; i++)
		{
			auto iter = fast_vector_find::find_simd(a, (long long)i);
			assert(iter == a.begin() + i);
		}

		for (long long i = 10000000 - 500; i < 10000000; i++)
		{
			auto iter = fast_vector_find::find_simd(a, (long long)i);
			assert(iter == a.begin() + i);
		}

	}

	{
		std::vector<unsigned long long> a;
		a.reserve(11112351 + 1);
		for (unsigned long long i = 0; i <= 11112351; i++)
		{
			a.push_back(i);
		}


		{
			auto iter = fast_vector_find::find_simd(a, (unsigned long long)0);
			assert(iter == a.begin() + 0);
		}

		{
			auto iter = fast_vector_find::find_simd(a, (unsigned long long)1);
			assert(iter == a.begin() + 1);
		}


		{

			auto iter = fast_vector_find::find_simd(a, (unsigned long long)500001);
			assert(iter == a.begin() + 500001);
		}


		{

			auto iter = fast_vector_find::find_simd(a, (unsigned long long)500002);
			assert(iter == a.begin() + 500002);
		}

		{

			auto iter = fast_vector_find::find_simd(a, (unsigned long long)500003);
			assert(iter == a.begin() + 500003);
		}

		{

			auto iter = fast_vector_find::find_simd(a, (unsigned long long)10000000);
			assert(iter == a.begin() + 10000000);
		}

		{

			auto iter = fast_vector_find::find_simd(a, (unsigned long long)10000001000);
			assert(iter == a.end());
		}
	}


	///
	///
	///
	///
	///

	{
		std::vector<short> a;
		a.reserve(20000);
		for (short i = 0; i <= 20000; i++)
		{
			a.push_back(i);
		}

		for (short i = 20000 - 500; i < 20000; i++)
		{
			auto iter = fast_vector_find::find_simd(a, (short)i);
			assert(iter == a.begin() + i);
		}

		for (short i = 0; i < 500; i++)
		{
			auto iter = fast_vector_find::find_simd(a, (short)i);
			assert(iter == a.begin() + i);
		}

		{
			auto iter = fast_vector_find::find_simd(a, (short)0);
			assert(iter == a.begin() + 0);
		}

		{
			auto iter = fast_vector_find::find_simd(a, (short)1);
			assert(iter == a.begin() + 1);
		}


		{

			auto iter = fast_vector_find::find_simd(a, (short)9999);
			assert(iter == a.begin() + 9999);
		}


		{
			auto iter = fast_vector_find::find_simd(a, (short)9000);
			assert(iter == a.begin() + 9000);
		}

		{
			auto iter = fast_vector_find::find_simd(a, (short)9001);
			assert(iter == a.begin() + 9001);
		}

		{
			auto iter = fast_vector_find::find_simd(a, (short)9002);
			assert(iter == a.begin() + 9002);
		}

		{
			auto iter = fast_vector_find::find_simd(a, (short)9003);
			assert(iter == a.begin() + 9003);
		}

		{
			auto iter = fast_vector_find::find_simd(a, (short)9004);
			assert(iter == a.begin() + 9004);
		}

		{
			auto iter = fast_vector_find::find_simd(a, (short)9005);
			assert(iter == a.begin() + 9005);
		}

		{
			auto iter = fast_vector_find::find_simd(a, (short)9006);
			assert(iter == a.begin() + 9006);
		}

		{
			auto iter = fast_vector_find::find_simd(a, (short)9007);
			assert(iter == a.begin() + 9007);
		}

		for(short i = 9008 ; i < 10100 ; i++)
		{
			auto iter = fast_vector_find::find_simd(a, (short)i);
			assert(iter == a.begin() + i);
		}

		{

			auto iter = fast_vector_find::find_simd(a, (short)5000);
			assert(iter == a.begin() + 5000);
		}

		{

			auto iter = fast_vector_find::find_simd(a, (short)2);
			assert(iter == a.begin() + 2);
		}

		{

			auto iter = fast_vector_find::find_simd(a, std::numeric_limits<short>::max());
			assert(iter == a.end());
		}
	}

	{
		std::vector<int> a;
		a.reserve(1000000);
		for (int i = 0; i <= 1000000; i++)
		{
			a.push_back(i);
		}


		{
			auto iter = fast_vector_find::find_simd(a, (int)0);
			assert(iter == a.begin() + 0);
		}

		{
			auto iter = fast_vector_find::find_simd(a, (int)1);
			assert(iter == a.begin() + 1);
		}

		{

			auto iter = fast_vector_find::find_simd(a, (int)10000);
			assert(iter == a.begin() + 10000);
		}

		{

			auto iter = fast_vector_find::find_simd(a, (int)10001);
			assert(iter == a.begin() + 10001);
		}

		{

			auto iter = fast_vector_find::find_simd(a, (int)10002);
			assert(iter == a.begin() + 10002);
		}

		{

			auto iter = fast_vector_find::find_simd(a, (int)10003);
			assert(iter == a.begin() + 10003);
		}
	

		for (int i = 0; i < 100; i++)
		{
			auto iter = fast_vector_find::find_simd(a, (int)i);
			assert(iter == a.begin() + i);
		}
		for(int i = 9000 ; i < 9100 ; i++)
		{
			auto iter = fast_vector_find::find_simd(a, (int)i);
			assert(iter == a.begin() + i);
		}

	

		{

			auto iter = fast_vector_find::find_simd(a, (int)9000);
			assert(iter == a.begin() + 9000);
		}

		{

			auto iter = fast_vector_find::find_simd(a, (int)5000);
			assert(iter == a.begin() + 5000);
		}

		{

			auto iter = fast_vector_find::find_simd(a, (int)2);
			assert(iter == a.begin() + 2);
		}

		{

			auto iter = fast_vector_find::find_simd(a, std::numeric_limits<int>::max());
			assert(iter == a.end());
		}
	}


	
	{
		std::vector<char> a;
		a.reserve(128);
		for (char i = 0; i <= 126; i++)
		{
			a.push_back(i);
		}


		{
			auto iter = fast_vector_find::find_simd(a, (char)0);
			assert(iter == a.begin() + 0);
		}

		{
			auto iter = fast_vector_find::find_simd(a, (char)1);
			assert(iter == a.begin() + 1);
		}


		{

			auto iter = fast_vector_find::find_simd(a, (char)29);
			assert(iter == a.begin() + 29);
		}


		{

			auto iter = fast_vector_find::find_simd(a, (char)2);
			assert(iter == a.begin() + 2);
		}

	
		{

			auto iter = fast_vector_find::find_simd(a, (char)30);
			assert(iter == a.begin() + 30);
		}

		{

			auto iter = fast_vector_find::find_simd(a, (char)126);
			assert(iter == a.begin() + 126);
		}
	}
}
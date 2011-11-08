#include <ut/tee.h>

#include <ut/assert.h>
#include <ut/test.h>

namespace ut
{
	namespace tests
	{
		class A	{	};
		class B	{	};
		class C	{	};

		begin_test_suite(MiscTests)
			test(TypeInfosAreSelfEquivivalent)
			{
				// ACT / ASSERT
				is_false(typeid(A).before(typeid(A)));
				is_false(typeid(B).before(typeid(B)));
				is_false(typeid(C).before(typeid(C)));
			}


			test(TypeInfosAreTotallyOrdered)
			{
				// ACT / ASSERT
				is_true(typeid(A).before(typeid(B)) ^ typeid(B).before(typeid(A)));
				is_true(typeid(B).before(typeid(C)) ^ typeid(C).before(typeid(B)));
				is_true(typeid(A).before(typeid(C)) ^ typeid(C).before(typeid(A)));
				is_true(
					(typeid(A).before(typeid(B)) && typeid(B).before(typeid(C)) && typeid(A).before(typeid(C)))
					^ (typeid(B).before(typeid(C)) && typeid(C).before(typeid(A)) && typeid(B).before(typeid(A)))
					^ (typeid(C).before(typeid(A)) && typeid(A).before(typeid(B)) && typeid(C).before(typeid(B)))
				);
			}


			test(TypeInfoLessPredicateIsEquivivalentToTypeInfoBefore)
			{
				// ACT / ASSERT
				are_equal(type_info_less()(&typeid(A), &typeid(B)), typeid(A).before(typeid(B)));
				are_equal(type_info_less()(&typeid(B), &typeid(A)), typeid(B).before(typeid(A)));
				are_equal(type_info_less()(&typeid(A), &typeid(C)), typeid(A).before(typeid(C)));
			}
		end_test_suite
	}
}

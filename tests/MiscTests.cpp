#include <ut/registry.h>

#include <ut/assert.h>
#include <ut/test.h>

namespace ut
{
	namespace tests
	{
		namespace
		{
			class A	{	};
			class B	{	};
			class C	{	};
			class D : public A	{	};
			class E : public D	{	};
		}

		begin_test_suite( MiscTests )
			test( TypeInfosAreSelfEquivivalent )
			{
				// ACT / ASSERT
				assert_is_false(!!typeid(A).before(typeid(A)));
				assert_is_false(!!typeid(B).before(typeid(B)));
				assert_is_false(!!typeid(C).before(typeid(C)));
			}


			test( TypeInfosAreTotallyOrdered )
			{
				// ACT / ASSERT
				assert_is_true(!!(typeid(A).before(typeid(B)) ^ typeid(B).before(typeid(A))));
				assert_is_true(!!(typeid(B).before(typeid(C)) ^ typeid(C).before(typeid(B))));
				assert_is_true(!!(typeid(A).before(typeid(C)) ^ typeid(C).before(typeid(A))));
				assert_is_true(
					(typeid(A).before(typeid(B)) && typeid(B).before(typeid(C)) && typeid(A).before(typeid(C)))
					^ (typeid(B).before(typeid(C)) && typeid(C).before(typeid(A)) && typeid(B).before(typeid(A)))
					^ (typeid(C).before(typeid(A)) && typeid(A).before(typeid(B)) && typeid(C).before(typeid(B)))
				);
			}


			test( TypeInfoLessPredicateIsEquivivalentToTypeInfoBefore )
			{
				// ACT / ASSERT
				assert_equal(type_info_less()(&typeid(A), &typeid(B)), !!typeid(A).before(typeid(B)));
				assert_equal(type_info_less()(&typeid(B), &typeid(A)), !!typeid(B).before(typeid(A)));
				assert_equal(type_info_less()(&typeid(A), &typeid(C)), !!typeid(A).before(typeid(C)));
			}
		end_test_suite
	}
}

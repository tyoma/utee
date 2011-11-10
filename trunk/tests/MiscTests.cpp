#include <ut/sp.h>
#include <ut/tee.h>

#include <ut/assert.h>
#include <ut/test.h>

#include <memory>

namespace ut
{
	namespace tests
	{
		class A	{	};
		class B	{	};
		class C	{	};

		template <typename X>
		class DtorMarker
		{
			bool &_destructed;

			const DtorMarker &operator =(const DtorMarker &rhs);

		public:
			DtorMarker(bool &destructed)
				: _destructed(destructed)
			{	}

			virtual ~DtorMarker()
			{	_destructed = true;	}
		};


		typedef DtorMarker<A> DtorMarkerA;
		typedef DtorMarker<B> DtorMarkerB;


		begin_test_suite(MiscTests)
			test(TypeInfosAreSelfEquivivalent)
			{
				// ACT / ASSERT
				is_false(!!typeid(A).before(typeid(A)));
				is_false(!!typeid(B).before(typeid(B)));
				is_false(!!typeid(C).before(typeid(C)));
			}


			test(TypeInfosAreTotallyOrdered)
			{
				// ACT / ASSERT
				is_true(!!(typeid(A).before(typeid(B)) ^ typeid(B).before(typeid(A))));
				is_true(!!(typeid(B).before(typeid(C)) ^ typeid(C).before(typeid(B))));
				is_true(!!(typeid(A).before(typeid(C)) ^ typeid(C).before(typeid(A))));
				is_true(
					(typeid(A).before(typeid(B)) && typeid(B).before(typeid(C)) && typeid(A).before(typeid(C)))
					^ (typeid(B).before(typeid(C)) && typeid(C).before(typeid(A)) && typeid(B).before(typeid(A)))
					^ (typeid(C).before(typeid(A)) && typeid(A).before(typeid(B)) && typeid(C).before(typeid(B)))
				);
			}


			test(TypeInfoLessPredicateIsEquivivalentToTypeInfoBefore)
			{
				// ACT / ASSERT
				are_equal(type_info_less()(&typeid(A), &typeid(B)), !!typeid(A).before(typeid(B)));
				are_equal(type_info_less()(&typeid(B), &typeid(A)), !!typeid(B).before(typeid(A)));
				are_equal(type_info_less()(&typeid(A), &typeid(C)), !!typeid(A).before(typeid(C)));
			}


			test(ConstructDestructEmptySharedPtr)
			{
				// INIT / ACT / ASSERT (must not throw)
				shared_ptr<A> pa;
				shared_ptr<B> pb;
			}


			test(HoldingPtrInSharedPtrIsNotDesctrutible)
			{
				// INIT
				bool destroyed_a = false, destroyed_b = false;

				// ACT
				shared_ptr<DtorMarkerA> pa(new DtorMarkerA(destroyed_a));
				shared_ptr<DtorMarkerB> pb(new DtorMarkerB(destroyed_b));

				// ASSERT
				is_false(destroyed_a);
				is_false(destroyed_b);
			}


			test(DestructionOfASharedPtrDestroysUnderlyingObject)
			{
				// INIT
				bool destroyed_a = false, destroyed_b = false;

				{
					shared_ptr<DtorMarkerA> pa(new DtorMarkerA(destroyed_a));
					{
						shared_ptr<DtorMarkerB> pb(new DtorMarkerB(destroyed_b));

				// ACT (going out of scope)
					}

				// ASSERT
					is_false(destroyed_a);
					is_true(destroyed_b);

				// ACT (going out of scope)
				}

				// ASSERT
				is_true(destroyed_a);
				is_true(destroyed_b);
			}


			test(MakingACopyOfAnEmptySharedPtrDoesNotFail)
			{
				// INIT
				shared_ptr<A> pa;
				shared_ptr<B> pb;

				// ACT / ASSERT (must not fail)
				shared_ptr<A> copy_pa(pa);
				shared_ptr<B> copy_pb(pb);
			}


			test(MakingACopyOfAnEmptySharedPtrDoesNotFailAtCopyAndAtDestruction)
			{
				// INIT
				bool destroyed_a = false, destroyed_b = false;
				shared_ptr<DtorMarkerA> pa(new DtorMarkerA(destroyed_a));
				shared_ptr<DtorMarkerB> pb(new DtorMarkerB(destroyed_b));

				// ACT / ASSERT (must not fail)
				shared_ptr<DtorMarkerA> copy_pa(pa);
				shared_ptr<DtorMarkerB> copy_pb(pb);

				// ASSERT
				is_false(destroyed_a);
				is_false(destroyed_b);

				// ACT (going out of scope) / ASSERT (must not fail)
			}


			test(MakingACopyPreventsFromDestructionWhileCopyIsAlive2)
			{
				// INIT
				bool destroyed = false;
				std::auto_ptr< shared_ptr<DtorMarkerA> > ppa(new shared_ptr<DtorMarkerA>(new DtorMarkerA(destroyed)));

				// ACT
				{
					shared_ptr<DtorMarkerA> copy_pa(*ppa);

					ppa.reset();

				// ASSERT
					is_false(destroyed);

				// ACT (going out of scope)
				}

				// ASSERT
				is_true(destroyed);
			}


			test(MakingACopyPreventsFromDestructionWhileCopyIsAlive3)
			{
				// INIT
				bool destroyed = false;
				std::auto_ptr< shared_ptr<DtorMarkerA> > ppa(new shared_ptr<DtorMarkerA>(new DtorMarkerA(destroyed)));

				// ACT
				{
					shared_ptr<DtorMarkerA> copy_pa1(*ppa);
					{
						shared_ptr<DtorMarkerA> copy_pa2(copy_pa1);

						ppa.reset();

				// ASSERT
						is_false(destroyed);

				// ACT (going out of scope)
					}

				// ASSERT
					is_false(destroyed);

				// ACT (going out of scope)
				}

				// ASSERT
				is_true(destroyed);
			}


			test(DereferencingAndMemberSelectionWorks)
			{
				// INIT
				A *a = new A;
				B *b1 = new B, *b2 = new B;

				shared_ptr<A> pa(a);
				shared_ptr<B> pb1(b1), pb2(b2);

				// ACT / ASSERT
				are_equal(a, pa.operator ->());
				are_equal(b1, pb1.operator ->());
				are_equal(b2, pb2.operator ->());
				
				are_equal(a, &*pa);
				are_equal(b1, &*pb1);
				are_equal(b2, &*pb2);
			}


			test(AssigningToSelfDoesNothing)
			{
				// INIT
				bool destroyed_a = false, destroyed_b = false;
				shared_ptr<DtorMarkerA> pa(new DtorMarkerA(destroyed_a));
				shared_ptr<DtorMarkerB> pb(new DtorMarkerB(destroyed_b));

				// ACT / ASSERT (must not fail)
				pa = pa;
				pb = pb;

				// ASSERT
				is_false(destroyed_a);
				is_false(destroyed_b);
			}


			test(AssigningToSelfValueDoesNothing)
			{
				// INIT
				bool destroyed_a = false, destroyed_b = false;
				shared_ptr<DtorMarkerA> pa(new DtorMarkerA(destroyed_a)), copy_pa(pa);
				shared_ptr<DtorMarkerB> pb(new DtorMarkerB(destroyed_b)), copy_pb(pb);

				// ACT / ASSERT (must not fail)
				copy_pa = pa;
				copy_pb = pb;

				// ASSERT
				is_false(destroyed_a);
				is_false(destroyed_b);
			}


			test(AssigningReturnsSelf)
			{
				// INIT
				bool destroyed_a = false, destroyed_b = false;
				shared_ptr<DtorMarkerA> pa(new DtorMarkerA(destroyed_a));
				shared_ptr<DtorMarkerB> pb(new DtorMarkerB(destroyed_b));

				// ACT / ASSERT (must not fail)
				const shared_ptr<DtorMarkerA> *ppa = &(pa = pa);
				const shared_ptr<DtorMarkerB> *ppb = &(pb = pb);

				// ASSERT
				are_equal(&pa, ppa);
				are_equal(&pb, ppb);
			}


			test(UnderlyingObjectSurvivesIfSharedPtrAssigned)
			{
				// INIT
				bool destroyed_a = false, destroyed_b = false;
				shared_ptr<DtorMarkerA> pa(new DtorMarkerA(destroyed_a));
				std::auto_ptr< shared_ptr<DtorMarkerB> > ppb(new shared_ptr<DtorMarkerB>(new DtorMarkerB(destroyed_b)));
				std::auto_ptr< shared_ptr<DtorMarkerA> > copy_ppa(new shared_ptr<DtorMarkerA>());
				shared_ptr<DtorMarkerB> copy_pb;

				// ACT
				*copy_ppa = pa;
				copy_pb = *ppb;
				ppb.reset();
				copy_ppa.reset();

				// ASSERT
				is_false(destroyed_a);
				is_false(destroyed_b);
			}


			test(PreviouslyHeldObjectIsDestroyedAtAssign)
			{
				// INIT
				bool destroyed_1 = false, destroyed_2 = false;
				shared_ptr<DtorMarkerA> pa1(new DtorMarkerA(destroyed_1)), pa2(new DtorMarkerA(destroyed_2));

				// ACT
				pa1 = pa2;

				// ASSERT
				is_true(destroyed_1);
				is_false(destroyed_2);
			}


			test(PreviouslyHeldObjectIsKeptAtAssignIfAnotherSharedPtrPointedAtIt)
			{
				// INIT
				bool destroyed_1 = false, destroyed_2 = false;
				shared_ptr<DtorMarkerA> pa1(new DtorMarkerA(destroyed_1)), copy_pa1(pa1), pa2(new DtorMarkerA(destroyed_2));

				// ACT
				pa1 = pa2;

				// ASSERT
				is_false(destroyed_1);
				is_false(destroyed_2);
			}
		end_test_suite
	}
}


// Glossary (xUnit terminology)
//
// Test fixture (aka test context) - a set of preconditions or state needed to run a test. The developer should
//	set up a known good state before the tests, and after the tests return to the original state.
//
// Test suite - a set of tests that all share the same fixture. The order of the tests execution shouldn't matter.
//
// Test execution - an execution of an individual unit test that proceeds as follows:
//	1. initializers(); // First, we should prepare our 'world' to make an isolated environment for testing
//	2. <body of a test> // Here we make all the tests
//	3. teardown(); // In the end, whether succeed or fail we should clean up our 'world' to not disturb other
//		tests or code
//
// Sample test file (cpp, no need in a separate header):
//
// #include <ut/test.h>
//
//	begin_test_suite( AcmeTests )
//		init( AcmeTestsInit )
//		{
//			// Fuel up our rocket
//		}
//		
//		teardown( AcmeTestsCleanup )
//		{
//			// Drain the fuel, assert if anything goes wrong
//		}
//		
//		
//		test( LaunchTheRocket )
//		{
//			// Test if the rocket can be launched
//		}
//		
//		
//		test( LaunchAndCheckRocketGauges )
//		{
//			// Check that the rocket gauges are showing that we are in flight
//		}
//	end_test_suite

#ifndef __UTEE_UT_TEST__
#define __UTEE_UT_TEST__

#ifdef USE_MSVCQT

	#define begin_test_suite(__test_suite)\
	[Microsoft::VisualStudio::TestTools::UnitTesting::TestClass]\
	public ref class __test_suite\
	{\
	public:

	#define init(__test_init)\
		[Microsoft::VisualStudio::TestTools::UnitTesting::TestInitialize]\
		void __test_init()

	#define teardown(__test_tdwn)\
		[Microsoft::VisualStudio::TestTools::UnitTesting::TestCleanup]\
		void __test_tdwn()

	#define test(__test)\
		[Microsoft::VisualStudio::TestTools::UnitTesting::TestMethod]\
		void __test()

	#define ignored_test(__test)\
		void __test()

	#define obsolete_test(__test)

	#define end_test_suite	};

#else	// USE_MSVCQT

	#include "registry.h"

	#define __tokenliteral(x) #x
	#define __tokenliteral2(x) __tokenliteral(x)
	#define __tokenpaste(x, y) x ## y
	#define __tokenpaste2(x, y) __tokenpaste(x, y)

	namespace ut
	{
		registry &registry_instance();

		/// @brief Automatic registrar class for an initalization method.
		template <class FixtureT, void (FixtureT::*metadata_provider())()>
		struct test_init_registrar
		{
			test_init_registrar()
			{
				registry_instance().add_init<FixtureT>(metadata_provider());
			}
		};


		/// @brief Automatic registrar class for a cleanup method.
		template <class FixtureT, void (FixtureT::*metadata_provider())()>
		struct test_teardown_registrar
		{
			test_teardown_registrar()
			{
				registry_instance().add_teardown<FixtureT>(metadata_provider());
			}
		};


		/// @brief Automatic registrar class for a test method.
		template <class FixtureT, void (FixtureT::*metadata_provider(const char *&))()>
		struct test_case_registrar
		{
			test_case_registrar()
			{
				typedef void (FixtureT::*test_case_method_t)();

				const char *name = 0;
				test_case_method_t method = metadata_provider(name);

				registry_instance().add_test<FixtureT>(method, name);
			}
		};
	}

	/// @brief The set of macros to enable tests notation as described above.
	#define begin_test_suite(__test_suite)\
	class __test_suite\
	{\
		typedef __test_suite this_suite_class;\
	public:\
		static const char *__suite_name()\
		{	return #__test_suite;	}\
		static std::string __suite_id()\
		{	return std::string(__FILE__) + std::string(__tokenliteral2(__LINE__)) + std::string(__tokenliteral2(__test_suite));	}

	#define init(__test_init)\
		static void (this_suite_class::*__##__test_init##_meta())()\
		{	return &this_suite_class::__test_init;	}\
		ut::test_init_registrar<this_suite_class, &this_suite_class::__##__test_init##_meta> __##__test_init##_registrar;\
		void __test_init()

	#define teardown(__test_tdwn)\
		static void (this_suite_class::*__##__test_tdwn##_meta())()\
		{	return &this_suite_class::__test_tdwn;	}\
		ut::test_teardown_registrar<this_suite_class, &this_suite_class::__##__test_tdwn##_meta> __##__test_tdwn##_registrar;\
		void __test_tdwn()

	#define test(__test)\
		static void (this_suite_class::*__##__test##_meta(const char *&name))()\
		{	return name = #__test, &this_suite_class::__test;	}\
		ut::test_case_registrar<this_suite_class, &this_suite_class::__##__test##_meta> __##__test##_registrar;\
		void __test()

	#define ignored_test(__test)\
		void __test()

	#define obsolete_test(__test)

	#define end_test_suite\
	 } static __tokenpaste2(g_suite, __LINE__);

#endif	// USE_MSVCQT

#endif	// __UTEE_UT_TEST__

#pragma once

#include "test_case.h"

#include <functional>
#include <map>
#include <set>
#include <string>
#include <vector>

namespace ut
{
	/// @brief This is a central registry for unit tests based on this famework. It supports adding non-static test
	///	methods from fixtures (tests cannot go outside of any fixture), as well as initialization and teardown
	///	methods. For a definition of a 'fixture', please, refer to http://en.wikipedia.org/wiki/Test_fixture
	class registry
	{
	public:
		typedef std::vector< shared_ptr<test_case> >::const_iterator const_iterator;

	public:
		registry();

		/// @brief Registers a test cases which can be run on properly initialized/terminated instance of a fixture.
		template <typename FixtureT>
		void add_test(void (FixtureT::*method)(), const char *name);

		/// @brief Registers an initializer method. All registered initializers will be called for an instance of
		///	a fixture prior each single test.
		template <typename FixtureT>
		void add_init(void (FixtureT::*method)());

		/// @brief Registers a teardown method. All registered terminators will be called for an instance of a fixture
		///	after each single test. Terminators can throw exceptions/assertions in the case cleanup cannot be done.
		template <typename FixtureT>
		void add_teardown(void (FixtureT::*method)());

		/// @brief This group of methods provides access to the test cases registered. Dereferencing an iterator gives
		///	shared_ptr to a test case interface (@see test_case).
		int tests_count() const;
		const_iterator tests_begin() const;
		const_iterator tests_end() const;

	private:
		typedef std::map<std::string /*suite_id*/, shared_ptr<destructible> /*setup*/> _setups_map_t;

	private:
		template <typename FixtureT>
		shared_ptr< setup_impl<FixtureT> > get_setup();

	private:
		std::vector< shared_ptr<test_case> > _test_cases;
		std::set<std::string> _registered_names;
		_setups_map_t _setups;
	};



	template <typename FixtureT>
	inline void registry::add_test(void (FixtureT::*method)(), const char *name)
	{
		typedef test_case_impl<FixtureT> test_case;
		typedef setup_impl<FixtureT> fixture_setup;

		shared_ptr<fixture_setup> setup(get_setup<FixtureT>());
		shared_ptr<test_case> tc(new test_case_impl<FixtureT>(method, name, setup));

		if (_registered_names.insert(std::string(tc->fixture_name().c_str()) + tc->name().c_str()).second)
			_test_cases.push_back(tc);
	}

	template <typename FixtureT>
	inline void registry::add_init(void (FixtureT::*method)())
	{
		get_setup<FixtureT>()->add_init_method(method);
	}

	template <typename FixtureT>
	inline void registry::add_teardown(void (FixtureT::*method)())
	{
		get_setup<FixtureT>()->add_teardown_method(method);
	}

	template <typename FixtureT>
	inline shared_ptr< setup_impl< FixtureT > > registry::get_setup()
	{
		typedef setup_impl<FixtureT> fixture_setup;

		const std::string id = FixtureT::__suite_id();
		_setups_map_t::const_iterator s = _setups.find(id);

		s = s == _setups.end() ?
			_setups.insert(std::make_pair(id, shared_ptr<destructible>(new fixture_setup))).first : s;
		return shared_ptr<fixture_setup>(static_pointer_cast< fixture_setup >(s->second));
	}
}

typedef void (*AcceptTestF)(void *i_parameter, ut::test_case *i_test);
typedef void (*EnumerateTestsF)(void *i_parameter, AcceptTestF i_acceptor);

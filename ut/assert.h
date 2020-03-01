#pragma once

#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>

namespace ut
{
	class LocationInfo
	{
	public:
		LocationInfo(const std::string &filename_, int line_);

		const std::string filename;
		const int line;

	private:
		const LocationInfo &operator =(const LocationInfo &rhs);
	};

	class FailedAssertion : public std::logic_error
	{
	public:
		FailedAssertion(const std::string &message, const LocationInfo &location_);
		virtual ~FailedAssertion() throw();

		LocationInfo location;

	private:
		static std::string ComposeMessage(const std::string &message, const LocationInfo &location_);
	};


	template <typename T>
	struct remove_const
	{	typedef T type;	};

	template <typename T>
	struct remove_const<const T>
	{	typedef typename remove_const<T>::type type;	};

	template <typename T1, typename T2>
	struct remove_const< std::pair<T1, T2> >
	{	typedef std::pair<typename remove_const<T1>::type, typename remove_const<T2>::type> type;	};


	template <typename T1, typename T2>
	inline void are_equal(const T1 &expected, const T2 &actual, const LocationInfo &location)
	{
		if (!(expected == actual))
			throw FailedAssertion("Values are not equal!", location);
	}

	template <typename T, size_t n, typename ContainerT>
	inline void are_equal(T (&expected)[n], const ContainerT &actual, const LocationInfo &location)
	{
		are_equal(n, static_cast<size_t>(std::distance(actual.begin(), actual.end())), location);
		is_true(std::equal(expected, expected + n, actual.begin()), location);
	}

	template <typename T, size_t n, typename ContainerT, typename PredicateT>
	inline void are_equal(T (&expected)[n], const ContainerT &actual, PredicateT predicate, const LocationInfo &location)
	{
		are_equal(n, static_cast<size_t>(std::distance(actual.begin(), actual.end())), location);
		is_true(std::equal(expected, expected + n, actual.begin(), predicate), location);
	}

	template <typename T, size_t n>
	inline void are_equal(const T (&expected)[n], const std::basic_string<T> &actual, const LocationInfo &location)
	{
		if (!(expected == actual))
			throw FailedAssertion("Values are not equal!", location);
	}

	template <typename T1, typename T2, size_t n>
	inline void are_equal(T1 (&expected)[n], T2 (&actual)[n], const LocationInfo &location)
	{	is_true(std::equal(expected, expected + n, actual), location);	}

	template <typename T1, typename T2, size_t n, typename PredicateT>
	inline void are_equal(T1 (&expected)[n], T2 (&actual)[n], PredicateT predicate, const LocationInfo &location)
	{	is_true(std::equal(expected, expected + n, actual, predicate), location);	}

	template <typename T>
	inline void are_approx_equal(T reference, T actual, double tolerance, const LocationInfo &location)
	{
		const double e = 2 * static_cast<double>(reference - actual) / static_cast<double>(reference + actual);

		if (e < -tolerance || tolerance < e)
			throw FailedAssertion("Values are not approximately equal!", location);
	}


	template <typename T1, size_t n, typename T2>
	inline void content_equal(T1 (&expected)[n], const T2 &actual, const LocationInfo &location)
	{	are_equal(std::vector<typename T2::value_type>(expected, expected + n), actual, location);	}

	template <typename T1, typename T2, size_t n>
	inline void content_equal(T1 (&expected)[n], T2 (&actual)[n], const LocationInfo &location)
	{	are_equal(std::vector<T2>(expected, expected + n), std::vector<T2>(actual, actual + n), location);	}

	template <typename T1, typename T2>
	inline void are_not_equal(const T1 &expected, const T2 &actual, const LocationInfo &location)
	{
		if (!(expected != actual))
			throw FailedAssertion("Values are equal!", location);
	}

	template <typename T1, typename T2>
	inline void are_equivalent(const T1& expected, const T2& actual, const LocationInfo &location)
	{
		using namespace std;

		vector<typename ut::remove_const<typename T1::value_type>::type> reference(expected.begin(), expected.end());
		vector<typename ut::remove_const<typename T2::value_type>::type> actual_(actual.begin(), actual.end());

		sort(reference.begin(), reference.end());
		sort(actual_.begin(), actual_.end());
		if (lexicographical_compare(reference.begin(), reference.end(), actual_.begin(), actual_.end())
			!= lexicographical_compare(actual_.begin(), actual_.end(), reference.begin(), reference.end()))
			throw FailedAssertion("The sets are not equivalent!", location);
	}

	template <typename T1, size_t n, typename T2>
	inline void are_equivalent(T1 (&expected)[n], const T2& actual, const LocationInfo &location)
	{	are_equivalent(std::vector<T1>(expected, expected + n), actual, location);	}

	inline void is_true(bool value, const LocationInfo &location)
	{
		if (!value)
			throw FailedAssertion("Value is not 'true'!", location);
	}

	inline void is_false(bool value, const LocationInfo &location)
	{
		if (value)
			throw FailedAssertion("Value is not 'false'!", location);
	}

	template <typename T>
	inline void is_empty(const T& container, const LocationInfo &location)
	{
		if (!container.empty())
			throw FailedAssertion("The container is not empty!", location);
	}

	template <typename T>
	inline void is_null(const T &value, const LocationInfo &location)
	{
		if (!(value == NULL))
			throw FailedAssertion("Value is not null!", location);
	}

	template <typename T>
	inline void is_not_null(const T &value, const LocationInfo &location)
	{
		if (!(value != NULL))
			throw FailedAssertion("Value is null!", location);
	}
}

#define assert_equal(_mp_expected, _mp_actual)  ut::are_equal(_mp_expected, _mp_actual, ut::LocationInfo(__FILE__, __LINE__))
#define assert_equal_pred(_mp_expected, _mp_actual, _mp_pedicate)  ut::are_equal(_mp_expected, _mp_actual, _mp_pedicate, ut::LocationInfo(__FILE__, __LINE__))
#define assert_approx_equal(_mp_expected, _mp_actual, _mp_tolerance) ut::are_approx_equal(_mp_expected, _mp_actual, _mp_tolerance, ut::LocationInfo(__FILE__, __LINE__))
#define assert_content_equal(_mp_expected, _mp_actual)  ut::content_equal(_mp_expected, _mp_actual, ut::LocationInfo(__FILE__, __LINE__))
#define assert_equivalent(_mp_expected, _mp_actual)  ut::are_equivalent(_mp_expected, _mp_actual, ut::LocationInfo(__FILE__, __LINE__))
#define assert_not_equal(_mp_expected, _mp_actual)  ut::are_not_equal(_mp_expected, _mp_actual, ut::LocationInfo(__FILE__, __LINE__))
#define assert_is_true(_mp_value)  ut::is_true(_mp_value, ut::LocationInfo(__FILE__, __LINE__))
#define assert_is_false(_mp_value)  ut::is_false(_mp_value, ut::LocationInfo(__FILE__, __LINE__))
#define assert_is_empty(_mp_container)  ut::is_empty(_mp_container, ut::LocationInfo(__FILE__, __LINE__))
#define assert_null(_mp_value) ut::is_null(_mp_value, ut::LocationInfo(__FILE__, __LINE__))
#define assert_not_null(_mp_value) ut::is_not_null(_mp_value, ut::LocationInfo(__FILE__, __LINE__))

#define assert_throws(_mp_fragment, expected_exception)\
	try { _mp_fragment; throw ut::FailedAssertion("Expected exception was not thrown!", ut::LocationInfo( __FILE__, __LINE__ )); }\
	catch (const ut::FailedAssertion &) { throw; }\
	catch (const expected_exception &) { } \
	catch (...) { throw ut::FailedAssertion("Exception of unexpected type was thrown!", ut::LocationInfo( __FILE__, __LINE__ )); }

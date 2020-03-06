#pragma once

#include <ostream>
#include <string>

namespace ut
{
	namespace exportable
	{
		template <typename CharT>
		class immutable_basic_string
		{
		public:
			typedef CharT value_type;

		public:
			immutable_basic_string(const std::basic_string<CharT>& from);
			immutable_basic_string(const immutable_basic_string& other);
			~immutable_basic_string();

			const CharT* c_str() const throw();

		private:
			typedef void (*DeleterF)(const CharT* buffer);

		private:
			const immutable_basic_string& operator =(const immutable_basic_string& rhs);

			void construct(const std::basic_string<CharT>& from);
			static void deallocate(const CharT* buffer);

			CharT* _text;
			DeleterF _deleter;
		};

		typedef immutable_basic_string<char> immutable_string;



		template <typename CharT>
		inline immutable_basic_string<CharT>::immutable_basic_string(const std::basic_string<CharT>& from)
		{	construct(from);	}

		template <typename CharT>
		inline immutable_basic_string<CharT>::immutable_basic_string(const immutable_basic_string& other)
		{	construct(other.c_str());	}

		template <typename CharT>
		inline immutable_basic_string<CharT>::~immutable_basic_string()
		{	_deleter(_text);	}

		template <typename CharT>
		inline const CharT* immutable_basic_string<CharT>::c_str() const throw()
		{	return _text;	}

		template <typename CharT>
		inline void immutable_basic_string<CharT>::construct(const std::basic_string<CharT>& from)
		{
			_text = new CharT[from.size() + 1];
			std::copy(from.begin(), from.end(), _text);
			_text[from.size()] = CharT();
			_deleter = &deallocate;
		}

		template <typename CharT>
		inline void immutable_basic_string<CharT>::deallocate(const CharT* buffer)
		{	delete[] buffer;	}
	}
}

template <typename CharT>
inline std::basic_ostream<CharT>& operator <<(std::basic_ostream<CharT>& i_ostream,
	const ut::exportable::immutable_basic_string<CharT>& i_string)
{	return i_ostream << i_string.c_str();	}

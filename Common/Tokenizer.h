
//##########################################################################
//Qlib 1.0
//开发者:曾全贵
//##########################################################################


#ifndef QLIB_TOKENIZER
#define QLIB_TOKENIZER

#include <list>
#include <string>

using namespace std;

namespace QLib
{

	template<class T>
	class CharSeperator
	{
	public:
		typedef T CharType;
	public:
		CharSeperator(T str[])
		{
			mSeperatorStr = str;
		}
		~CharSeperator()
		{

		}

	public:
		bool containsChar(T c)
		{
			return (mSeperatorStr.find(c) != basic_string<T>::npos);
		}

	protected:
		basic_string<T> mSeperatorStr;
	};

	template<class T>
	class Tokenizer
	{
	public:
		typedef typename list<basic_string<typename T::CharType>*>::iterator iterator;
	public:
		Tokenizer(basic_string<typename T::CharType> value, T seperator)
		{
			parse(value, seperator);
		}
		~Tokenizer()
		{
			for (iterator i = mListToken.begin(); i != mListToken.end(); i++)
			{
				delete *i;
			}
			mListToken.clear();
		}

	public:
		typename list<basic_string<typename T::CharType>*>::iterator begin()
		{
			return mListToken.begin();
		}
		typename list<basic_string<typename T::CharType>*>::iterator end()
		{
			return mListToken.end();
		}
		int getSize()
		{
			return mListToken.size();
		}


	protected:
		void parse(basic_string<typename T::CharType> value, T seperator)
		{
			basic_string<typename T::CharType> *newToken = new basic_string<typename T::CharType>();
			for (basic_string<typename T::CharType>::iterator i = value.begin(); i != value.end(); i++)
			{
				if (seperator.containsChar(*i))
				{
					mListToken.push_back(newToken);
					newToken = new basic_string<typename T::CharType>();
				}
				else
				{
					*newToken += *i;
				}
			}
			mListToken.push_back(newToken);
		}


	protected:
		list < basic_string<typename T::CharType>* > mListToken;
	};

}
#endif
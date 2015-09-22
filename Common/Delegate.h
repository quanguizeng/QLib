//##########################################################################
//Qlib 1.0
//ПЊЗЂеп:дјШЋЙѓ
//##########################################################################

/*
This file is part of Qlib.

Qlib is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Qlib is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef QLIB_DELEGATE
#define QLIB_DELEGATE

#include <list>
#include <typeinfo>

using namespace std;

template<typename TP1, typename TP2>
class IDelegate
{
public:
	virtual ~IDelegate() { }
	virtual bool isType(const std::type_info& _type) = 0;
	virtual void invoke(TP1 p1, TP2 p2) = 0;
	virtual bool compare(IDelegate<typename TP1, typename TP2> *_delegate) const = 0;
};

template<typename TP1, typename TP2>
class CStaticDelegate : public  IDelegate<typename TP1, typename TP2>
{
public:
	typedef void(*Func)(TP1 p1, TP2 p2);
	CStaticDelegate(Func _func) : mFunc(_func) { }
	virtual bool isType(const std::type_info& _type) { return typeid(CStaticDelegate<typename TP1, typename TP2>) == _type; }
	virtual void invoke(TP1 p1, TP2 p2)
	{
		mFunc(p1, p2);
	}
	virtual bool compare(IDelegate<typename TP1, typename TP2> *_delegate) const
	{
		if (0 == _delegate || !_delegate->isType(typeid(CStaticDelegate <typename TP1, typename TP2>))) return false;
		CStaticDelegate <typename TP1, typename TP2> * cast = static_cast<CStaticDelegate <typename TP1, typename TP2> *>(_delegate);
		return cast->mFunc == mFunc;
	}
private:
	Func mFunc;
};
template <typename T, typename TP1, typename TP2>
class CMethodDelegate : public  IDelegate <typename TP1, typename TP2>
{
public:
	typedef void (T::*Method)(TP1 p1, TP2 p2);
	CMethodDelegate(T * _object, Method _method) : mObject(_object), mMethod(_method) { }
	virtual bool isType(const std::type_info& _type) { return typeid(CMethodDelegate <T, TP1, TP2>) == _type; }
	virtual void invoke(TP1 p1, TP2 p2)
	{
		(mObject->*mMethod)(p1, p2);
	}
	virtual bool compare(IDelegate <typename TP1, typename TP2>  * _delegate) const
	{
		if (0 == _delegate || !_delegate->isType(typeid(CMethodDelegate <T, TP1, TP2>))) return false;
		CMethodDelegate <T, TP1, TP2>  * cast = static_cast<  CMethodDelegate <T, TP1, TP2>  * >(_delegate);
		return cast->mObject == mObject && cast->mMethod == mMethod;
	}
private:
	T * mObject;
	Method mMethod;
};
template   <typename TP1, typename TP2>
inline  IDelegate <typename TP1, typename TP2>  * createDelegate(void(*_func)(TP1 p1, TP2 p2))
{
	return new CStaticDelegate <typename TP1, typename TP2>(_func);
}
template <typename T, typename TP1, typename TP2>
inline  IDelegate <typename TP1, typename TP2>  * createDelegate(T * _object, void (T::*_method)(TP1 p1, TP2 p2))
{
	return new CMethodDelegate  <T, TP1, TP2>(_object, _method);
}
template   <typename TP1, typename TP2>
class CMultiDelegate
{
public:
	typedef IDelegate <typename TP1, typename TP2>  IMyDelegate;
	typedef typename std::list<IMyDelegate*> ListDelegate;
	typedef typename ListDelegate::iterator ListDelegateIterator;
	typedef typename ListDelegate::const_iterator ConstListDelegateIterator;
	CMultiDelegate() { }
	~CMultiDelegate() { clear(); }
	bool empty() const
	{
		for (ConstListDelegateIterator iter = mListDelegates.begin(); iter != mListDelegates.end(); ++iter)
		{
			if (*iter) return false;
		}
		return true;
	}
	void clear()
	{
		for (ListDelegateIterator iter = mListDelegates.begin(); iter != mListDelegates.end(); ++iter)
		{
			if (*iter)
			{
				delete (*iter);
				(*iter) = 0;
			}
		}
	}
	CMultiDelegate  <typename TP1, typename TP2> & operator+=(IMyDelegate* _delegate)
	{
		for (ListDelegateIterator iter = mListDelegates.begin(); iter != mListDelegates.end(); ++iter)
		{
			if ((*iter) && (*iter)->compare(_delegate))
			{
				delete _delegate;
				return *this;
			}
		}
		mListDelegates.push_back(_delegate);
		return *this;
	}
	CMultiDelegate  <typename TP1, typename TP2> & operator-=(IMyDelegate* _delegate)
	{
		for (ListDelegateIterator iter = mListDelegates.begin(); iter != mListDelegates.end(); ++iter)
		{
			if ((*iter) && (*iter)->compare(_delegate))
			{
				if ((*iter) != _delegate) delete (*iter);
				(*iter) = 0;
				break;
			}
		}
		delete _delegate;
		return *this;
	}
	void operator()(TP1 p1, TP2 p2)
	{
		ListDelegateIterator iter = mListDelegates.begin();
		while (iter != mListDelegates.end())
		{
			if (0 == (*iter))
			{
				iter = mListDelegates.erase(iter);
			}
			else
			{
				(*iter)->invoke(p1, p2);
				++iter;
			}
		}
	}
private:
	CMultiDelegate(const CMultiDelegate  <typename TP1, typename TP2> & _event);
	CMultiDelegate<typename TP1, typename TP2> & operator=(const CMultiDelegate<typename TP1, typename TP2> & _event);
private:
	ListDelegate mListDelegates;
};

#endif
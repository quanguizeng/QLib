#ifndef QLIB_REFLECTION
#define QLIB_REFLECTION

#include <string>    
#include <map>    
#include <iostream>

using namespace std;

namespace QLib
{
#define ClassFactoryInstance() ClassFactory::getInstance()
#define DEFINE_DYNAMIC_CREATE(ClassName) \
	static Register<ClassName> mMy##ClassName;

#define IMPLIMENT_DYNAMIC_CREATE(ClassName) \
	Register<ClassName> mMy##ClassName;

	typedef void* (*CreateFuntion)(void);

	class ClassFactory
	{
	protected:
		ClassFactory()
		{
		}
	public:
		~ClassFactory()
		{
			delete mMapClass;
			mMapClass = nullptr;
			mInstance = nullptr;
		}
	public:
		static ClassFactory * getInstance()
		{
			if (mInstance == nullptr)
			{
				mInstance = new ClassFactory;
			}

			return mInstance;
		}
		void* getClassByName(string name)
		{
			map<string, CreateFuntion>::const_iterator it;

			it = mMapClass->find(name);
			if (it == mMapClass->end())
			{
				return nullptr;
			}
			else
			{
				return it->second();
			}
		}
		void registClass(string name, CreateFuntion method)
		{
			mMapClass->insert(make_pair(name, method));
		}

	protected:

		static map<string, CreateFuntion> *mMapClass;
		static ClassFactory *mInstance;
	};

	class RegistyClass
	{
	public:
		RegistyClass(string name, CreateFuntion method)
		{
			ClassFactoryInstance()->registClass(name, method);
		}
	};

	template<typename T>
	class Register
	{
	public:
		Register()
		{
			RegistyClass registyClass(string(typeid(T).name()).substr(6), Register<T>::createInstance);
		}

		static void* createInstance()
		{
			return new T;
		}
	};
}

#endif
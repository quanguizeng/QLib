//##########################################################################
//Qlib 1.0
//开发者:曾全贵
//##########################################################################

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
	static RegistyClass<ClassName> mMy##ClassName;

#define IMPLIMENT_DYNAMIC_CREATE(ClassName) \
	RegistyClass<ClassName> mMy##ClassName;

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

	template<typename T>
	class RegistyClass
	{
	public:
		RegistyClass()
		{
			ClassFactoryInstance()->registClass(string(typeid(T).name()).substr(6), RegistyClass<T>::createInstance);
		}
		static void* createInstance()
		{
			return new T;
		}
	};


}

#endif
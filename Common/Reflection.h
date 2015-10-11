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
	private:							\
	static RegistryClass<ClassName> mMy##ClassName;

#define IMPLIMENT_DYNAMIC_CREATE(ClassName) \
	RegistryClass<ClassName> ClassName::mMy##ClassName;

	typedef void* (*CreateFunction)(void);

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
			map<string, CreateFunction>::const_iterator it;

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
		void registClass(string name, CreateFunction method)
		{
			mMapClass->insert(make_pair(name, method));
		}

	protected:

		static map<string, CreateFunction> *mMapClass;
		static ClassFactory *mInstance;
	};

	template<typename T>
	class RegistryClass
	{
	public:
		RegistryClass()
		{
			ClassFactoryInstance()->registClass(string(typeid(T).name()).substr(6), RegistryClass<T>::createInstance);
		}
		static void* createInstance()
		{
			return new T;
		}
	};

	class MmemberBase;
	typedef void(*setMemberValue)(MmemberBase *pObject, void*);
	typedef map<string, setMemberValue> SetMemberList;

	class MmemberBase
	{
	public:
		MmemberBase() {}
		virtual ~MmemberBase() {}
		virtual void registProperty() = NULL;
		inline SetMemberList &getPropertyList()
		{
			return mListProperty;
		}

		SetMemberList mListProperty;
	};

#define MEMBER_INTERFACE(ClassType, VarType, VarName)    \
public:                                       \
	inline static void set##VarName(MmemberBase *pBase, void* pValue)    \
	{\
	    ClassType* pSub = (ClassType*)pBase;      \
		pSub->VarName = *(VarType*)pValue;          \
	}                                         \
	inline static void get##VarName(MmemberBase *pBase, void* pValue)  \
	{\
	    ClassType* pSub = (ClassType*)pBase;      \
	    VarType* pLeft = (VarType*)pValue;      \
		*pLeft = pSub->VarName;          \
	}


}

#endif
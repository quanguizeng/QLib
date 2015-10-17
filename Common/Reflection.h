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

	static string removeSpace(string str)
	{
		int begin = 0;
		begin = str.find(" ", begin);
		while (begin != -1)
		{
			str.replace(begin, 1, "");
			begin = str.find(" ", begin);
		}
		return str;
	}

	class MmemberBase;
	typedef void(*PropertyInterface)(MmemberBase *pObject, void*);
	typedef map<string, PropertyInterface> SetMemberList;

#define SET_MEMEBER_NAME(Object, Name)	removeSpace((string("set")+string(#Name)))
#define GET_MEMEBER_NAME(Object, Name)	removeSpace((string("get")+string(#Name)))
#define SET_MEMBER_INTERFACE(Class, Name)	Class::set##Name
#define GET_MEMBER_INTERFACE(Class, Name)	Class::get##Name
#define ADD_SET_MEMBER_INTERFACE(Class, Object, Variable) addSetMemberInterface(SET_MEMEBER_NAME(Object, Variable), SET_MEMBER_INTERFACE(Class, Variable))
#define ADD_GET_MEMBER_INTERFACE(Class, Object, Variable) addGetMemberInterface(GET_MEMEBER_NAME(Object, Variable), GET_MEMBER_INTERFACE(Class, Variable))
#define _SET_MEMBER_VALUE(Object, VarLeft, VarRight) SET_MEMEBER_NAME(Object, VarLeft), Object, &VarRight
#define _GET_MEMBER_VALUE(Object, VarLeft, VarRight) GET_MEMEBER_NAME(Object, VarRight), Object, &VarLeft
#define SET_MEMBER_VALUE(Object, VarLeft, VarRight) (Object)->setMemberValue(_SET_MEMBER_VALUE(Object, VarLeft, VarRight))
#define GET_MEMBER_VALUE(Object, VarLeft, VarRight) (Object)->getMemberValue(_GET_MEMBER_VALUE(Object, VarLeft, VarRight))

	class MmemberBase
	{
			typedef SetMemberList::const_iterator PropertyIterator;
	public:
		MmemberBase() 
		{
			mGetListProperty.clear();
			mSetListProperty.clear();
		}
		virtual ~MmemberBase() 
		{
			mGetListProperty.clear();
			mSetListProperty.clear();
		}
		virtual void registProperty() = NULL;
		inline SetMemberList &getPropertyList()
		{
			return mGetListProperty;
		}
		inline SetMemberList &setPropertyList()
		{
			return mSetListProperty;
		}

		inline bool addSetMemberInterface(string memberName, PropertyInterface memberInterface)
		{
			PropertyIterator it = mSetListProperty.find(memberName);
			if (it != mSetListProperty.end())
			{
				return false;
			}
			mSetListProperty.insert(pair<string, PropertyInterface>(memberName, memberInterface));

			return true;
		}

		inline bool addGetMemberInterface(string memberName, PropertyInterface memberInterface)
		{
			PropertyIterator it = mGetListProperty.find(memberName);
			if (it != mGetListProperty.end())
			{
				return false;
			}
			mGetListProperty.insert(pair<string, PropertyInterface>(memberName, memberInterface));

			return true;
		}

		inline bool getMemberValue(string memberName, MmemberBase *pBase, void* pValue)
		{
			if (pBase == nullptr || pValue == nullptr)
			{
				return false;
			}
			PropertyIterator it = mGetListProperty.find(memberName);
			if (it == mGetListProperty.end())
			{
				return false;
			}
			mGetListProperty[memberName](pBase, pValue);

			return true;
		}

		inline bool setMemberValue(string memberName, MmemberBase *pBase, void* pValue)
		{
			if (pBase == nullptr || pValue == nullptr)
			{
				return false;
			}
			PropertyIterator it = mSetListProperty.find(memberName);
			if (it == mSetListProperty.end())
			{
				return false;
			}
			mSetListProperty[memberName](pBase, pValue);

			return true;
		}

		SetMemberList mSetListProperty;
		SetMemberList mGetListProperty;
	};

#define MEMBER_INTERFACE(ClassType, VarType, VarName)    \
public:                                       \
	inline static void set##VarName(MmemberBase *pBase, void* pValue)    \
	{\
	    ClassType* pSub = dynamic_cast<ClassType*>(pBase);      \
		pSub->VarName = *(VarType*)pValue;          \
	}                                         \
	inline static void get##VarName(MmemberBase *pBase, void* pValue)  \
	{\
	    ClassType* pSub = dynamic_cast<ClassType*>(pBase);      \
	    VarType* pLeft = (VarType*)pValue;      \
		*pLeft = pSub->VarName;          \
	}
}

#endif
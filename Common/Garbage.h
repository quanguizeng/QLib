
//##########################################################################
//Qlib 1.0
//开发者:曾全贵
//##########################################################################


#ifndef QLIB_GARBAGE
#define QLIB_GARBAGE

#include <set>

using namespace std;

namespace QLib
{
	template<typename C> class Garbage
	{
	public:
		Garbage()
		{}
		Garbage(C* item)
		{
			add(item);
		}
		inline void add(C* item)
		{
			if (item == NULL)
			{
				return;
			}
			mItems.insert(item);
		}

		inline void remove(C* item)
		{
			if (item == NULL)
			{
				return;
			}
			mItems.erase(item);
		}

		inline void destroy(C* item)
		{
			if (item == NULL)
			{
				return;
			}
			mItems.erase(item);
			delete item;
		}

		inline int size()
		{
			return mItems.size();
		}

		~Garbage()
		{
			typedef typename set<C*>::iterator iterator;
			for (iterator it = mItems.begin(); it != mItems.end(); ++it)
				delete *it;
			mItems.clear();
		}

	private:
		set<C*> mItems;
	};

}

#endif
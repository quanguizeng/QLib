
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
		inline void add(C* item)
		{
			if (item == NULL)
			{
				return;
			}
			m_items.insert(item);
		}

		inline void remove(C* item)
		{
			if (item == NULL)
			{
				return;
			}
			m_items.erase(item);
		}

		inline void destroy(C* item)
		{
			if (item == NULL)
			{
				return;
			}
			m_items.erase(item);
			delete item;
		}

		inline int size()
		{
			return m_items.size();
		}

		~Garbage()
		{
			typedef typename set<C*>::iterator iterator;
			for (iterator it = m_items.begin(); it != m_items.end(); ++it)
				delete *it;
			m_items.clear();
		}

	private:
		set<C*> m_items;
	};

}

#endif
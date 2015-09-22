
//##########################################################################
//Qlib 1.0
//开发者:曾全贵
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
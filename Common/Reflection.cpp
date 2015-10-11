
#include "Reflection.h"

namespace QLib
{
	map<string, CreateFunction> *ClassFactory::mMapClass = new map<string, CreateFunction>;
	ClassFactory *ClassFactory::mInstance = NULL;
}

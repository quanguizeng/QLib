
#include "Reflection.h"

namespace QLib
{
	map<string, CreateFuntion> *ClassFactory::mMapClass = new map<string, CreateFuntion>;
	ClassFactory *ClassFactory::mInstance = NULL;
}

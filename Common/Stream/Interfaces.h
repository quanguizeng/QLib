//##########################################################################
//Qlib 1.0
//开发者:曾全贵
//##########################################################################


#ifndef QLIB_INTERFACE
#define QLIB_INTERFACE

namespace QLib
{
	class IStream
	{
	public:
		virtual int	size() = 0;
		virtual bool	close() = 0;
		virtual bool isEnd() = 0;
		virtual int	read(void* pBuffer, int nSize) = 0;
		virtual int	write(void* pBuffer, int nSize) = 0;
	};
}

#endif
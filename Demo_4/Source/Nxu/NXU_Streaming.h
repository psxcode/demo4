#ifndef	NXU_STREAMING_H
#define	NXU_STREAMING_H

#include <Nxf.h>
#include <NxSimpleTypes.h>
#include "External/Code/PhysX/Stream.h"

#if DBS_NXU_FILESYSTEM == DBS_NXU_FILESYSTEM_NATIVE
#	include "NXU_File.h"
#elif DBS_NXU_FILESYSTEM == DBS_NXU_FILESYSTEM_OGRE
#	include "NXU_OgreDataStream.h"
#else
#	error
#endif

namespace	NXU
{

class	UserStream:	public NxStream
{
	 public:
#if DBS_NXU_FILESYSTEM == DBS_NXU_FILESYSTEM_NATIVE
		UserStream(const char	*filename, bool	load);
#elif DBS_NXU_FILESYSTEM == DBS_NXU_FILESYSTEM_OGRE
		 UserStream(Ogre::DataStreamPtr &stream, bool load);
#endif
		virtual	~UserStream();

		virtual	NxU8 readByte()const;
		virtual	NxU16	readWord()const;
		virtual	NxU32	readDword()const;
		virtual	float	readFloat()const;
		virtual	double readDouble()const;
		virtual	void readBuffer(void *buffer,	NxU32	size)const;

		virtual	NxStream &storeByte(NxU8 b);
		virtual	NxStream &storeWord(NxU16	w);
		virtual	NxStream &storeDword(NxU32 d);
		virtual	NxStream &storeFloat(NxReal	f);
		virtual	NxStream &storeDouble(NxF64	f);
		virtual	NxStream &storeBuffer(const	void *buffer,	NxU32	size);

#if DBS_NXU_FILESYSTEM == DBS_NXU_FILESYSTEM_NATIVE
		NXU_FILE *fp;
#elif DBS_NXU_FILESYSTEM == DBS_NXU_FILESYSTEM_OGRE
		Ogre::DataStreamPtr &mOgreStream;
#endif
};

class	MemoryWriteBuffer: public	NxStream
{
	 public:
		MemoryWriteBuffer();
		virtual	~MemoryWriteBuffer();
		void clear();

		virtual	NxU8 readByte()const
		{
			NX_ASSERT(0);
			return 0;
		}
		virtual	NxU16	readWord()const
		{
			NX_ASSERT(0);
			return 0;
		}
		virtual	NxU32	readDword()const
		{
			NX_ASSERT(0);
			return 0;
		}
		virtual	float	readFloat()const
		{
			NX_ASSERT(0);
			return 0.0f;
		}
		virtual	double readDouble()const
		{
			NX_ASSERT(0);
			return 0.0;
		}
		virtual	void readBuffer(void *,	NxU32)const
		{
			NX_ASSERT(0);
		}

		virtual	NxStream &storeByte(NxU8 b);
		virtual	NxStream &storeWord(NxU16	w);
		virtual	NxStream &storeDword(NxU32 d);
		virtual	NxStream &storeFloat(NxReal	f);
		virtual	NxStream &storeDouble(NxF64	f);
		virtual	NxStream &storeBuffer(const	void *buffer,	NxU32	size);

		NxU32	currentSize;
		NxU32	maxSize;
		NxU8 *data;
};

class	MemoryReadBuffer:	public NxStream
{
	 public:
		MemoryReadBuffer(const NxU8	*data);
		virtual	~MemoryReadBuffer();

		virtual	NxU8 readByte()const;
		virtual	NxU16	readWord()const;
		virtual	NxU32	readDword()const;
		virtual	float	readFloat()const;
		virtual	double readDouble()const;
		virtual	void readBuffer(void *buffer,	NxU32	size)const;

		virtual	NxStream &storeByte(NxU8)
		{
			NX_ASSERT(0);
			return	*this;
		}
		virtual	NxStream &storeWord(NxU16)
		{
			NX_ASSERT(0);
			return	*this;
		}
		virtual	NxStream &storeDword(NxU32)
		{
			NX_ASSERT(0);
			return	*this;
		}
		virtual	NxStream &storeFloat(NxReal)
		{
			NX_ASSERT(0);
			return	*this;
		}
		virtual	NxStream &storeDouble(NxF64)
		{
			NX_ASSERT(0);
			return	*this;
		}
		virtual	NxStream &storeBuffer(const	void*,	NxU32)
		{
			NX_ASSERT(0);
			return	*this;
		}

		mutable	const	NxU8 *buffer;
};

class	ImportMemoryWriteBuffer: public	NxStream
{
	public:
		ImportMemoryWriteBuffer();
		virtual	~ImportMemoryWriteBuffer();
		void clear();

		virtual	NxU8 readByte()const
		{
			NX_ASSERT(0);
			return 0;
		}
		virtual	NxU16	readWord()const
		{
			NX_ASSERT(0);
			return 0;
		}
		virtual	NxU32	readDword()const
		{
			NX_ASSERT(0);
			return 0;
		}
		virtual	float	readFloat()const
		{
			NX_ASSERT(0);
			return 0.0f;
		}
		virtual	double readDouble()const
		{
			NX_ASSERT(0);
			return 0.0;
		}
		virtual	void readBuffer(void *,	NxU32)const
		{
			NX_ASSERT(0);
		}

		virtual	NxStream &storeByte(NxU8 b);
		virtual	NxStream &storeWord(NxU16	w);
		virtual	NxStream &storeDword(NxU32 d);
		virtual	NxStream &storeFloat(NxReal	f);
		virtual	NxStream &storeDouble(NxF64	f);
		virtual	NxStream &storeBuffer(const	void *buffer,	NxU32	size);

		NxU32	currentSize;
		NxU32	maxSize;
		NxU8 *data;
};

class	ImportMemoryReadBuffer:	public NxStream
{
	public:
		ImportMemoryReadBuffer(const NxU8	*data);
		virtual	~ImportMemoryReadBuffer();

		virtual	NxU8 readByte()const;
		virtual	NxU16	readWord()const;
		virtual	NxU32	readDword()const;
		virtual	float	readFloat()const;
		virtual	double readDouble()const;
		virtual	void readBuffer(void *buffer,	NxU32	size)const;

		virtual	NxStream &storeByte(NxU8)
		{
			NX_ASSERT(0);
			return	*this;
		}
		virtual	NxStream &storeWord(NxU16)
		{
			NX_ASSERT(0);
			return	*this;
		}
		virtual	NxStream &storeDword(NxU32)
		{
			NX_ASSERT(0);
			return	*this;
		}
		virtual	NxStream &storeFloat(NxReal)
		{
			NX_ASSERT(0);
			return	*this;
		}
		virtual	NxStream &storeDouble(NxF64)
		{
			NX_ASSERT(0);
			return	*this;
		}
		virtual	NxStream &storeBuffer(const	void*,	NxU32)
		{
			NX_ASSERT(0);
			return	*this;
		}

		mutable	const	NxU8 *buffer;
};


}
#endif

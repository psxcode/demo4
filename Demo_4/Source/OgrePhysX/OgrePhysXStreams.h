#pragma once
#include "DemoPrerequisites.h"
#if DBS_LM_PHYSX

#include <OgreDataStream.h>
#include <NxPhysics.h>
#include <NxStream.h> 
#include <streambuf>
#include <OgreString.h>
#include "OgrePhysXClasses.h"

namespace Demo {
namespace OgrePhysX {
//------------------------------------------------------------
class OgreNxStream : public NxStream
{
public:
	OgreNxStream(Ogre::DataStreamPtr ds)
		: mOgreDataStream(ds)
	{
	}
	virtual ~OgreNxStream() {}

	virtual NxU8 readByte()const {
		NxU8 byte = 0;
		mOgreDataStream->read(&byte, sizeof(NxU8));
		return byte;
	}
	virtual NxU16 readWord()const {
		NxU16 word = 0;
		mOgreDataStream->read(&word, sizeof(NxU16));
		return word;
	}
	virtual NxU32 readDword()const {
		NxU32 dword = 0;
		mOgreDataStream->read(&dword, sizeof(NxU32));
		return dword;
	}
	virtual float readFloat()const {
		float f = 0;
		mOgreDataStream->read(&f, sizeof(float));
		return f;
	}
	virtual double readDouble()const {
		double d = 0;
		mOgreDataStream->read(&d, sizeof(double));
		return d;
	}
	virtual void readBuffer(void* buffer, NxU32 size)const {
		mOgreDataStream->read(buffer, size);
	}

	NxStream& storeByte(NxU8 b) {
		mOgreDataStream->write(&b, sizeof(b));
		return *this;
	}
	NxStream& storeWord(NxU16 w) { 
		mOgreDataStream->write(&w, sizeof(w));
		return *this;
	}
	NxStream& storeDword(NxU32 d) { 
		mOgreDataStream->write(&d, sizeof(d));
		return *this;
	}
	NxStream& storeFloat(NxReal f) {
		mOgreDataStream->write(&f, sizeof(f));
		return *this;
	}
	NxStream& storeDouble(NxF64 f) { 
		mOgreDataStream->write(&f, sizeof(f));
		return *this;
	}
	NxStream& storeBuffer(const void* buffer, NxU32 size) { 
		mOgreDataStream->write(buffer, size_t(size));
		return *this;
	}

private:
	Ogre::DataStreamPtr mOgreDataStream;
};
//------------------------------------------------------------
}
}
#endif
#pragma once
#include "DemoPrerequisites.h"
#if DBS_LM_PHYSX

#include <NxUserOutputStream.h>
#include <OgreStringConverter.h>
#include <OgreLogManager.h>

namespace Demo {
namespace OgrePhysX {
//------------------------------------------------------------
class LogOutputStream : public NxUserOutputStream
{
public:
	virtual ~LogOutputStream() {}
    virtual void reportError (NxErrorCode code, const char *message, const char* file, int line) {
		string_traits::str msg = "[OgrePhysX] Error in " + string_traits::str(file) + "line " + Ogre::StringConverter::toString(line) + ": " + string_traits::str(message);
#if	OGREPHYSX_CANMOVESTATICACTORS
		if (msg.find("Static actor moved") == string_traits::str::npos) Ogre::LogManager::getSingleton().logMessage(msg);
#else
		Ogre::LogManager::getSingleton().logMessage(msg);
#endif
    }
    virtual NxAssertResponse reportAssertViolation (const char *message, const char *file,int line) {
        //this should not get hit by
        // a properly debugged SDK!
        assert(0);
        return NX_AR_CONTINUE;
    }
    virtual void print (const char *message) {
		Ogre::LogManager::getSingleton().logMessage("[OgrePhysX] " + string_traits::str(message));
    }
};
//------------------------------------------------------------
}
}
#endif
#pragma once

#include <OgreDataStream.h>

namespace NXU {
//------------------------------------------------------------
static size_t nxu_fread( void *buffer, size_t size, int count, Ogre::DataStreamPtr &istream ) {
	size_t ret = 0;
	for( int i = 0; i < count; ++i )
		ret += istream->read( buffer, size );
	return ret;
}
//------------------------------------------------------------
static size_t nxu_fwrite( const void *buffer, size_t size, int count, Ogre::DataStreamPtr &ostream ) {
	size_t ret = 0;
	for( int i = 0; i < count; ++i )
		ret += ostream->write( buffer, size );
	return ret;
}
//------------------------------------------------------------
static void nxu_fseek( Ogre::DataStreamPtr &istream, long loc ) {
	istream->seek(loc);
}
//------------------------------------------------------------
static size_t nxu_ftell( Ogre::DataStreamPtr &istream ) {
	return istream->tell();
}
//------------------------------------------------------------
static size_t nxu_fputc( char c, Ogre::DataStreamPtr &ostream ) {
	return nxu_fwrite( &c, sizeof(c), 1, ostream );
}
//------------------------------------------------------------
static size_t nxu_fputs( const char *str, Ogre::DataStreamPtr &ostream ) {
	size_t ret = 0;
	if( str ) {
		size_t len = strlen(str);
		ret = nxu_fwrite( str, len, 1, ostream );
	}
	return ret;
}
//------------------------------------------------------------
static size_t nxu_fprintf( Ogre::DataStreamPtr &ostream, const char *fmt, ... ) {
	size_t ret = 0;
	char buffer[2048];
	va_list ap;
	va_start(ap, fmt);
	vsprintf(buffer, fmt, ap);
	va_end(ap);
	ret = nxu_fputs(buffer, ostream);
	return ret;
}
//------------------------------------------------------------
static bool nxu_feof( Ogre::DataStreamPtr &istream ) {
	return istream->eof();
}
//------------------------------------------------------------
} //namespace NXU
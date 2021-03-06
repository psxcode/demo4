/*
www.sourceforge.net/projects/tinyxml
Original code (2.0 and earlier )copyright (c) 2000-2006 Lee Thomason (www.grinninglizard.com)

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must
not claim that you wrote the original software. If you use this
software in a product, an acknowledgment in the product documentation
would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
*/

#include "DemoStableHeaders.h"
#if DBS_LM_SCRIPT_TIXML

#include <ctype.h>

#ifdef TIXML_USE_STL
#	include <sstream>
#	include <iostream>
#endif

#include "tinyxml.h"

#if DBS_LM_NXU && DBS_NXU_FILESYSTEM == DBS_NXU_FILESYSTEM_NATIVE
#	include "NXU_File.h"
#endif
//------------------------------------------------------------
bool TiXmlBase::condenseWhiteSpace = true;
//------------------------------------------------------------
// Microsoft compiler security
FILE* TiXmlFOpen( const char* filename, const char* mode )
{
	#if defined(_MSC_VER) && (_MSC_VER >= 1400 )
		FILE* fp = 0;
		errno_t err = fopen_s( &fp, filename, mode );
		if ( !err && fp )
			return fp;
		return 0;
	#else
		return fopen( filename, mode );
	#endif
}
//------------------------------------------------------------
void TiXmlBase::EncodeString( string_traits::in str, string_traits::inout outString )
{
	string_traits::str::size_type i=0;

	while( i < str.length() )
	{
		unsigned char c = (unsigned char) str[i];

		if (    c == '&' 
		     && i < ( str.length() - 2 )
			 && str[i+1] == '#'
			 && str[i+2] == 'x' )
		{
			// Hexadecimal character reference.
			// Pass through unchanged.
			// &#xA9;	-- copyright symbol, for example.
			//
			// The -1 is a bug fix from Rob Laveaux. It keeps
			// an overflow from happening if there is no ';'.
			// There are actually 2 ways to exit this loop -
			// while fails (error case) and break (semicolon found).
			// However, there is no mechanism (currently) for
			// this function to return an error.
			while ( i < str.length() - 1 )
			{
				outString.append( str.c_str() + i, 1 );
				++i;
				if ( str[i] == ';' )
					break;
			}
		}
		else if ( c == '&' )
		{
			outString.append( entity[0].str, entity[0].strLength );
			++i;
		}
		else if ( c == '<' )
		{
			outString.append( entity[1].str, entity[1].strLength );
			++i;
		}
		else if ( c == '>' )
		{
			outString.append( entity[2].str, entity[2].strLength );
			++i;
		}
		else if ( c == '\"' )
		{
			outString.append( entity[3].str, entity[3].strLength );
			++i;
		}
		else if ( c == '\'' )
		{
			outString.append( entity[4].str, entity[4].strLength );
			++i;
		}
		else if ( c < 32 )
		{
			// Easy pass at non-alpha/numeric/symbol
			// Below 32 is symbolic.
			char buf[ 32 ];
			
			#if defined(TIXML_SNPRINTF)		
				TIXML_SNPRINTF( buf, sizeof(buf), "&#x%02X;", (unsigned) ( c & 0xff ) );
			#else
				sprintf( buf, "&#x%02X;", (unsigned) ( c & 0xff ) );
			#endif		

			//*ME:	warning C4267: convert 'size_t' to 'int'
			//*ME:	Int-Cast to make compiler happy ...
			outString.append( buf, strlen( buf ) );
			++i;
		}
		else
		{
			//char realc = (char) c;
			//outString->append( &realc, 1 );
			outString += (char) c;	// somewhat more efficient function call.
			++i;
		}
	}
}
//------------------------------------------------------------
TiXmlNode::TiXmlNode( NodeType _type ) : TiXmlBase()
{
	parent = 0;
	type = _type;
	firstChild = 0;
	lastChild = 0;
	prev = 0;
	next = 0;
}
//------------------------------------------------------------
TiXmlNode::~TiXmlNode()
{
	TiXmlNode* node = firstChild;
	TiXmlNode* temp = 0;

	while ( node )
	{
		temp = node;
		node = node->next;
		delete temp;
	}	
}
//------------------------------------------------------------
void TiXmlNode::CopyTo( TiXmlNode* target ) const
{
	target->SetValue (value.c_str() );
	target->userData = userData; 
}
//------------------------------------------------------------
void TiXmlNode::Clear()
{
	TiXmlNode* node = firstChild;
	TiXmlNode* temp = 0;

	while ( node )
	{
		temp = node;
		node = node->next;
		delete temp;
	}	

	firstChild = 0;
	lastChild = 0;
}
//------------------------------------------------------------
TiXmlNode* TiXmlNode::LinkEndChild( TiXmlNode* node )
{
	assert( node->parent == 0 || node->parent == this );
	assert( node->GetDocument() == 0 || node->GetDocument() == this->GetDocument() );

	if ( node->Type() == TiXmlNode::DOCUMENT )
	{
		delete node;
		if ( GetDocument() ) GetDocument()->SetError( TIXML_ERROR_DOCUMENT_TOP_ONLY, 0, 0, TIXML_ENCODING_UNKNOWN );
		return 0;
	}

	node->parent = this;

	node->prev = lastChild;
	node->next = 0;

	if ( lastChild )
		lastChild->next = node;
	else
		firstChild = node;			// it was an empty list.

	lastChild = node;
	return node;
}
//------------------------------------------------------------
TiXmlNode* TiXmlNode::InsertEndChild( const TiXmlNode& addThis )
{
	if ( addThis.Type() == TiXmlNode::DOCUMENT )
	{
		if ( GetDocument() ) GetDocument()->SetError( TIXML_ERROR_DOCUMENT_TOP_ONLY, 0, 0, TIXML_ENCODING_UNKNOWN );
		return 0;
	}
	TiXmlNode* node = addThis.Clone();
	if ( !node )
		return 0;

	return LinkEndChild( node );
}
//------------------------------------------------------------
TiXmlNode* TiXmlNode::InsertBeforeChild( TiXmlNode* beforeThis, const TiXmlNode& addThis )
{	
	if ( !beforeThis || beforeThis->parent != this ) {
		return 0;
	}
	if ( addThis.Type() == TiXmlNode::DOCUMENT )
	{
		if ( GetDocument() ) GetDocument()->SetError( TIXML_ERROR_DOCUMENT_TOP_ONLY, 0, 0, TIXML_ENCODING_UNKNOWN );
		return 0;
	}

	TiXmlNode* node = addThis.Clone();
	if ( !node )
		return 0;
	node->parent = this;

	node->next = beforeThis;
	node->prev = beforeThis->prev;
	if ( beforeThis->prev )
	{
		beforeThis->prev->next = node;
	}
	else
	{
		assert( firstChild == beforeThis );
		firstChild = node;
	}
	beforeThis->prev = node;
	return node;
}
//------------------------------------------------------------
TiXmlNode* TiXmlNode::InsertAfterChild( TiXmlNode* afterThis, const TiXmlNode& addThis )
{
	if ( !afterThis || afterThis->parent != this ) {
		return 0;
	}
	if ( addThis.Type() == TiXmlNode::DOCUMENT )
	{
		if ( GetDocument() ) GetDocument()->SetError( TIXML_ERROR_DOCUMENT_TOP_ONLY, 0, 0, TIXML_ENCODING_UNKNOWN );
		return 0;
	}

	TiXmlNode* node = addThis.Clone();
	if ( !node )
		return 0;
	node->parent = this;

	node->prev = afterThis;
	node->next = afterThis->next;
	if ( afterThis->next )
	{
		afterThis->next->prev = node;
	}
	else
	{
		assert( lastChild == afterThis );
		lastChild = node;
	}
	afterThis->next = node;
	return node;
}
//------------------------------------------------------------
TiXmlNode* TiXmlNode::ReplaceChild( TiXmlNode* replaceThis, const TiXmlNode& withThis )
{
	if ( replaceThis->parent != this )
		return 0;

	TiXmlNode* node = withThis.Clone();
	if ( !node )
		return 0;

	node->next = replaceThis->next;
	node->prev = replaceThis->prev;

	if ( replaceThis->next )
		replaceThis->next->prev = node;
	else
		lastChild = node;

	if ( replaceThis->prev )
		replaceThis->prev->next = node;
	else
		firstChild = node;

	delete replaceThis;
	node->parent = this;
	return node;
}
//------------------------------------------------------------
bool TiXmlNode::RemoveChild( TiXmlNode* removeThis )
{
	if ( removeThis->parent != this )
	{	
		assert( 0 );
		return false;
	}

	if ( removeThis->next )
		removeThis->next->prev = removeThis->prev;
	else
		lastChild = removeThis->prev;

	if ( removeThis->prev )
		removeThis->prev->next = removeThis->next;
	else
		firstChild = removeThis->next;

	delete removeThis;
	return true;
}
//------------------------------------------------------------
const TiXmlNode* TiXmlNode::IterateChildren( const TiXmlNode* previous ) const
{
	if ( !previous )
	{
		return FirstChild();
	}
	else
	{
		assert( previous->parent == this );
		return previous->NextSibling();
	}
}
//------------------------------------------------------------
const TiXmlElement* TiXmlNode::FirstChildElement() const
{
	const TiXmlNode* node;

	for (	node = FirstChild();
			node;
			node = node->NextSibling() )
	{
		if ( node->ToElement() )
			return node->ToElement();
	}
	return 0;
}
//------------------------------------------------------------
const TiXmlElement* TiXmlNode::NextSiblingElement() const
{
	const TiXmlNode* node;

	for (	node = NextSibling();
			node;
			node = node->NextSibling() )
	{
		if ( node->ToElement() )
			return node->ToElement();
	}
	return 0;
}
//------------------------------------------------------------
const TiXmlDocument* TiXmlNode::GetDocument() const
{
	const TiXmlNode* node;

	for( node = this; node; node = node->parent )
	{
		if ( node->ToDocument() )
			return node->ToDocument();
	}
	return 0;
}
//------------------------------------------------------------
TiXmlElement::TiXmlElement (const char * _value)
	: TiXmlNode( TiXmlNode::ELEMENT )
{
	firstChild = lastChild = 0;
	value = _value;
}
//------------------------------------------------------------
#ifdef TIXML_USE_STL
TiXmlElement::TiXmlElement( string_traits::in _value ) 
	: TiXmlNode( TiXmlNode::ELEMENT )
{
	firstChild = lastChild = 0;
	value = _value;
}
#endif
//------------------------------------------------------------
TiXmlElement::TiXmlElement( const TiXmlElement& copy)
	: TiXmlNode( TiXmlNode::ELEMENT )
{
	firstChild = lastChild = 0;
	copy.CopyTo<TIXML_DEFAULT_CASE_SENSITIVE>( this );	
}
//------------------------------------------------------------
void TiXmlElement::operator=( const TiXmlElement& base )
{
	ClearThis();
	base.CopyTo<TIXML_DEFAULT_CASE_SENSITIVE>( this );
}
//------------------------------------------------------------
TiXmlElement::~TiXmlElement()
{
	ClearThis();
}
//------------------------------------------------------------
void TiXmlElement::ClearThis()
{
	Clear();
	while( attributeSet.First() )
	{
		TiXmlAttribute* node = attributeSet.First();
		attributeSet.Remove( node );
		delete node;
	}
}
//------------------------------------------------------------
void TiXmlElement::Print( FILE* cfile, int depth ) const
{
	int i;
	assert( cfile );
	for ( i=0; i<depth; i++ ) {
		fprintf( cfile, "    " );
	}

	fprintf( cfile, "<%s", value.c_str() );

	const TiXmlAttribute* attrib;
	for ( attrib = attributeSet.First(); attrib; attrib = attrib->Next() )
	{
		fprintf( cfile, " " );
		attrib->Print( cfile, depth );
	}

	// There are 3 different formatting approaches:
	// 1) An element without children is printed as a <foo /> node
	// 2) An element with only a text child is printed as <foo> text </foo>
	// 3) An element with children is printed on multiple lines.
	TiXmlNode* node;
	if ( !firstChild )
	{
		fprintf( cfile, " />" );
	}
	else if ( firstChild == lastChild && firstChild->ToText() )
	{
		fprintf( cfile, ">" );
		firstChild->Print( cfile, depth + 1 );
		fprintf( cfile, "</%s>", value.c_str() );
	}
	else
	{
		fprintf( cfile, ">" );

		for ( node = firstChild; node; node=node->NextSibling() )
		{
			if ( !node->ToText() )
			{
				fprintf( cfile, "\n" );
			}
			node->Print( cfile, depth+1 );
		}
		fprintf( cfile, "\n" );
		for( i=0; i<depth; ++i ) {
			fprintf( cfile, "    " );
		}
		fprintf( cfile, "</%s>", value.c_str() );
	}
}
//------------------------------------------------------------
bool TiXmlElement::Accept( TiXmlVisitor* visitor ) const
{
	if ( visitor->VisitEnter( *this, attributeSet.First() ) ) 
	{
		for ( const TiXmlNode* node=FirstChild(); node; node=node->NextSibling() )
		{
			if ( !node->Accept( visitor ) )
				break;
		}
	}
	return visitor->VisitExit( *this );
}
//------------------------------------------------------------
TiXmlNode* TiXmlElement::Clone() const
{
	TiXmlElement* clone = new TiXmlElement( Value() );
	if ( !clone )
		return 0;

	CopyTo<TIXML_DEFAULT_CASE_SENSITIVE>( clone );
	return clone;
}
//------------------------------------------------------------
const char* TiXmlElement::GetText() const
{
	const TiXmlNode* child = this->FirstChild();
	if ( child ) {
		const TiXmlText* childText = child->ToText();
		if ( childText ) {
			return childText->Value();
		}
	}
	return 0;
}
//------------------------------------------------------------
TiXmlDocument::TiXmlDocument() : TiXmlNode( TiXmlNode::DOCUMENT )
{
	tabsize = 4;
	useMicrosoftBOM = false;
	ClearError();
}
//------------------------------------------------------------
TiXmlDocument::TiXmlDocument( const char * documentName ) : TiXmlNode( TiXmlNode::DOCUMENT )
{
	tabsize = 4;
	useMicrosoftBOM = false;
	value = documentName;
	ClearError();
}
//------------------------------------------------------------
#ifdef TIXML_USE_STL
TiXmlDocument::TiXmlDocument( string_traits::in documentName ) : TiXmlNode( TiXmlNode::DOCUMENT )
{
	tabsize = 4;
	useMicrosoftBOM = false;
    value = documentName;
	ClearError();
}
#endif
//------------------------------------------------------------
TiXmlDocument::TiXmlDocument( const TiXmlDocument& copy ) : TiXmlNode( TiXmlNode::DOCUMENT )
{
	copy.CopyTo( this );
}
//------------------------------------------------------------
void TiXmlDocument::operator=( const TiXmlDocument& copy )
{
	Clear();
	copy.CopyTo( this );
}
//------------------------------------------------------------
bool TiXmlDocument::LoadFile( TiXmlEncoding encoding )
{
	// See STL_STRING_BUG below.
	//StringToBuffer buf( value );

	return LoadFile( Value(), encoding );
}
//------------------------------------------------------------
bool TiXmlDocument::SaveFile() const
{
	// See STL_STRING_BUG below.
//	StringToBuffer buf( value );
//
//	if ( buf.buffer && SaveFile( buf.buffer ) )
//		return true;
//
//	return false;
	return SaveFile( Value() );
}
//------------------------------------------------------------
#if DBS_LM_NXU && DBS_NXU_FILESYSTEM == DBS_NXU_FILESYSTEM_NATIVE
bool TiXmlDocument::LoadFile(char *mem, int len, TiXmlEncoding encoding)
{
	// There was a really	terrifying little	bug	here.	The	code:
	//		value	=	filename
	// in	the	STL	case,	cause	the	assignment method	of the std::string to
	// be	called.	What is	strange, is	that the std::string had the same
	// address as	it's c_str() method, and so	bad	things happen. Looks
	// like	a	bug	in the Microsoft STL implementation.
	// See STL_STRING_BUG	above.
	// Fixed with	the	StringToBuffer class.

	// reading in	binary mode	so that	tinyxml	can	normalize	the	EOL
	NXU_FILE *file = nxu_fopen(0, "rb", mem, len);

	if (file)
	{
		bool result	= LoadFile(file, encoding);
		nxu_fclose(file);
		return result;
	}
	else
	{
		SetError( 0, 0,	0, TIXML_ENCODING_UNKNOWN);
		return false;
	}
}
//------------------------------------------------------------
bool TiXmlDocument::LoadFile( NXU_FILE *file, TiXmlEncoding encoding)
{
	if (!file)
	{
		SetError( 0, 0,	0, TIXML_ENCODING_UNKNOWN);
		return false;
	}

	// Delete	the	existing data:
	Clear();
	location.Clear();

	// Get the file	size,	so we	can	pre-allocate the string. HUGE	speed	impact.
	long length	=	0;
	nxu_fseek(file,	0, SEEK_END);
	length = (long) nxu_ftell(file);
	nxu_fseek(file,	0, SEEK_SET);

	// Strange case, but good	to handle	up front.
	if (length ==	0)
	{
		SetError(TIXML_ERROR_DOCUMENT_EMPTY, 0,	0, TIXML_ENCODING_UNKNOWN);
		return false;
	}

	// If	we have	a	file,	assume it	is all one big XML file, and read	it in.
	// The document	parser may decide	the	document ends	sooner than	the	entire file, however.	
	string_traits::str data;
	data.reserve(length);

	// Subtle	bug	here.	TinyXml	did	use	fgets. But from	the	XML	spec:
	// 2.11	End-of-Line	Handling
	// <snip>
	// <quote>
	// ...the	XML	processor	MUST behave	as if	it normalized	all	line breaks	in external
	// parsed	entities (including	the	document entity) on	input, before	parsing, by	translating
	// both	the	two-character	sequence #xD #xA and any #xD that	is not followed	by #xA to
	// a single	#xA	character.
	// </quote>
	//
	// It	is not clear fgets does	that,	and	certainly	isn't	clear	it works cross platform.
	// Generally,	you	expect fgets to	translate	from the convention	of the OS	to the c/unix
	// convention, and not work	generally.

	/*
	while( fgets(	buf, sizeof(buf),	file ) )
	{
	data +=	buf;
	}
	 */

	char *buf	=	new	char[length	+	1];
	buf[0] = 0;

	if (nxu_fread(buf, length, 1,	file)	!= 1)
	{
		SetError( 0, 0,	0, TIXML_ENCODING_UNKNOWN);
		return false;
	}

	const	char *lastPos	=	buf;
	const	char *p	=	buf;

	buf[length]	=	0;
	while	(*p)
	{
		assert(p < (buf	+	length));
		if (*p ==	0xa)
		{
			// Newline character.	No special rules for this. Append	all	the	characters
			// since the last	string,	and	include	the	newline.
			data.append(lastPos, (p	-	lastPos	+	1)); //	append,	include	the	newline
			++p; //	move past	the	newline
			lastPos	=	p; //	and	point	to the new buffer	(may be	0)
			assert(p <=	(buf + length));
		}
		else if	(*p	== 0xd)
		{
			// Carriage	return.	Append what	we have	so far,	then
			// handle	moving forward in	the	buffer.
			if ((p - lastPos)	>	0)
			{
				data.append(lastPos, p - lastPos); //	do not add the CR
			}
			data +=	(char)0xa; //	a	proper newline

			if (*(p	+	1) ==	0xa)
			{
				// Carriage	return - new line	sequence
				p	+= 2;
				lastPos	=	p;
				assert(p <=	(buf + length));
			}
			else
			{
				// it	was	followed by	something	else...that	is presumably	characters again.
				++p;
				lastPos	=	p;
				assert(p <=	(buf + length));
			}
		}
		else
		{
			++p;
		}
	}
	// Handle	any	left over	characters.
	if (p	-	lastPos)
	{
		data.append(lastPos, p - lastPos);
	}
	delete []	buf;
	buf	=	0;

	Parse(data.c_str(),	0, encoding);

	if (Error())
	{
		return false;
	}
	else
	{
		return true;
	}
}
#endif //DBS_LM_NXU
//------------------------------------------------------------
bool TiXmlDocument::LoadFile( Ogre::DataStreamPtr &istream )
{
	string_traits::str scriptText = istream->getAsString();
	Parse( scriptText.c_str() );
	return !Error();
}
//------------------------------------------------------------
bool TiXmlDocument::LoadFile( const char* _filename, TiXmlEncoding encoding )
{
	// There was a really terrifying little bug here. The code:
	//		value = filename
	// in the STL case, cause the assignment method of the std::string to
	// be called. What is strange, is that the std::string had the same
	// address as it's c_str() method, and so bad things happen. Looks
	// like a bug in the Microsoft STL implementation.
	// Add an extra string to avoid the crash.
	string_traits::str filename( _filename );
	value = filename;

	// reading in binary mode so that tinyxml can normalize the EOL
	FILE* file = TiXmlFOpen( value.c_str (), "rb" );	

	if ( file )
	{
		bool result = LoadFile( file, encoding );
		fclose( file );
		return result;
	}
	else
	{
		SetError( TIXML_ERROR_OPENING_FILE, 0, 0, TIXML_ENCODING_UNKNOWN );
		return false;
	}
}
//------------------------------------------------------------
bool TiXmlDocument::LoadFile( FILE* file, TiXmlEncoding encoding )
{
	if ( !file ) 
	{
		SetError( TIXML_ERROR_OPENING_FILE, 0, 0, TIXML_ENCODING_UNKNOWN );
		return false;
	}

	// Delete the existing data:
	Clear();
	location.Clear();

	// Get the file size, so we can pre-allocate the string. HUGE speed impact.
	long length = 0;
	fseek( file, 0, SEEK_END );
	length = ftell( file );
	fseek( file, 0, SEEK_SET );

	// Strange case, but good to handle up front.
	if ( length <= 0 )
	{
		SetError( TIXML_ERROR_DOCUMENT_EMPTY, 0, 0, TIXML_ENCODING_UNKNOWN );
		return false;
	}

	// If we have a file, assume it is all one big XML file, and read it in.
	// The document parser may decide the document ends sooner than the entire file, however.
	string_traits::str data;
	data.reserve( length );

	// Subtle bug here. TinyXml did use fgets. But from the XML spec:
	// 2.11 End-of-Line Handling
	// <snip>
	// <quote>
	// ...the XML processor MUST behave as if it normalized all line breaks in external 
	// parsed entities (including the document entity) on input, before parsing, by translating 
	// both the two-character sequence #xD #xA and any #xD that is not followed by #xA to 
	// a single #xA character.
	// </quote>
	//
	// It is not clear fgets does that, and certainly isn't clear it works cross platform. 
	// Generally, you expect fgets to translate from the convention of the OS to the c/unix
	// convention, and not work generally.

	/*
	while( fgets( buf, sizeof(buf), file ) )
	{
		data += buf;
	}
	*/

	char* buf = new char[ length+1 ];
	buf[0] = 0;

	if ( fread( buf, length, 1, file ) != 1 ) {
		delete [] buf;
		SetError( TIXML_ERROR_OPENING_FILE, 0, 0, TIXML_ENCODING_UNKNOWN );
		return false;
	}

	const char* lastPos = buf;
	const char* p = buf;

	buf[length] = 0;
	while( *p ) {
		assert( p < (buf+length) );
		if ( *p == 0xa ) {
			// Newline character. No special rules for this. Append all the characters
			// since the last string, and include the newline.
			data.append( lastPos, (p-lastPos+1) );	// append, include the newline
			++p;									// move past the newline
			lastPos = p;							// and point to the new buffer (may be 0)
			assert( p <= (buf+length) );
		}
		else if ( *p == 0xd ) {
			// Carriage return. Append what we have so far, then
			// handle moving forward in the buffer.
			if ( (p-lastPos) > 0 ) {
				data.append( lastPos, p-lastPos );	// do not add the CR
			}
			data += (char)0xa;						// a proper newline

			if ( *(p+1) == 0xa ) {
				// Carriage return - new line sequence
				p += 2;
				lastPos = p;
				assert( p <= (buf+length) );
			}
			else {
				// it was followed by something else...that is presumably characters again.
				++p;
				lastPos = p;
				assert( p <= (buf+length) );
			}
		}
		else {
			++p;
		}
	}
	// Handle any left over characters.
	if ( p-lastPos ) {
		data.append( lastPos, p-lastPos );
	}		
	delete [] buf;
	buf = 0;

	Parse( data.c_str(), 0, encoding );

	if (  Error() )
        return false;
    else
		return true;
}
//------------------------------------------------------------
bool TiXmlDocument::SaveFile( const char * filename ) const
{
	// The old c stuff lives on...
	FILE* fp = TiXmlFOpen( filename, "w" );
	if ( fp )
	{
		bool result = SaveFile( fp );
		fclose( fp );
		return result;
	}
	return false;
}
//------------------------------------------------------------
bool TiXmlDocument::SaveFile( FILE* fp ) const
{
	if ( useMicrosoftBOM ) 
	{
		const unsigned char TIXML_UTF_LEAD_0 = 0xefU;
		const unsigned char TIXML_UTF_LEAD_1 = 0xbbU;
		const unsigned char TIXML_UTF_LEAD_2 = 0xbfU;

		fputc( TIXML_UTF_LEAD_0, fp );
		fputc( TIXML_UTF_LEAD_1, fp );
		fputc( TIXML_UTF_LEAD_2, fp );
	}
	Print( fp, 0 );
	return (ferror(fp) == 0);
}
//------------------------------------------------------------
void TiXmlDocument::CopyTo( TiXmlDocument* target ) const
{
	TiXmlNode::CopyTo( target );

	target->error = error;
	target->errorId = errorId;
	target->errorDesc = errorDesc;
	target->tabsize = tabsize;
	target->errorLocation = errorLocation;
	target->useMicrosoftBOM = useMicrosoftBOM;

	TiXmlNode* node = 0;
	for ( node = firstChild; node; node = node->NextSibling() )
	{
		target->LinkEndChild( node->Clone() );
	}	
}
//------------------------------------------------------------
TiXmlNode* TiXmlDocument::Clone() const
{
	TiXmlDocument* clone = new TiXmlDocument();
	if ( !clone )
		return 0;

	CopyTo( clone );
	return clone;
}
//------------------------------------------------------------
void TiXmlDocument::Print( FILE* cfile, int depth ) const
{
	assert( cfile );
	for ( const TiXmlNode* node=FirstChild(); node; node=node->NextSibling() )
	{
		node->Print( cfile, depth );
		fprintf( cfile, "\n" );
	}
}
//------------------------------------------------------------
bool TiXmlDocument::Accept( TiXmlVisitor* visitor ) const
{
	if ( visitor->VisitEnter( *this ) )
	{
		for ( const TiXmlNode* node=FirstChild(); node; node=node->NextSibling() )
		{
			if ( !node->Accept( visitor ) )
				break;
		}
	}
	return visitor->VisitExit( *this );
}
//------------------------------------------------------------
const TiXmlAttribute* TiXmlAttribute::Next() const
{
	// We are using knowledge of the sentinel. The sentinel
	// have a value or name.
	if ( next->value.empty() && next->name.empty() )
		return 0;
	return next;
}
//------------------------------------------------------------
const TiXmlAttribute* TiXmlAttribute::Previous() const
{
	// We are using knowledge of the sentinel. The sentinel
	// have a value or name.
	if ( prev->value.empty() && prev->name.empty() )
		return 0;
	return prev;
}
//------------------------------------------------------------
void TiXmlAttribute::Print( FILE* cfile, int /*depth*/ )const
{
	string_traits::str n, v;

	EncodeString( name, n );
	EncodeString( value, v );

	if (value.find ('\"') == string_traits::str::npos) {
		if ( cfile ) {
			fprintf (cfile, "%s=\"%s\"", n.c_str(), v.c_str() );
		}
	} else {
		if ( cfile ) {
			fprintf (cfile, "%s='%s'", n.c_str(), v.c_str() );
		}
	}
}
//------------------------------------------------------------
void TiXmlAttribute::Print( string_traits::inout str )const
{
	string_traits::str n, v;

	EncodeString( name, n );
	EncodeString( value, v );

	if (value.find ('\"') == string_traits::str::npos){
		str += n; str += "=\""; str += v; str += "\"";
	} else {
		str += n; str += "='"; str += v; str += "'";
	}
}
//------------------------------------------------------------
int TiXmlAttribute::QueryIntValue( int* ival ) const
{
	if ( TIXML_SSCANF( value.c_str(), "%d", ival ) == 1 )
		return TIXML_SUCCESS;
	return TIXML_WRONG_TYPE;
}
//------------------------------------------------------------
int TiXmlAttribute::QueryDoubleValue( double* dval ) const
{
	if ( TIXML_SSCANF( value.c_str(), "%lf", dval ) == 1 )
		return TIXML_SUCCESS;
	return TIXML_WRONG_TYPE;
}
//------------------------------------------------------------
void TiXmlAttribute::SetIntValue( int _value )
{
	char buf [64];
	#if defined(TIXML_SNPRINTF)		
		TIXML_SNPRINTF(buf, sizeof(buf), "%d", _value);
	#else
		sprintf (buf, "%d", _value);
	#endif
	SetValue (buf);
}
//------------------------------------------------------------
void TiXmlAttribute::SetDoubleValue( double _value )
{
	char buf [256];
	#if defined(TIXML_SNPRINTF)		
		TIXML_SNPRINTF( buf, sizeof(buf), "%lf", _value);
	#else
		sprintf (buf, "%lf", _value);
	#endif
	SetValue (buf);
}
//------------------------------------------------------------
int TiXmlAttribute::IntValue() const
{
	return atoi (value.c_str ());
}
//------------------------------------------------------------
double  TiXmlAttribute::DoubleValue() const
{
	return atof (value.c_str ());
}
//------------------------------------------------------------
TiXmlComment::TiXmlComment( const TiXmlComment& copy ) : TiXmlNode( TiXmlNode::COMMENT )
{
	copy.CopyTo( this );
}
//------------------------------------------------------------
void TiXmlComment::operator=( const TiXmlComment& base )
{
	Clear();
	base.CopyTo( this );
}
//------------------------------------------------------------
void TiXmlComment::Print( FILE* cfile, int depth ) const
{
	assert( cfile );
	for ( int i=0; i<depth; i++ )
	{
		fprintf( cfile,  "    " );
	}
	fprintf( cfile, "<!--%s-->", value.c_str() );
}
//------------------------------------------------------------
void TiXmlComment::CopyTo( TiXmlComment* target ) const
{
	TiXmlNode::CopyTo( target );
}
//------------------------------------------------------------
bool TiXmlComment::Accept( TiXmlVisitor* visitor ) const
{
	return visitor->Visit( *this );
}
//------------------------------------------------------------
TiXmlNode* TiXmlComment::Clone() const
{
	TiXmlComment* clone = new TiXmlComment();

	if ( !clone )
		return 0;

	CopyTo( clone );
	return clone;
}
//------------------------------------------------------------
void TiXmlText::Print( FILE* cfile, int depth ) const
{
	assert( cfile );
	if ( cdata )
	{
		int i;
		fprintf( cfile, "\n" );
		for ( i=0; i<depth; i++ ) {
			fprintf( cfile, "    " );
		}
		fprintf( cfile, "<![CDATA[%s]]>\n", value.c_str() );	// unformatted output
	}
	else
	{
		string_traits::str buffer;
		EncodeString( value, buffer );
		fprintf( cfile, "%s", buffer.c_str() );
	}
}
//------------------------------------------------------------
void TiXmlText::CopyTo( TiXmlText* target ) const
{
	TiXmlNode::CopyTo( target );
	target->cdata = cdata;
}
//------------------------------------------------------------
bool TiXmlText::Accept( TiXmlVisitor* visitor ) const
{
	return visitor->Visit( *this );
}
//------------------------------------------------------------
TiXmlNode* TiXmlText::Clone() const
{	
	TiXmlText* clone = 0;
	clone = new TiXmlText( "" );

	if ( !clone )
		return 0;

	CopyTo( clone );
	return clone;
}
//------------------------------------------------------------
TiXmlDeclaration::TiXmlDeclaration( const char * _version,
									const char * _encoding,
									const char * _standalone )
	: TiXmlNode( TiXmlNode::DECLARATION )
{
	version = _version;
	encoding = _encoding;
	standalone = _standalone;
}
//------------------------------------------------------------
#ifdef TIXML_USE_STL
TiXmlDeclaration::TiXmlDeclaration(	string_traits::in _version,
									string_traits::in _encoding,
									string_traits::in _standalone )
	: TiXmlNode( TiXmlNode::DECLARATION )
{
	version = _version;
	encoding = _encoding;
	standalone = _standalone;
}
#endif
//------------------------------------------------------------
TiXmlDeclaration::TiXmlDeclaration( const TiXmlDeclaration& copy )
	: TiXmlNode( TiXmlNode::DECLARATION )
{
	copy.CopyTo( this );	
}
//------------------------------------------------------------
void TiXmlDeclaration::operator=( const TiXmlDeclaration& copy )
{
	Clear();
	copy.CopyTo( this );
}
//------------------------------------------------------------
void TiXmlDeclaration::Print( FILE* cfile, int /*depth*/ ) const
{
	if ( cfile ) fprintf( cfile, "<?xml " );

	if ( !version.empty() ) {
		if ( cfile ) fprintf (cfile, "version=\"%s\" ", version.c_str ());
	}
	if ( !encoding.empty() ) {
		if ( cfile ) fprintf (cfile, "encoding=\"%s\" ", encoding.c_str ());
	}
	if ( !standalone.empty() ) {
		if ( cfile ) fprintf (cfile, "standalone=\"%s\" ", standalone.c_str ());
	}
	if ( cfile ) fprintf( cfile, "?>" );
}
//------------------------------------------------------------
void TiXmlDeclaration::Print( string_traits::inout str )const
{
	str += "<?xml ";
	if ( !version.empty() ) {
		str += "version=\""; str += version; str += "\" ";
	}
	if ( !encoding.empty() ) {
		str += "encoding=\""; str += encoding; str += "\" ";
	}
	if ( !standalone.empty() ) {
		str += "standalone=\""; str += standalone; str += "\" ";
	}
	str += "?>";
}
//------------------------------------------------------------
void TiXmlDeclaration::CopyTo( TiXmlDeclaration* target ) const
{
	TiXmlNode::CopyTo( target );

	target->version = version;
	target->encoding = encoding;
	target->standalone = standalone;
}
//------------------------------------------------------------
bool TiXmlDeclaration::Accept( TiXmlVisitor* visitor ) const
{
	return visitor->Visit( *this );
}
//------------------------------------------------------------
TiXmlNode* TiXmlDeclaration::Clone() const
{	
	TiXmlDeclaration* clone = new TiXmlDeclaration();

	if ( !clone )
		return 0;

	CopyTo( clone );
	return clone;
}
//------------------------------------------------------------
void TiXmlUnknown::Print( FILE* cfile, int depth ) const
{
	for ( int i=0; i<depth; i++ )
		fprintf( cfile, "    " );
	fprintf( cfile, "<%s>", value.c_str() );
}
//------------------------------------------------------------
void TiXmlUnknown::CopyTo( TiXmlUnknown* target ) const
{
	TiXmlNode::CopyTo( target );
}
//------------------------------------------------------------
bool TiXmlUnknown::Accept( TiXmlVisitor* visitor ) const
{
	return visitor->Visit( *this );
}
//------------------------------------------------------------
TiXmlNode* TiXmlUnknown::Clone() const
{
	TiXmlUnknown* clone = new TiXmlUnknown();

	if ( !clone )
		return 0;

	CopyTo( clone );
	return clone;
}
//------------------------------------------------------------
TiXmlAttributeSet::TiXmlAttributeSet()
{
	sentinel.next = &sentinel;
	sentinel.prev = &sentinel;
}
//------------------------------------------------------------
TiXmlAttributeSet::~TiXmlAttributeSet()
{
	assert( sentinel.next == &sentinel );
	assert( sentinel.prev == &sentinel );
}
//------------------------------------------------------------
void TiXmlAttributeSet::Add( TiXmlAttribute* addMe )
{
    #ifdef TIXML_USE_STL
	assert( !Find( TIXML_STRING( addMe->Name() ) ) );	// Shouldn't be multiply adding to the set.
	#else
	assert( !Find( addMe->Name() ) );	// Shouldn't be multiply adding to the set.
	#endif

	addMe->next = &sentinel;
	addMe->prev = sentinel.prev;

	sentinel.prev->next = addMe;
	sentinel.prev      = addMe;
}
//------------------------------------------------------------
void TiXmlAttributeSet::Remove( TiXmlAttribute* removeMe )
{
	TiXmlAttribute* node;

	for( node = sentinel.next; node != &sentinel; node = node->next )
	{
		if ( node == removeMe )
		{
			node->prev->next = node->next;
			node->next->prev = node->prev;
			node->next = 0;
			node->prev = 0;
			return;
		}
	}
	assert( 0 );		// we tried to remove a non-linked attribute.
}
//------------------------------------------------------------
#ifdef TIXML_USE_STL	
TiXmlBase::traits<TiXmlBase::istream>::inout operator >> ( TiXmlBase::traits<TiXmlBase::istream>::inout in, TiXmlNode & base )
{
	TiXmlBase::string_traits::str tag;
	tag.reserve( 8 * 1000 );
	base.StreamIn( in, tag );

	base.Parse( tag.c_str(), 0, TIXML_DEFAULT_ENCODING );
	return in;
}
//------------------------------------------------------------
TiXmlBase::traits<TiXmlBase::ostream>::inout operator << ( TiXmlBase::traits<TiXmlBase::ostream>::inout out, const TiXmlNode & base )
{
	TiXmlPrinter printer;
	printer.SetStreamPrinting();
	base.Accept( &printer );
	out << printer.Str();

	return out;
}
//------------------------------------------------------------
TiXmlBase::string_traits::inout operator<< ( TiXmlBase::string_traits::inout out, const TiXmlNode& base )
{
	TiXmlPrinter printer;
	printer.SetStreamPrinting();
	base.Accept( &printer );
	out.append( printer.Str() );

	return out;
}
#endif
//------------------------------------------------------------
TiXmlHandle TiXmlHandle::FirstChild() const
{
	if ( node )
	{
		TiXmlNode* child = node->FirstChild();
		if ( child )
			return TiXmlHandle( child );
	}
	return TiXmlHandle( 0 );
}
//------------------------------------------------------------
TiXmlHandle TiXmlHandle::FirstChildElement() const
{
	if ( node )
	{
		TiXmlElement* child = node->FirstChildElement();
		if ( child )
			return TiXmlHandle( child );
	}
	return TiXmlHandle( 0 );
}
//------------------------------------------------------------
TiXmlHandle TiXmlHandle::Child( int count ) const
{
	if ( node )
	{
		int i;
		TiXmlNode* child = node->FirstChild();
		for (	i=0;
				child && i<count;
				child = child->NextSibling(), ++i )
		{
			// nothing
		}
		if ( child )
			return TiXmlHandle( child );
	}
	return TiXmlHandle( 0 );
}
//------------------------------------------------------------
TiXmlHandle TiXmlHandle::ChildElement( int count ) const
{
	if ( node )
	{
		int i;
		TiXmlElement* child = node->FirstChildElement();
		for (	i=0;
				child && i<count;
				child = child->NextSiblingElement(), ++i )
		{
			// nothing
		}
		if ( child )
			return TiXmlHandle( child );
	}
	return TiXmlHandle( 0 );
}
//------------------------------------------------------------
bool TiXmlPrinter::VisitEnter( const TiXmlDocument& )
{
	return true;
}
//------------------------------------------------------------
bool TiXmlPrinter::VisitExit( const TiXmlDocument& )
{
	return true;
}
//------------------------------------------------------------
bool TiXmlPrinter::VisitEnter( const TiXmlElement& element, const TiXmlAttribute* firstAttribute )
{
	DoIndent();
	buffer += "<";
	buffer += element.Value();

	for( const TiXmlAttribute* attrib = firstAttribute; attrib; attrib = attrib->Next() )
	{
		buffer += " ";
		attrib->Print( buffer );
	}

	if ( !element.FirstChild() ) 
	{
		buffer += " />";
		DoLineBreak();
	}
	else 
	{
		buffer += ">";
		if (    element.FirstChild()->ToText()
			  && element.LastChild() == element.FirstChild()
			  && element.FirstChild()->ToText()->CDATA() == false )
		{
			simpleTextPrint = true;
			// no DoLineBreak()!
		}
		else
		{
			DoLineBreak();
		}
	}
	++depth;	
	return true;
}
//------------------------------------------------------------
bool TiXmlPrinter::VisitExit( const TiXmlElement& element )
{
	--depth;
	if ( !element.FirstChild() ) 
	{
		// nothing.
	}
	else 
	{
		if ( simpleTextPrint )
		{
			simpleTextPrint = false;
		}
		else
		{
			DoIndent();
		}
		buffer += "</";
		buffer += element.Value();
		buffer += ">";
		DoLineBreak();
	}
	return true;
}
//------------------------------------------------------------
bool TiXmlPrinter::Visit( const TiXmlText& text )
{
	if ( text.CDATA() )
	{
		DoIndent();
		buffer += "<![CDATA[";
		buffer += text.Value();
		buffer += "]]>";
		DoLineBreak();
	}
	else if ( simpleTextPrint )
	{
		TiXmlBase::string_traits::str str;
		TiXmlBase::EncodeString( text.ValueTStr(), str );
		buffer += str;
	}
	else
	{
		DoIndent();
		TiXmlBase::string_traits::str str;
		TiXmlBase::EncodeString( text.ValueTStr(), str );
		buffer += str;
		DoLineBreak();
	}
	return true;
}
//------------------------------------------------------------
bool TiXmlPrinter::Visit( const TiXmlDeclaration& declaration )
{
	DoIndent();
	declaration.Print( buffer );
	DoLineBreak();
	return true;
}
//------------------------------------------------------------
bool TiXmlPrinter::Visit( const TiXmlComment& comment )
{
	DoIndent();
	buffer += "<!--";
	buffer += comment.Value();
	buffer += "-->";
	DoLineBreak();
	return true;
}
//------------------------------------------------------------
bool TiXmlPrinter::Visit( const TiXmlUnknown& unknown )
{
	DoIndent();
	buffer += "<";
	buffer += unknown.Value();
	buffer += ">";
	DoLineBreak();
	return true;
}
//------------------------------------------------------------
#endif // DBS_LM_SCRIPT_TIXML
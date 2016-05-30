#pragma once

template< typename Base >
struct TagCounter
{
	static size_t s_counter; // declaration
};

template< typename Base >
size_t TagCounter< Base >::s_counter; // definition: default 0


//For every visitable class, we can create a variable to store its tag:
template< typename Visitable, typename Base >
struct TagHolder
{
	static size_t s_tag;
};

//A tag is initialized the first time It is requested:
template< typename Visitable, typename Base >
size_t GetTag()
{
	size_t& tag = TagHolder< const Visitable, const Base >::s_tag;
	if( tag == 0 ) {
		// first time : generate tag
		tag = ++TagCounter< const Base >::s_counter;
	}
	return tag;
}

//The GetTag templates applies const qualifier to its arguments and so GetTag and GetTag both return the same tag value. This prevents unnecessary tag generation and treats const/non-const objects in the same way.
//To ensure initialization at startup, we force a call to GetTag while defining the tag variable. 
template< typename Visitable, typename Base >
size_t TagHolder< Visitable, Base >::s_tag = GetTag< Visitable, Base >();

//Curiously Recurring Template Pattern ( CRTP )[3] allows defining a helper function in Base that every Visitable class can use to retrieve its tag: 
// Base class must inherit from BaseVisitable
template< typename Base >
struct VisitableBase
{
	template< typename Visitable >
	size_t GetTagHelper( const Visitable* /* this */ ) const
	{
		return GetTag< Visitable, Base >();
	}
};

//A macro helps defining a virtual function in every Visitable class to return its tag: 
#define VIS_DEFINE_VISITABLE()      \
virtual size_t Tag() const			\
{                                   \
	return GetTagHelper( this );    \
}

//Visitable is an empty base class and the compiler could optimize it away. Here's how a client would look like (similar to Loki): 
class Shape : public VisitableBase< Shape > {
public:
	VIS_DEFINE_VISITABLE()
};

class Circle : public Shape {
public:
	VIS_DEFINE_VISITABLE()
};

class Polygon : public Shape {
public:
	VIS_DEFINE_VISITABLE()
};

//The VTable is parameterized over the type of function it holds as well as the Base class. 
template< typename Base, typename Func >
class VTable {
	std::vector< Func > table_; // storage

	//A new function is added to the table by passing the Visitable to find out the slot. 
	template< typename Visitable >
	void add( Func f )
	{
		size_t index = GetTag< Visitable, Base >(); // find the slot
		if( index >= table_.size() )
		{
			// find function for Base if it exists
			const size_t base_tag = GetTag< Base, Base >();
			Func default_function = ( base_tag >= table_.size() ) ? 0 : table_[ base_tag ];
			// expand the table
			table_.resize( index + 1, default_function );
		}
		table_[ index ] = f;
	}

	//An indexing operation is also provided: 
	Func operator[] ( size_t index ) const
	{
		if( index >= table_.size() )
			index = GetTag< Base, Base >();

		return table_[ index ];
	}

}; // VTable

//The VTable defaults to function for Base during expansion and retrieval resulting in a behavior closer to cyclic than acyclic visitor.

//Cooperative Visitor technique allows implementing visitors that take N arguments. For this discussion we look at a library implementation that allows creating visitors that take just one argument: the object being visited. 
template< typename Base, typename ReturnType >
class Visitor;

//Clients derive from Visitor and implement necessary visit methods: 
class ShapeVisitor : public Visitor< Shape, double >
{
public:
	double Visit( Shape& ) { /* implementation */ }      
	double Visit( Circle& ) { /* implementation */ }      
	double Visit( Polygon& ) { /* implementation */ }     

};
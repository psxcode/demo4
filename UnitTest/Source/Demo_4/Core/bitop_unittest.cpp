#include "Core\bitop.h"
#include "gtest\gtest.h"

TEST( bitop, StaticFunctions )
{
	typedef Demo::Core::bitop< unsigned int > uint_bitop;

	unsigned int flag0 = 0x00000000;
	unsigned int flag1 = 0x00000001;
	unsigned int flag2 = 0x00001000;
	unsigned int flag3 = 0x00010000;

	unsigned int flagBitop = 0x0;
	unsigned int flagTest = 0x0;

	//Demo::bitop<T>::set_flag test
	uint_bitop::set_flag( flagBitop, flag1 );
	flagTest |= flag1;
	EXPECT_EQ( flagBitop, flagTest ) << "set_flag 01";

	uint_bitop::set_flag( flagBitop, flag2 );
	flagTest |= flag2;
	EXPECT_EQ( flagBitop, flagTest ) << "set_flag 02";

	uint_bitop::set_flag( flagBitop, flag3 );
	flagTest |= flag3;
	EXPECT_EQ( flagBitop, flagTest ) << "set_flag 03";

	//Demo::bitop<T>::reset_flag test
	uint_bitop::reset_flag( flagBitop, flag0 );
	EXPECT_EQ( flagBitop, flagTest ) << "reset_flag 01";

	uint_bitop::reset_flag( flagBitop, flag1 );
	EXPECT_EQ( flagBitop, unsigned int(0x00011000) ) << "reset_flag 02";

	uint_bitop::reset_flag( flagBitop, flag2 );
	EXPECT_EQ( flagBitop, unsigned int(0x00010000) ) << "reset_flag 03";

	//Demo::bitop<T>::toggle_flag test
	uint_bitop::toggle_flag( flagBitop, flag1 );
	EXPECT_EQ( flagBitop, unsigned int(0x00010001) ) << "toggle_flag 01";

	uint_bitop::toggle_flag( flagBitop, flag3 );
	EXPECT_EQ( flagBitop, unsigned int(0x00000001) ) << "toggle_flag 02";

	//Demo::bitop<T>::is_flag_set test
	EXPECT_TRUE( uint_bitop::is_flag_set( flagBitop, unsigned int(0x00000001) ) ) << "is_flag_set 01";

	EXPECT_FALSE( uint_bitop::is_flag_set( flagBitop, unsigned int(0x10000000) ) ) << "is_flag_set 02";

	//Demo::bitop<T>::and_flag test
	EXPECT_EQ( uint_bitop::and_flag( flagBitop, flag1 ), unsigned int(0x00000001) ) << "and_flag 01";

	EXPECT_EQ( uint_bitop::and_flag( unsigned int(0x10000001), unsigned int(0x00000001) ), unsigned int(0x00000001) ) << "and_flag 02";
}

TEST( bitop, CtorOperators )
{
	//default_ctor value_ctor copy_ctor test
	{
		Demo::Core::bitop< int > def;
		EXPECT_EQ( def.value, 0x0 ) << "default_ctor test";

		Demo::Core::bitop< int > a( 0x00010000 );
		EXPECT_EQ( a.value, 0x00010000 ) << "value_ctor test";

		Demo::Core::bitop< int > b( a );
		EXPECT_EQ( b.value, a.value ) << "copy_ctor test";
	}

	//operator= operator== operator!= test
	{
		Demo::Core::bitop< int > a( 0x0000FF00 );
		Demo::Core::bitop< int > b( 0xC0000000 );

		EXPECT_TRUE( a != b ) << "operator!= test";

		b = a;
		EXPECT_EQ( b.value, a.value ) << "operator= test";

		EXPECT_TRUE( b == a ) << "operator== test";
	}
}

TEST( bitop, MutatorFunctions )
{
	Demo::Core::bitop< int > a;

	//set test
	a.set( 0xFFFFFFFF );
	EXPECT_EQ( a.value, 0xFFFFFFFF ) << "set test";

	//reset test
	a.reset( 0x000FF000 );
	EXPECT_EQ( a.value, 0xFFF00FFF ) << "reset test";

	//toggle test
	a.toggle( 0xFFF00000 );
	EXPECT_EQ( a.value, 0x00000FFF ) << "toggle test";

	//is_set test
	EXPECT_TRUE( a.is_set(0x00000F00) ) << "is_set test 01";
	EXPECT_FALSE( a.is_set(0xF0000000) ) << "is_set test 02";

	EXPECT_EQ( a.and( 0xF0000000 ), 0x0 ) << "and test 01";
	EXPECT_EQ( a.and( 0xF0F0F0F0 ), 0x000000F0 ) << "and test 02";
}
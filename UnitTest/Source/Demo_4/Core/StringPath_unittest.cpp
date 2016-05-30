#include "Core\StringPath.h"
#include "gtest\gtest.h"

//#pragma comment( lib, "Demo_4.lib" )



TEST( StringPath, MainTest )
{
	using Demo::Core::StringPath;
	StringPath strp( "Root" );

	EXPECT_EQ( strp.getName(), "Root" );
	EXPECT_TRUE( strp.isRootDir() );
	EXPECT_TRUE( strp.getParentDir() == 0 );
	EXPECT_TRUE( strp.getRootDir() == 0 );
	EXPECT_EQ( strp.numChildDirs(), 0 );

	strp.createChildDir( "D0_Lv1" );
	strp.createChildDir( "D1_Lv1" );
	EXPECT_EQ( strp.numChildDirs(), 2 );

	{
		StringPath::shared_ptr ptr = strp.getChildDir( "D0_Lv1" );
		EXPECT_TRUE( !ptr.isNull() );
		EXPECT_EQ( ptr->getFullPath(), "Root/D0_Lv1/" );

		ptr = strp.getChildDir( "NotExist" );
		EXPECT_TRUE( ptr.isNull() );

		ptr = strp.getChildDir( "D1_Lv1" );
		EXPECT_TRUE( !ptr.isNull() );
		EXPECT_EQ( ptr->getFullPath(), "Root/D1_Lv1/" );

		strp.removeChildDir( ptr->getName() );
		EXPECT_EQ( strp.numChildDirs(), 1 );
		EXPECT_TRUE( ptr->isRootDir() );
		EXPECT_TRUE( ptr->getParentDir() == 0 );
		EXPECT_EQ( ptr->getFullPath(), "D1_Lv1/" );

		strp.createChildDir( "D1_Lv1" );
		StringPath::shared_ptr lv1Dir = strp.getChildDir( "D1_Lv1" );
		EXPECT_TRUE( lv1Dir != 0 );
		EXPECT_EQ( strp.numChildDirs(), 2 );
		StringPath::shared_ptr lv2Dir = lv1Dir->createChildDir( "D0_Lv2" );
		EXPECT_TRUE( lv2Dir != 0 );
		StringPath::shared_ptr plv2Dir = ptr->createChildDir( "D1_Lv2" );
		EXPECT_TRUE( plv2Dir != 0 );
		strp.addDir( ptr );
		EXPECT_EQ( strp.numChildDirs(), 2 );
		EXPECT_TRUE( lv1Dir == ptr );
		EXPECT_EQ( ptr->numChildDirs(), 2 );
		EXPECT_EQ( lv1Dir->numChildDirs(), 2 );
	}

	EXPECT_EQ( strp.numChildDirs(), 2 );
}
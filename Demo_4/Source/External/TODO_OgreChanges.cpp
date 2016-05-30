
/**************************************************************************
 * Additions
 *************************************************************************/
namespace /*Ogre*/
{
	//-------------------------------------------------------------------
	class MaterialManager
	{
	public:
		/*( �� ��������� )
		--�������:
		����� ���������� �������������� �����,
		��� �� ������ �.
		������ ��������� ������� ����� ����������.

		--�������������:
		������� �������� ��� ����� ����������(� underscore),
		��� ��� ����� �������������� �� ������ Material.

		--���� ��������:
		����� ��������� ����� � _getSchemeIndex � _getSchemeName
		*/
		bool _hasSchemeName( const String& name )const;
	};

	bool MaterialManager::_hasSchemeName( const String& schemeName )const
	{
		SchemeMap::const_iterator i = mSchemes.find(schemeName);
		if (i != mSchemes.end())
		{
			return true;
		}
		return false;
	}
	//-------------------------------------------------------------------
	class Material
	{
	public:
		//--------------------------------------------------------------------

		/*
		��������� typedef LodTechniques � public
		-- �������:
		���������� ��� LodTechniques �������� � protected ������.
		��� ������� ������, ��� LodTechniques �� �������������� �������� ������ ������.
		������ ��� ������� ����� ��� �������� ������ ������������� �� ��� ��������.
		��� ����� Material generator ���� �� ������ ������������ ����� �����,
		� ����� �������������� ��� ������� �� ���'�� ��� ���������� �����.

		-- ���� ��������:
		���� ���������� ���������� - � public ������ ����� LodValueIterator
		*/

		//--------------------------------------------------------------------

		/*
		typedef LodTechniquesPtr
		-- �������:
		������ ����� ���������� LodTechniques �������������� ������������ ������ ������ Ogre::Material
		������ � ������ �������� ���������� ��� � ��������� ��������.
		����� �� ���� �������� � ��������� ���� �������� SharedPtr

		-- ���� ��������:
		����� ���������� ���� LodTechniques ( ��������� � public )
		*/
		typedef SharedPtr<LodTechniques> LodTechniquesPtr;

		//--------------------------------------------------------------------

		/*( �� ��������� )
		������� getTechniques
		-- �������:
		���������� ����� � Material �������
		��� ����������� ����� ���������� ( Material Scheme )
		������� GetBestTechnique �������� �� ������� �����.
		��� ���������� ������� ����� � MaterialManager::_getActiveSchemeIndex().
		����� ������� ������������ ����� ���� ����, �� ��� ��������.
		� ���� �� ������ ���������� ����� ����� ������ �������� �� ����,
		���� ��� ��� �������� ( typedef map<unsigned short, Technique*>::type LodTechniques; )
		��������: ������ ������ �������� �� ���� ��������� ����� ���������� ���������.
				�� ����� ���������� ��� ���� ������� �����.
				�� ���� ����� ����� �������� ������� ��������� ���������� ��� �����.

		-- �������������:
		�������������� ������������� � ������ ���� Material Generator.
		�� ���� Generator ����� ����� ��� ����� � �������������� ��� ���������� � ������������
		������� �������� � ���� SharedPtr, ����� �� ���� �������� � ���������

		-- ���� ��������:
		����� ��������� ��� ������� ����� Material::getBestTechnique
		*/
		LodTechniquesPtr getTechniques( const String& schemeName );

		//--------------------------------------------------------------------

		/*( �� ��������� )
		������� copyTechniquesByScheme
		-- �������:
		������ ������ ����������� ����� ������ - ������� ��� � ����� ���������.

		-- �������������:
		�������������� ������������� � Material generator.
		��� ������ ��� �������� ������� ����� ��� ������ ������ ��� ����� ������ �����.
		����� Material generator ������������ ������ �������, �������� ��������� ���������.
		*/
		LodTechniquesPtr copyTechniquesByScheme( const String& oldSchemeName, const String& newSchemeName );

		//--------------------------------------------------------------------

		/*
		������� changeTechniquesScheme
		-- �������:
		�������� ����� ���������� � ���� ������.
		���������� ����� ������ � ���������� ������, ������������� �� ������� ���'��.

		-- �������������:
		�������������� ������������� � Material generator.
		Material generator ������ �������� ��� ����� (�������� convert_gbuffer),
		������� ��������������, �� ��� ����� ����� (�������� gbuffer).
		��� ���� �� �������� ����� ������, ������� �� ��������.

		-- ���� ��������:
		����� ������� Material::getBestTechnique()
		*/
		LodTechniquesPtr changeTechniquesScheme( const String& oldSchemeName, const String& newSchemeName );
	};
	//--------------------------------------------------------------------
	Material::LodTechniquesPtr Material::getLodTechniques(const String &schemeName)
	{
		MaterialManager& matMgr = MaterialManager::getSingleton();
		unsigned short schemeIndex = matMgr._getSchemeIndex( schemeName );
		if( mCompilationRequired || mSupportedTechniques.empty() )
		{
			//TODO: ����� ����������� ��������������� ���� ������
			//      �� ���������� mTechniques
			return LodTechniquesPtr(0);
		}
		else
		{
			BestTechniquesBySchemeList::iterator si = mBestTechniquesBySchemeList.find( schemeIndex );
			if( si == mBestTechniquesBySchemeList.end() )
			{
				return LodTechniquesPtr(0);
			}
			return LodTechniquesPtr( si->second, SPFM_NO_FREE );
		}
	}
	//--------------------------------------------------------------------
	Material::LodTechniquesPtr Material::duplicateLodTechniques(const String &oldSchemeName, const String &newSchemeName)
	{
		MaterialManager& matMgr = MaterialManager::getSingleton();
		unsigned short oldSchemeIndex = matMgr._getSchemeIndex( oldSchemeName );
		unsigned short newSchemeIndex = matMgr._getSchemeIndex( newSchemeName );		
		LodTechniquesPtr lodtechs( OGRE_NEW_T(LodTechniques, MEMCATEGORY_RESOURCE), SPFM_DELETE_T );

		if( mCompilationRequired || mSupportedTechniques.empty() )
		{
			//����� �������� ������� ������� �� ���������� mTechniques
			Techniques::iterator iter = mTechniques.begin();
			Techniques::iterator end_iter = mTechniques.end();
			for( ; iter != end_iter; ++iter )
			{
				//��������� ������ ������� �� ������������ ����� ����������
				if( (*iter)->_getSchemeIndex() == oldSchemeIndex )
				{
					Technique* tech = OGRE_NEW Technique( this, (**iter) );
					tech->setSchemeName( newSchemeName );

					// Insert won't replace if technique for this scheme/lod is	already there
					lodtechs->insert( LodTechniques::value_type( tech->getLodIndex(), tech ) );
				}
			}
		}
		else
		{
			//����� �������� ������� �� ���������� mBestTechniquesBySchemeList
			BestTechniquesBySchemeList::iterator si = mBestTechniquesBySchemeList.find( oldSchemeIndex );
			if( si != mBestTechniquesBySchemeList.end() )
			{
				LodTechniques::iterator iter = si->second->begin();
				LodTechniques::iterator end_iter = si->second->end();
				for( ; iter != end_iter; ++iter )
				{
					Technique* tech = OGRE_NEW Technique( this, *(iter->second) );
					tech->setSchemeName( newSchemeName );

					// Insert won't replace if technique for this scheme/lod is	already there
					lodtechs->insert( LodTechniques::value_type( tech->getLodIndex(), tech ) );
				}
			}
		}

		//������� ����� ������� � ����� ��������� mTechniques
		LodTechniques::iterator iter = lodtechs->begin();
		LodTechniques::iterator end_iter = lodtechs->end();
		for( ; iter != end_iter; ++iter )
		{
			mTechniques.push_back( iter->second );
		}
		mCompilationRequired = true;

		return lodtechs;
	}
	//--------------------------------------------------------------------
	Material::LodTechniquesPtr Material::changeTechniquesScheme( const String& oldSchemeName, const String& newSchemeName )
	{
		MaterialManager &matMgr = MaterialManager::getSingleton();
		unsigned short oldSchemeIndex = matMgr._getSchemeIndex( oldSchemeName );
		unsigned short newSchemeIndex = matMgr._getSchemeIndex( newSchemeName );
		LodTechniquesPtr lodtechs( OGRE_NEW_T(LodTechniques, MEMCATEGORY_RESOURCE), SPFM_DELETE_T );
		TechniqueIterator iter = getTechniqueIterator();
		while( iter.hasMoreElements() )
		{
			Technique* tech = iter.getNext();
			if( tech->_getSchemeIndex() == oldSchemeIndex )
			{
				tech->setSchemeName( newSchemeName );
				lodtechs->insert( LodTechniques::value_type( tech->getLodIndex(), tech ) );
			}
		}
		mCompilationRequired = true;
		return lodtechs;
	}
	//-------------------------------------------------------------------
}
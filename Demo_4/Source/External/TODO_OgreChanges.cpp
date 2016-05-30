
/**************************************************************************
 * Additions
 *************************************************************************/
namespace /*Ogre*/
{
	//-------------------------------------------------------------------
	class MaterialManager
	{
	public:
		/*( НЕ ДОБАВЛЯТЬ )
		--Причина:
		Любое упоминание несуществующей схемы,
		тут же создаёт её.
		Просто проверить наличие схемы невозможно.

		--Использование:
		Функция делается под стиль внутренней(с underscore),
		так как вызов предполагается из класса Material.

		--Куда добавить:
		Лучше поставить рядом с _getSchemeIndex и _getSchemeName
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
		Перенести typedef LodTechniques в public
		-- Причина:
		Изначально тип LodTechniques объявлен в protected секции.
		Это сделано потому, что LodTechniques не предполагается выносить наружу класса.
		Однако это удобный пакет для хранения техник упорядоченных по ЛОД индексам.
		Для моего Material generator было бы удобно использовать такой пакет,
		и разом конвертировать все техники по ЛОД'ам для конкретной схемы.

		-- Куда добавить:
		Выше настоящего объявления - в public секцию после LodValueIterator
		*/

		//--------------------------------------------------------------------

		/*
		typedef LodTechniquesPtr
		-- Причина:
		скорее всего изначально LodTechniques предполагалось использовать внутри класса Ogre::Material
		однако я теперь планирую возвращать его в некоторых функциях.
		Чтобы не было вопросов с удалением надо добавить SharedPtr

		-- Куда добавить:
		после объявления типа LodTechniques ( перенести в public )
		*/
		typedef SharedPtr<LodTechniques> LodTechniquesPtr;

		//--------------------------------------------------------------------

		/*( НЕ ДОБАВЛЯТЬ )
		Функция getTechniques
		-- Причина:
		Невозможно взять у Material технику
		для определённой схемы материалов ( Material Scheme )
		Функция GetBestTechnique работает по текущей схеме.
		Она спрашивает текущую схему у MaterialManager::_getActiveSchemeIndex().
		Можно конечно переставлять схемы туда сюда, но это неудобно.
		К тому же вообще невозможно взять пакет техник разбитых на ЛОДы,
		хотя они так хранятся ( typedef map<unsigned short, Technique*>::type LodTechniques; )
		Поправка: Пакеты техник разбитых на ЛОДы создаются после компиляции материала.
				До этого контейнеры для этих пакетов пусты.
				То есть сразу после парсинга скрипта материала контейнеры ещё пусты.

		-- Использование:
		Предполагается использование в классе типа Material Generator.
		То есть Generator будет брать всю ветку и модифицировать или копировать с модификацией
		Возврат значения в виде SharedPtr, чтобы не было вопросов с удалением

		-- Куда добавить:
		Лучше поставить эту функцию после Material::getBestTechnique
		*/
		LodTechniquesPtr getTechniques( const String& schemeName );

		//--------------------------------------------------------------------

		/*( НЕ ДОБАВЛЯТЬ )
		Функция copyTechniquesByScheme
		-- Причина:
		Лучший способ скопировать пакет техник - сделать это в самом материале.

		-- Использование:
		Предполагается использование в Material generator.
		Для начала сам материал создает копию ЛОД пакета техник под новым именем схемы.
		Затем Material generator модифицирует каждую технику, добавляя шейдерные программы.
		*/
		LodTechniquesPtr copyTechniquesByScheme( const String& oldSchemeName, const String& newSchemeName );

		//--------------------------------------------------------------------

		/*
		Функция changeTechniquesScheme
		-- Причина:
		Изменяет схему материалов у всех техник.
		Возвращает пакет техник с измененной схемой, упорядоченных по индексу ЛОД'ов.

		-- Использование:
		Предполагается использование в Material generator.
		Material generator просит изменить имя схемы (допустим convert_gbuffer),
		заранее подготовленной, на имя своей схемы (допустим gbuffer).
		При этом он получает пакет техник, которые он изменяет.

		-- Куда добавить:
		После функции Material::getBestTechnique()
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
			//TODO: Можно реализовать самостоятельнвй сбор техник
			//      из контейнера mTechniques
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
			//Будем выбирать техники вручную из контейнера mTechniques
			Techniques::iterator iter = mTechniques.begin();
			Techniques::iterator end_iter = mTechniques.end();
			for( ; iter != end_iter; ++iter )
			{
				//Проверяем каждую технику на соответствие схеме материалов
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
			//Можно выбирать техники из контейнера mBestTechniquesBySchemeList
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

		//Добавим новые техники в общий контейнер mTechniques
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
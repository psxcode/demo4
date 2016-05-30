#pragma once
#include "Tagged.h"

namespace Demo
{
namespace Core
{
//------------------------------------------------------------
class _DemoExport Controller : public Tagged<Controller>
{
public:
	DEFINE_CLASS_SHARED_PTR(Controller)
	DEFINE_TAGGED
	//--------------------------------//
	class traits
	{
	public:
		typedef type_traits< Controller >::ptr			child_ctr_t;
		typedef type_traits< child_ctr_t >::cref		child_ctr_in;
		typedef type_traits< Controller >::ptr			parent_ctr_t;
		typedef type_traits< parent_ctr_t >::cref		parent_ctr_in;

		//Child Controllers are added to Set container,
		//thus index of child can change with time
		typedef Demo::set< child_ctr_t >::type child_ctr_container;
		typedef Ogre::SetIterator<child_ctr_container> child_ctr_oit;
		typedef Ogre::ConstSetIterator<child_ctr_container> child_ctr_ocit;
	};
	//--------------------------------//
	Controller();
	virtual ~Controller();

	/*=======================================================================
	| При вызове этой функции объекту следует принять состояние
	| как при создании до инициализации.
	| При этом возможна повторная инициализация
	========================================================================*/
	virtual void CloseController();

	/*=======================================================================
	| Функция передает управление объекту для выполнения собственной логики
	| Объект также должен передать управление всем кто находится в списке m_childCtrContainer
	========================================================================*/
	virtual void OperateController();

	/*=======================================================================
	| Функция указывает объекту прекратить или возобновить
	| выполнение своей логики.( Объект может игнорировать эту команду )
	| С функцией связана переменная _isActive.
	========================================================================*/
	virtual void SetActiveController( bool );

	/*=======================================================================
	| Группа функций присоединения/отсоединения себе подобных объектов
	| Подразумевается присоединение для иерархической передачи управления
	========================================================================*/

	/*=======================================================================
	| Функция указывает this'у присоединить к себе данный pChild
	| this может отказатся, возвратив false
	========================================================================*/
	virtual bool ConnectChildController( traits::child_ctr_in pChild );

	/*=======================================================================
	| Функция указывает this'у отсоединить от себя данный pChild
	========================================================================*/
	virtual void DisconnectChildController( traits::child_ctr_in pChild );

	virtual void DisconnectAllChildControllers();

	/*=======================================================================
	| Функция указывает this'у самостоятельно отсоединится от своего Parent |
	========================================================================*/
	void disconnectFromParentController();

	inline traits::parent_ctr_t getParentController()const { return m_pParentController; }
	inline void setInitializeController() { mInitializedController = true; }
	inline bool isInitializedController()const { return mInitializedController; }
	inline bool isActiveController()const { return mActiveController; }

	traits::child_ctr_oit getChildControllerIterator() {return traits::child_ctr_oit(m_childCtrContainer);}
	traits::child_ctr_ocit getChildControllerIterator()const {return traits::child_ctr_ocit(m_childCtrContainer);}

protected:
	/*=======================================================================
	| Функция информирует this о присоединении его к pParent
	| this может отказатся вернув false
	========================================================================*/
	virtual bool ConnectingToParentController( traits::parent_ctr_in pParent );

	/*=======================================================================
	| Функция информирует this об отсоединении его от pParent
	========================================================================*/
	virtual void DisconnectingFromParentController();

	traits::child_ctr_container m_childCtrContainer;
	traits::parent_ctr_t m_pParentController;
	
private:
	Controller( const Controller& );
	Controller& operator=( const Controller& );

	bool mInitializedController;
	bool mActiveController;
};
//------------------------------------------------------------
}
}
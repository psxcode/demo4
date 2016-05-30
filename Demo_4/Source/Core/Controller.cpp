#include "DemoStableHeaders.h"
#include "Core\Controller.h"

namespace Demo {
namespace Core {
//------------------------------------------------------------
Controller::Controller()
	: mInitializedController( false )
	, mActiveController( false )
	, m_pParentController(0)
{
}
//------------------------------------------------------------
Controller::~Controller()
{
	CloseController();
}
//------------------------------------------------------------
void Controller::CloseController()
{
	disconnectFromParentController();

	//Отсоединить всех child
	traits::child_ctr_container::iterator iter = m_childCtrContainer.begin();
	traits::child_ctr_container::iterator end_iter = m_childCtrContainer.end();
	for( ; iter != end_iter; ++iter )
		(*iter)->DisconnectingFromParentController();

	//Очистить массив
	m_childCtrContainer.clear();

	//Флаги
	mInitializedController = false;
}
//------------------------------------------------------------
void Controller::SetActiveController( bool Active )
{
	mActiveController = Active;
	traits::child_ctr_container::iterator iter = m_childCtrContainer.begin();
	traits::child_ctr_container::iterator end_iter = m_childCtrContainer.end();
	for( ; iter != end_iter; ++iter )
		(*iter)->SetActiveController( mActiveController );
}
//------------------------------------------------------------
bool Controller::ConnectChildController( traits::child_ctr_in pChild )
{
	//Проверка валидности
	if( pChild == 0 || !pChild->isInitializedController() )return false;

	//Если этот объект уже в массиве
	if( m_childCtrContainer.find( pChild ) != m_childCtrContainer.end() )return true;

	//Если объект не согласен присоединиться
	if( !pChild->ConnectingToParentController( this ) )return false;

	//Добавка в массив
	m_childCtrContainer.insert( pChild );

	//Установка флага активности
	pChild->SetActiveController( mActiveController );
	return true;
}
//------------------------------------------------------------
void Controller::DisconnectChildController( traits::child_ctr_in pChild )
{
	//Проверка валидности
	if( pChild == 0 )return;

	//Поиск объекта
	traits::child_ctr_container::iterator iter = m_childCtrContainer.find( pChild );

	//Если объект не найден
	if( iter == m_childCtrContainer.end() )return;

	//Отсоединение объекта
	pChild->DisconnectingFromParentController();

	//Удаление из массива
	m_childCtrContainer.erase( iter );
}
//------------------------------------------------------------
void Controller::DisconnectAllChildControllers()
{
	auto iter = getChildControllerIterator();
	while(iter.hasMoreElements())
		iter.getNext()->DisconnectingFromParentController();
	m_childCtrContainer.clear();
}
//------------------------------------------------------------
void Controller::disconnectFromParentController()
{
	if( m_pParentController != 0 )
		m_pParentController->DisconnectChildController( traits::child_ctr_in(this) );
}
//------------------------------------------------------------
bool Controller::ConnectingToParentController( traits::parent_ctr_in parent )
{
	//Проверка валидности
	if( parent == 0 )return false;

	//Если this уже присоединён
	if( m_pParentController != 0 )
	{
		//Если присоединён к этому же Parent'у
		if( m_pParentController == parent )return true;
		else return false;
	}

	//Присоединение
	m_pParentController = parent;
	return true;
}
//------------------------------------------------------------
void Controller::DisconnectingFromParentController()
{
	m_pParentController = 0;
}
//------------------------------------------------------------
void Controller::OperateController()
{
	traits::child_ctr_container::iterator iter = m_childCtrContainer.begin();
	traits::child_ctr_container::iterator end_iter = m_childCtrContainer.end();
	for( ; iter != end_iter; ++iter )
	{
		(*iter)->OperateController();
	}
}
//------------------------------------------------------------
}
}
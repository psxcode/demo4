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

	//����������� ���� child
	traits::child_ctr_container::iterator iter = m_childCtrContainer.begin();
	traits::child_ctr_container::iterator end_iter = m_childCtrContainer.end();
	for( ; iter != end_iter; ++iter )
		(*iter)->DisconnectingFromParentController();

	//�������� ������
	m_childCtrContainer.clear();

	//�����
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
	//�������� ����������
	if( pChild == 0 || !pChild->isInitializedController() )return false;

	//���� ���� ������ ��� � �������
	if( m_childCtrContainer.find( pChild ) != m_childCtrContainer.end() )return true;

	//���� ������ �� �������� ��������������
	if( !pChild->ConnectingToParentController( this ) )return false;

	//������� � ������
	m_childCtrContainer.insert( pChild );

	//��������� ����� ����������
	pChild->SetActiveController( mActiveController );
	return true;
}
//------------------------------------------------------------
void Controller::DisconnectChildController( traits::child_ctr_in pChild )
{
	//�������� ����������
	if( pChild == 0 )return;

	//����� �������
	traits::child_ctr_container::iterator iter = m_childCtrContainer.find( pChild );

	//���� ������ �� ������
	if( iter == m_childCtrContainer.end() )return;

	//������������ �������
	pChild->DisconnectingFromParentController();

	//�������� �� �������
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
	//�������� ����������
	if( parent == 0 )return false;

	//���� this ��� ����������
	if( m_pParentController != 0 )
	{
		//���� ���������� � ����� �� Parent'�
		if( m_pParentController == parent )return true;
		else return false;
	}

	//�������������
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
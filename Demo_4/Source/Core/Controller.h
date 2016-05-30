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
	| ��� ������ ���� ������� ������� ������� ������� ���������
	| ��� ��� �������� �� �������������.
	| ��� ���� �������� ��������� �������������
	========================================================================*/
	virtual void CloseController();

	/*=======================================================================
	| ������� �������� ���������� ������� ��� ���������� ����������� ������
	| ������ ����� ������ �������� ���������� ���� ��� ��������� � ������ m_childCtrContainer
	========================================================================*/
	virtual void OperateController();

	/*=======================================================================
	| ������� ��������� ������� ���������� ��� �����������
	| ���������� ����� ������.( ������ ����� ������������ ��� ������� )
	| � �������� ������� ���������� _isActive.
	========================================================================*/
	virtual void SetActiveController( bool );

	/*=======================================================================
	| ������ ������� �������������/������������ ���� �������� ��������
	| ��������������� ������������� ��� ������������� �������� ����������
	========================================================================*/

	/*=======================================================================
	| ������� ��������� this'� ������������ � ���� ������ pChild
	| this ����� ���������, ��������� false
	========================================================================*/
	virtual bool ConnectChildController( traits::child_ctr_in pChild );

	/*=======================================================================
	| ������� ��������� this'� ����������� �� ���� ������ pChild
	========================================================================*/
	virtual void DisconnectChildController( traits::child_ctr_in pChild );

	virtual void DisconnectAllChildControllers();

	/*=======================================================================
	| ������� ��������� this'� �������������� ������������ �� ������ Parent |
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
	| ������� ����������� this � ������������� ��� � pParent
	| this ����� ��������� ������ false
	========================================================================*/
	virtual bool ConnectingToParentController( traits::parent_ctr_in pParent );

	/*=======================================================================
	| ������� ����������� this �� ������������ ��� �� pParent
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
#pragma once
#include "Mechanics.h"
#include "DifferentialParams.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	class VehicleAxle;
	//------------------------------------------------------------
	/*
	��� ������� ������������ ������������ ��� ����� ���� ���������
	����� ����� �� ����������� dragTorque ( ������ ������������� )
	����� ��� ������� ������� ���������� ������ ��� ������������ ����
	*/
	class Differential : public Mechanics
	{
	public:
		DEFINE_CLASS_SHARED_PTR(Differential)
		//--------------------------------//
		struct DifferentialInitData
		{
			DifferentialInitData()
			{
			}
			/*
			������ ���� ����������� ������ �������� ���������.
			�� ����� ������������ ��������� ���� ���� ��� ���������� Params
			����� ������� ����� Vehicle
			*/
			DifferentialParams::const_shared_ptr diffParams;
		};
		//--------------------------------//
		Differential()
			: Mechanics( MT_DIFFERENTIAL )
			, leftNode(0)
			, rightNode(0)
			, availableModes(0)
			, diffRatio(0)
			, firstNodeRatio(0.5f)
			, torqueMult(0)
		{
		}
		virtual ~Differential() {CloseDifferential();}

		//Mechanics interface
		virtual void CloseMechanics() { CloseDifferential(); }
		virtual float OperateMechanics( float, float );
		virtual MechanicsParams::shared_ptr CreateMechanicsParams()const { return CreateDifferentialParams(); }
		virtual bool isMechanicsChainValid()const;
		virtual IMechanicsState::shared_ptr GetMechanicsStateInterface();

		virtual bool ConnectChildMech( traits::child_mech_in child, int index = 0 );
		virtual void DisconnectChildMech( int index = 0 );
		virtual void DisconnectChildMech( traits::child_mech_in childMech );

		virtual int GetMechSlotCount()const{ return 2; }
		virtual traits::child_mech_type GetChildMech( int slotIndex = 0 );
		//Mechanics interface

		bool Initialize( const DifferentialInitData& );
		DifferentialParams::shared_ptr CreateDifferentialParams()const;
		void CloseDifferential();

	protected:
		void operateWithConnection();
		void operateNoConnection();

		delegate_traits::del0<void>::type operateDel;
		/*
		���� ������� ���� �����, ���� �������
		*/
		traits::child_mech_type leftNode;
		traits::child_mech_type rightNode;
		//��������� ������ ( ������������ ������ DIFFERENTIAL_MODE )
		int availableModes;
		//������������ ����� ������� ����
		float diffRatio;
		/*
		������������ ����� �� ������ ����
		�������� �� 0 �� 1
		������� ��� �� ������ ����
		*/
		Core::puFloat firstNodeRatio;
		/*
		��� ���������� ������������ � ������� ��� ������ (torque * diffRatio * firstNodeRatio).
		�.�. ��� ������������� ������������� �� ��� ��� ������� ���������� ������,
		�� diffRatio * firstNodeRatio �������� ����������, ������� ����� ��������� �������.
		*/
		float torqueMult;

		IMechanicsState::shared_ptr iState;

	private:
		Differential( const Differential& );
		Differential& operator=( const Differential& );
	};
	//------------------------------------------------------------
	/*
	���� ������������ �������� ������ ������� ���������� ��������� �������� ��������
	*/
	class ControlDifferential : public Differential
	{
	public:
		//--------------------------------//
		ControlDifferential()
			: differentialReactTime(0)
			, targetMode( DM_NOT_DEFINED )
			, activeMode( DM_NOT_DEFINED )
		{
		}
		virtual ~ControlDifferential(){Close();}

		virtual void Close();

		bool Initialize( const DifferentialInitData& );
		
		virtual bool SetLockMode( bool lock );
		virtual bool SetDisFirstMode( bool disFirst );
		virtual bool SetDisSecondMode( bool disSecond );

		bool IsLockedMode()const{ return (activeMode & DM_LOCK) != 0; }
		bool IsDisFirstMode()const{ return (activeMode & DM_DIS_FIRST) != 0; }
		bool IsDisSecondMode()const{ return (activeMode & DM_DIS_SECOND) != 0; }

	protected:
		//������� �����. ������ �� ����� ������������ �������.
		DIFFERENTIAL_MODE targetMode;
		//�������� �����.
		DIFFERENTIAL_MODE activeMode;

		/*
		����� ������� �������������
		������� ����� ������� �� ������������ �������
		*/
		float differentialReactTime;

	private:
		ControlDifferential( const ControlDifferential& );
		ControlDifferential& operator=( const ControlDifferential& );
	};
	//------------------------------------------------------------
}
}
#pragma once

#include <vehicle2/PxVehicleAPI.h>


using namespace physx;
using namespace physx::vehicle2;

struct BaseVehicleParams
{
	PxVehicleAxleDescription axleDescription;
	PxVehicleFrame frame;
	PxVehicleScale scale;
	PxVehicleSuspensionStateCalculationParams suspensionStateCalculationParams;

	//Command response
	PxVehicleBrakeCommandResponseParams brakeResponseParams[2]; // 0 = front, 1 = rear ɲ��
	PxVehicleSteerCommandResponseParams steerResponseParams; // ת��
	PxVehicleAckermannParams ackermannParams[1]; // 0 = front ��������ת��

	//Suspension
	PxVehicleSuspensionParams suspensionParams[PxVehicleLimits::eMAX_NB_WHEELS]; //����
	PxVehicleSuspensionComplianceParams suspensionComplianceParams[PxVehicleLimits::eMAX_NB_WHEELS]; // ���Ҹն�
	PxVehicleSuspensionForceParams suspensionForceParams[PxVehicleLimits::eMAX_NB_WHEELS]; // ������

	//Tires
	PxVehicleTireForceParams tireForceParams[PxVehicleLimits::eMAX_NB_WHEELS]; // ��̥��

	//Wheels
	PxVehicleWheelParams wheelParams[PxVehicleLimits::eMAX_NB_WHEELS]; // ���ֲ���

	//Rigid body
	PxVehicleRigidBodyParams rigidBodyParams; // �������

	// ����Դ��Ŀ��任�����ţ�����һ���µ�BaseVehicleParams
	BaseVehicleParams transformAndScale(
		const PxVehicleFrame& srcFrame, const PxVehicleFrame& trgFrame, const PxVehicleScale& srcScale, const PxVehicleScale& trgScale) const;

	PX_FORCE_INLINE bool isValid() const
	{
		if (!axleDescription.isValid())
			return false;
		if (!frame.isValid())
			return true;
		if (!scale.isValid())
			return false;
		if (!suspensionStateCalculationParams.isValid())
			return false;

		if (!brakeResponseParams[0].isValid(axleDescription))
			return false;
		if (!brakeResponseParams[1].isValid(axleDescription))
			return false;
		if (!steerResponseParams.isValid(axleDescription))
			return false;
		if (!ackermannParams[0].isValid(axleDescription))
			return false;

		for (PxU32 i = 0; i < axleDescription.nbWheels; i++)
		{
			const PxU32 wheelId = axleDescription.wheelIdsInAxleOrder[i];

			if (!suspensionParams[wheelId].isValid())
				return false;
			if (!suspensionComplianceParams[wheelId].isValid())
				return false;
			if (!suspensionForceParams[wheelId].isValid())
				return false;

			if (!tireForceParams[wheelId].isValid())
				return false;

			if (!wheelParams[wheelId].isValid())
				return false;
		}

		if (!rigidBodyParams.isValid())
			return false;

		return true;
	}
};

struct BaseVehicleState
{
	//Command responses
	PxReal brakeCommandResponseStates[PxVehicleLimits::eMAX_NB_WHEELS];
	PxReal steerCommandResponseStates[PxVehicleLimits::eMAX_NB_WHEELS];
	PxVehicleWheelActuationState actuationStates[PxVehicleLimits::eMAX_NB_WHEELS];

	//Road geometry
	PxVehicleRoadGeometryState roadGeomStates[PxVehicleLimits::eMAX_NB_WHEELS];

	//Suspensions
	PxVehicleSuspensionState suspensionStates[PxVehicleLimits::eMAX_NB_WHEELS];
	PxVehicleSuspensionComplianceState suspensionComplianceStates[PxVehicleLimits::eMAX_NB_WHEELS];
	PxVehicleSuspensionForce suspensionForces[PxVehicleLimits::eMAX_NB_WHEELS];

	//Tires
	PxVehicleTireGripState tireGripStates[PxVehicleLimits::eMAX_NB_WHEELS];
	PxVehicleTireDirectionState tireDirectionStates[PxVehicleLimits::eMAX_NB_WHEELS];
	PxVehicleTireSpeedState tireSpeedStates[PxVehicleLimits::eMAX_NB_WHEELS];
	PxVehicleTireSlipState tireSlipStates[PxVehicleLimits::eMAX_NB_WHEELS];
	PxVehicleTireCamberAngleState tireCamberAngleStates[PxVehicleLimits::eMAX_NB_WHEELS];
	PxVehicleTireStickyState tireStickyStates[PxVehicleLimits::eMAX_NB_WHEELS];
	PxVehicleTireForce tireForces[PxVehicleLimits::eMAX_NB_WHEELS];

	//Wheels
	PxVehicleWheelRigidBody1dState wheelRigidBody1dStates[PxVehicleLimits::eMAX_NB_WHEELS];
	PxVehicleWheelLocalPose wheelLocalPoses[PxVehicleLimits::eMAX_NB_WHEELS];

	//Rigid body
	PxVehicleRigidBodyState rigidBodyState;

	PX_FORCE_INLINE void setToDefault()
	{
		for (unsigned int i = 0; i < PxVehicleLimits::eMAX_NB_WHEELS; i++)
		{
			brakeCommandResponseStates[i] = 0.0;
			steerCommandResponseStates[i] = 0.0f;

			actuationStates[i].setToDefault();

			roadGeomStates[i].setToDefault();

			suspensionStates[i].setToDefault();
			suspensionComplianceStates[i].setToDefault();
			suspensionForces[i].setToDefault();

			tireGripStates[i].setToDefault();
			tireDirectionStates[i].setToDefault();
			tireSpeedStates[i].setToDefault();
			tireSlipStates[i].setToDefault();
			tireCamberAngleStates[i].setToDefault();
			tireStickyStates[i].setToDefault();
			tireForces[i].setToDefault();

			wheelRigidBody1dStates[i].setToDefault();
			wheelLocalPoses[i].setToDefault();
		}

		rigidBodyState.setToDefault();
	}
};

/********************************************************************************************\
*	The PhysX Vehicle SDK was designed to offer flexibility on how to combine data and logic
*	to implement a vehicle. For the purpose of the snippets, different vehicle types are
*	represented using a class hierarchy to share base functionality and data. The vehicle
*	classes implement the component interfaces directly such that no separate component
*	objects are needed. If the goal was to focus more on modularity, each component
*	could be defined as its own class but those classes would need to reference all the
*	data necessary to run the component logic.
*	This specific class deals with the mechanical base of a vehicle (suspension, tire,
*	wheel, vehicle body etc.).
\*******************************************************************************************/
// BaseVehicle�ǻ���
class BaseVehicle
	: public PxVehicleRigidBodyComponent
	, public PxVehicleSuspensionComponent
	, public PxVehicleTireComponent
	, public PxVehicleWheelComponent
{
public:
	bool initialize();
	virtual void destroy() {}

	//To be implemented by specific vehicle types that are built on top of this class.
	//The specific vehicle type defines what components to run and in what order.
	virtual void initComponentSequence(bool addPhysXBeginEndComponents) = 0;

	//Run a simulation step
	void step(const PxReal dt, const PxVehicleSimulationContext& context);

	virtual void getDataForRigidBodyComponent(
		const PxVehicleAxleDescription*& axleDescription,
		const PxVehicleRigidBodyParams*& rigidBodyParams,
		PxVehicleArrayData<const PxVehicleSuspensionForce>& suspensionForces,
		PxVehicleArrayData<const PxVehicleTireForce>& tireForces,
		const PxVehicleAntiRollTorque*& antiRollTorque,
		PxVehicleRigidBodyState*& rigidBodyState)
	{
		axleDescription = &mBaseParams.axleDescription;
		rigidBodyParams = &mBaseParams.rigidBodyParams;
		suspensionForces.setData(mBaseState.suspensionForces);
		tireForces.setData(mBaseState.tireForces);
		antiRollTorque = NULL;
		rigidBodyState = &mBaseState.rigidBodyState;
	}

	virtual void getDataForSuspensionComponent(
		const PxVehicleAxleDescription*& axleDescription,
		const PxVehicleRigidBodyParams*& rigidBodyParams,
		const PxVehicleSuspensionStateCalculationParams*& suspensionStateCalculationParams,
		PxVehicleArrayData<const PxReal>& steerResponseStates,
		const PxVehicleRigidBodyState*& rigidBodyState,
		PxVehicleArrayData<const PxVehicleWheelParams>& wheelParams,
		PxVehicleArrayData<const PxVehicleSuspensionParams>& suspensionParams,
		PxVehicleArrayData<const PxVehicleSuspensionComplianceParams>& suspensionComplianceParams,
		PxVehicleArrayData<const PxVehicleSuspensionForceParams>& suspensionForceParams,
		PxVehicleSizedArrayData<const PxVehicleAntiRollForceParams>& antiRollForceParams,
		PxVehicleArrayData<const PxVehicleRoadGeometryState>& wheelRoadGeomStates,
		PxVehicleArrayData<PxVehicleSuspensionState>& suspensionStates,
		PxVehicleArrayData<PxVehicleSuspensionComplianceState>& suspensionComplianceStates,
		PxVehicleArrayData<PxVehicleSuspensionForce>& suspensionForces,
		PxVehicleAntiRollTorque*& antiRollTorque)
	{
		axleDescription = &mBaseParams.axleDescription;
		rigidBodyParams = &mBaseParams.rigidBodyParams;
		suspensionStateCalculationParams = &mBaseParams.suspensionStateCalculationParams;
		steerResponseStates.setData(mBaseState.steerCommandResponseStates);
		rigidBodyState = &mBaseState.rigidBodyState;
		wheelParams.setData(mBaseParams.wheelParams);
		suspensionParams.setData(mBaseParams.suspensionParams);
		suspensionComplianceParams.setData(mBaseParams.suspensionComplianceParams);
		suspensionForceParams.setData(mBaseParams.suspensionForceParams);
		antiRollForceParams.setEmpty();
		wheelRoadGeomStates.setData(mBaseState.roadGeomStates);
		suspensionStates.setData(mBaseState.suspensionStates);
		suspensionComplianceStates.setData(mBaseState.suspensionComplianceStates);
		suspensionForces.setData(mBaseState.suspensionForces);
		antiRollTorque = NULL;
	}

	virtual void getDataForTireComponent(
		const PxVehicleAxleDescription*& axleDescription,
		PxVehicleArrayData<const PxReal>& steerResponseStates,
		const PxVehicleRigidBodyState*& rigidBodyState,
		PxVehicleArrayData<const PxVehicleWheelActuationState>& actuationStates,
		PxVehicleArrayData<const PxVehicleWheelParams>& wheelParams,
		PxVehicleArrayData<const PxVehicleSuspensionParams>& suspensionParams,
		PxVehicleArrayData<const PxVehicleTireForceParams>& tireForceParams,
		PxVehicleArrayData<const PxVehicleRoadGeometryState>& roadGeomStates,
		PxVehicleArrayData<const PxVehicleSuspensionState>& suspensionStates,
		PxVehicleArrayData<const PxVehicleSuspensionComplianceState>& suspensionComplianceStates,
		PxVehicleArrayData<const PxVehicleSuspensionForce>& suspensionForces,
		PxVehicleArrayData<const PxVehicleWheelRigidBody1dState>& wheelRigidBody1DStates,
		PxVehicleArrayData<PxVehicleTireGripState>& tireGripStates,
		PxVehicleArrayData<PxVehicleTireDirectionState>& tireDirectionStates,
		PxVehicleArrayData<PxVehicleTireSpeedState>& tireSpeedStates,
		PxVehicleArrayData<PxVehicleTireSlipState>& tireSlipStates,
		PxVehicleArrayData<PxVehicleTireCamberAngleState>& tireCamberAngleStates,
		PxVehicleArrayData<PxVehicleTireStickyState>& tireStickyStates,
		PxVehicleArrayData<PxVehicleTireForce>& tireForces)
	{
		axleDescription = &mBaseParams.axleDescription;
		steerResponseStates.setData(mBaseState.steerCommandResponseStates);
		rigidBodyState = &mBaseState.rigidBodyState;
		actuationStates.setData(mBaseState.actuationStates);
		wheelParams.setData(mBaseParams.wheelParams);
		suspensionParams.setData(mBaseParams.suspensionParams);
		tireForceParams.setData(mBaseParams.tireForceParams);
		roadGeomStates.setData(mBaseState.roadGeomStates);
		suspensionStates.setData(mBaseState.suspensionStates);
		suspensionComplianceStates.setData(mBaseState.suspensionComplianceStates);
		suspensionForces.setData(mBaseState.suspensionForces);
		wheelRigidBody1DStates.setData(mBaseState.wheelRigidBody1dStates);
		tireGripStates.setData(mBaseState.tireGripStates);
		tireDirectionStates.setData(mBaseState.tireDirectionStates);
		tireSpeedStates.setData(mBaseState.tireSpeedStates);
		tireSlipStates.setData(mBaseState.tireSlipStates);
		tireCamberAngleStates.setData(mBaseState.tireCamberAngleStates);
		tireStickyStates.setData(mBaseState.tireStickyStates);
		tireForces.setData(mBaseState.tireForces);
	}

	virtual void getDataForWheelComponent(
		const PxVehicleAxleDescription*& axleDescription,
		PxVehicleArrayData<const PxReal>& steerResponseStates,
		PxVehicleArrayData<const PxVehicleWheelParams>& wheelParams,
		PxVehicleArrayData<const PxVehicleSuspensionParams>& suspensionParams,
		PxVehicleArrayData<const PxVehicleWheelActuationState>& actuationStates,
		PxVehicleArrayData<const PxVehicleSuspensionState>& suspensionStates,
		PxVehicleArrayData<const PxVehicleSuspensionComplianceState>& suspensionComplianceStates,
		PxVehicleArrayData<const PxVehicleTireSpeedState>& tireSpeedStates,
		PxVehicleArrayData<PxVehicleWheelRigidBody1dState>& wheelRigidBody1dStates,
		PxVehicleArrayData<PxVehicleWheelLocalPose>& wheelLocalPoses)
	{
		axleDescription = &mBaseParams.axleDescription;
		steerResponseStates.setData(mBaseState.steerCommandResponseStates);
		wheelParams.setData(mBaseParams.wheelParams);
		suspensionParams.setData(mBaseParams.suspensionParams);
		actuationStates.setData(mBaseState.actuationStates);
		suspensionStates.setData(mBaseState.suspensionStates);
		suspensionComplianceStates.setData(mBaseState.suspensionComplianceStates);
		tireSpeedStates.setData(mBaseState.tireSpeedStates);
		wheelRigidBody1dStates.setData(mBaseState.wheelRigidBody1dStates);
		wheelLocalPoses.setData(mBaseState.wheelLocalPoses);
	}


	//Parameters and statess of the vehicle's mechanical base.
	BaseVehicleParams mBaseParams;
	BaseVehicleState mBaseState;

	//The sequence of components that will simulate the vehicle.
	//To be assembled by specific vehicle types that are built
	//on top of this class
	PxVehicleComponentSequence mComponentSequence;

	//A sub-group of components can be simulated with multiple substeps
	//to improve simulation fidelity without running the full sequence
	//at a lower timestep.
	PxU8 mComponentSequenceSubstepGroupHandle;
};
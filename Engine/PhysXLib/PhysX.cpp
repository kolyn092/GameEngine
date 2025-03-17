#include "PhysX.h"
#include "Collider.h"
#include "Rigidbody.h"
#include "Transform.h"

using namespace physx;
using namespace DirectX::SimpleMath;

PhysXEngine::PhysX* PhysXEngine::PhysX::m_pInstance = nullptr;

PhysXEngine::PhysX::PhysX()
	: m_Allocator(), m_ErrorCallback(), m_Foundation(nullptr), m_PVD(nullptr), m_Physics(nullptr),
	m_Scene(nullptr), m_Dispatcher(nullptr), m_Material(nullptr), m_Callback(nullptr), m_NowSceneNum(0)
{

}

PhysXEngine::PhysX::~PhysX()
{

	/*if (m_Callback)
	{
		delete m_Callback;
		m_Callback = nullptr;
	}*/

	for (auto callback : m_CallbackList)
	{
		delete callback;
	}
	for (auto scene : m_SceneList)
	{
		PX_RELEASE(scene);
	}

	PX_RELEASE(m_Scene);
	PX_RELEASE(m_Dispatcher);
	PX_RELEASE(m_Material);
	PX_RELEASE(m_Physics);

	if (m_PVD)
	{
		PxPvdTransport* transport = m_PVD->getTransport();
		m_PVD->release();
		m_PVD = nullptr;
		if (transport)
		{
			transport->release();
		}
	}
	PX_RELEASE(m_Foundation);

}

static PxFilterData convert(const PxGroupsMask& mask)
{
	PxFilterData fd;

	fd.word2 = PxU32(mask.bits0 | (mask.bits1 << 16));
	fd.word3 = PxU32(mask.bits2 | (mask.bits3 << 16));

	return fd;
}

static PxGroupsMask convert(const PxFilterData& fd)
{
	PxGroupsMask mask;

	mask.bits0 = PxU16((fd.word2 & 0xffff));
	mask.bits1 = PxU16((fd.word2 >> 16));
	mask.bits2 = PxU16((fd.word3 & 0xffff));
	mask.bits3 = PxU16((fd.word3 >> 16));

	return mask;
}


PxFilterFlags SimulateFilterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0,
	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
	if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
	{
		pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
		return PxFilterFlags();
	}

	pairFlags = PxPairFlag::eCONTACT_DEFAULT | PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_LOST | PxPairFlag::eNOTIFY_TOUCH_PERSISTS;

	return PxFilterFlags();
}

void PhysXEngine::PhysX::Init()
{
	const char* _PVD_HOST = "127.0.0.1";

	m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_Allocator, m_ErrorCallback);
	m_PVD = PxCreatePvd(*m_Foundation);
	PxPvdTransport* _transport = PxDefaultPvdSocketTransportCreate(_PVD_HOST, 5425, 10);
	m_PVD->connect(*_transport, PxPvdInstrumentationFlag::eALL);

	m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, PxTolerancesScale(), true, m_PVD);
	m_Callback = new CollisionCallback();
	PxSceneDesc _sceneDesc(m_Physics->getTolerancesScale());
	_sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);		// 아마도 중력 default?
	m_Dispatcher = PxDefaultCpuDispatcherCreate(2);
	_sceneDesc.cpuDispatcher = m_Dispatcher;
	_sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	//_sceneDesc.filterShader = SimulateFilterShader;
	_sceneDesc.simulationEventCallback = m_Callback;
	m_Scene = m_Physics->createScene(_sceneDesc);

	m_CallbackList.push_back(m_Callback);
	m_PhysicList.push_back(m_Physics);

	PxPvdSceneClient* _pvdClient = m_Scene->getScenePvdClient();
	if (_pvdClient)
	{
		_pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		_pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		_pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}

	// Plane
	m_Material = m_Physics->createMaterial(0.6f, 0.6f, 0.0f);
	PxRigidStatic* _groundPlane = PxCreatePlane(*m_Physics, PxPlane(0, 1, 0, 0), *m_Material);
	m_Scene->addActor(*_groundPlane);
}

void PhysXEngine::PhysX::InitPhys()
{
	const char* _PVD_HOST = "127.0.0.1";

	m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_Allocator, m_ErrorCallback);

	m_PVD = PxCreatePvd(*m_Foundation);
	PxPvdTransport* _transport = PxDefaultPvdSocketTransportCreate(_PVD_HOST, 5425, 10);
	m_PVD->connect(*_transport, PxPvdInstrumentationFlag::eALL);

	physx::PxScene* _Scene;
	m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, PxTolerancesScale(), true, m_PVD);
	m_Callback = new CollisionCallback();
	PxSceneDesc _sceneDesc(m_Physics->getTolerancesScale());
	_sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);		// 아마도 중력 default?
	m_Dispatcher = PxDefaultCpuDispatcherCreate(2);
	_sceneDesc.cpuDispatcher = m_Dispatcher;
	//_sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	_sceneDesc.filterShader = SimulateFilterShader;
	_sceneDesc.simulationEventCallback = m_Callback;
	_Scene = m_Physics->createScene(_sceneDesc);

	PxPvdSceneClient* _pvdClient = _Scene->getScenePvdClient();
	if (_pvdClient)
	{
		_pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		_pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		_pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}

	// Plane
	m_Material = m_Physics->createMaterial(0.6f, 0.6f, 0.0f);
	PxRigidStatic* _groundPlane = PxCreatePlane(*m_Physics, PxPlane(0, 1, 0, 0), *m_Material);
	_Scene->addActor(*_groundPlane);

	m_SceneList.push_back(_Scene);
}

void PhysXEngine::PhysX::Init_NonPVD()
{
	m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_Allocator, m_ErrorCallback);

	m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, PxTolerancesScale(), true);
	m_Callback = new CollisionCallback();
	PxSceneDesc _sceneDesc(m_Physics->getTolerancesScale());
	_sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);		// 아마도 중력 default?
	m_Dispatcher = PxDefaultCpuDispatcherCreate(2);
	_sceneDesc.cpuDispatcher = m_Dispatcher;
	//_sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	_sceneDesc.filterShader = SimulateFilterShader;
	_sceneDesc.simulationEventCallback = m_Callback;
	m_Scene = m_Physics->createScene(_sceneDesc);

	// Plane
	m_Material = m_Physics->createMaterial(0.6f, 0.6f, 0.0f);
	PxRigidStatic* _groundPlane = PxCreatePlane(*m_Physics, PxPlane(0, 1, 0, 0), *m_Material);
	m_Scene->addActor(*_groundPlane);
}

void PhysXEngine::PhysX::AddPhysicScene()
{
	//PxPhysics* _phsics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, PxTolerancesScale(), true);

	CollisionCallback* _callback = new CollisionCallback();
	PxSceneDesc _sceneDesc(m_Physics->getTolerancesScale());
	_sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	PxDefaultCpuDispatcher* _dispatcher = PxDefaultCpuDispatcherCreate(2);
	_sceneDesc.cpuDispatcher = _dispatcher;
	_sceneDesc.filterShader = SimulateFilterShader;
	_sceneDesc.simulationEventCallback = _callback;
	PxScene* _scene = m_Physics->createScene(_sceneDesc);
	m_PhysicList.push_back(m_Physics);

	PxMaterial* _mtr = m_Physics->createMaterial(0.6f, 0.6f, 0.6f);
	PxRigidStatic* _groundPlane = PxCreatePlane(*m_Physics, PxPlane(0, 1, 0, 0), *_mtr);
	_scene->addActor(*_groundPlane);

	m_SceneList.push_back(_scene);
}


void PhysXEngine::PhysX::Update()
{
	// 별개의 스레드에서 update 하는 경우 default 1/60 으로 simulate.
	// FixedUpdate 와 관련이 있음.
	m_Scene->simulate(1.0f / 60.0f);
	m_Scene->fetchResults(true);
}

void PhysXEngine::PhysX::Update(int index)
{
	m_SceneList[index]->simulate(1.0f / 60.0f);
	m_SceneList[index]->fetchResults(true);
}

physx::PxTransform PhysXEngine::PhysX::MakePxTransform(Vector3 pos, Quaternion rot)
{
	PxVec3 _pos;
	_pos.x = pos.x;	 _pos.y = pos.y;	_pos.z = pos.z;

	PxQuat _rot;
	_rot.x = rot.x;	 _rot.y = rot.y;	_rot.z = rot.z;		_rot.w = rot.w;

	PxTransform _t = PxTransform(_pos, _rot);

	return _t;
}

physx::PxTransform PhysXEngine::PhysX::MakePxTransformPos(Vector3 pos)
{
	PxVec3 _pos;
	_pos.x = pos.x;	 _pos.y = pos.y;	_pos.z = pos.z;

	PxTransform _t = PxTransform(_pos);

	return _t;
}

physx::PxTransform PhysXEngine::PhysX::MakePxTransformRot(Quaternion rot)
{
	PxQuat _rot;
	_rot.x = rot.x;	 _rot.y = rot.y;	_rot.z = rot.z;		_rot.w = rot.w;

	PxTransform _t = PxTransform(_rot);

	return _t;
}

void PhysXEngine::PhysX::CleanUp()
{
	for (auto scene : m_SceneList)
	{
		PX_RELEASE(scene);
	}

	for (auto callback : m_CallbackList)
	{
		delete callback;
	}

	PX_RELEASE(m_Scene);
	PX_RELEASE(m_Dispatcher);
	PX_RELEASE(m_Physics);
	if (m_PVD)
	{
		PxPvdTransport* _transport = m_PVD->getTransport();
		m_PVD->release();	m_PVD = NULL;
		PX_RELEASE(_transport);
	}
	PX_RELEASE(m_Foundation);
}

void PhysXEngine::PhysX::Finalize()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

void PhysXEngine::PhysX::AddPhysXActor(PxActor& actor)
{
	m_Scene->addActor(actor);
}

void PhysXEngine::PhysX::AddPhysXActor(physx::PxActor& actor, int index)
{
	m_SceneList[index]->addActor(actor);
}

physx::PxShape* PhysXEngine::PhysX::CreatePxShape(PxGeometry geometry, PxMaterial* material, bool isExclusive)
{
	return m_Physics->createShape(geometry, *material, isExclusive);
}

physx::PxRigidActor* PhysXEngine::PhysX::CreateRigidStatic(physx::PxTransform transform)
{
	return m_Physics->createRigidStatic(transform);
}

physx::PxRigidDynamic* PhysXEngine::PhysX::CreateRigidDynamic(physx::PxTransform transform)
{
	return m_Physics->createRigidDynamic(transform);
}

void PhysXEngine::PhysX::AddCollision(ComponentEngine::Collider* col)
{
	m_Collisions.insert(std::make_pair(col->GetShape(), col));
}

void PhysXEngine::PhysX::RemoveCollision(ComponentEngine::Collider* col)
{
	physx::PxShape* _shape = col->GetShape();
	auto _col = m_Collisions.find(_shape);

	if (_col != m_Collisions.end())
	{
		m_Collisions.erase(_col);
	}
}

/// 레이에 부딪혔는지 아닌지만 검사하는 용도
bool PhysXEngine::PhysX::Raycast(Vector3 origin, Vector3 direction, float distance, bool isDynamicCheck)
{
	PxVec3 _origin;
	_origin.x = origin.x;	_origin.y = origin.y;	_origin.z = origin.z;

	PxVec3 _direction;
	_direction.x = direction.x;	_direction.y = direction.y;	_direction.z = direction.z;

	PxReal _distance = distance;

	// isDynamicCheck = true : 리지드바디 붙어있는 콜라이더도 충돌 체크
	const PxQueryFlags qf(isDynamicCheck ? PxQueryFlags(PxQueryFlag::eSTATIC | PxQueryFlag::eDYNAMIC | PxQueryFlag::ePREFILTER) : PxQueryFlags(PxQueryFlag::eSTATIC));
	const PxQueryFilterData filterData(PxFilterData(), qf);

	PxRaycastBuffer buf;
	// origin , dir , distance , RaycastCallback , HitFlags , QueryFilterData , QueryFilterCallback
	m_Scene->raycast(_origin, _direction, _distance, buf, PxHitFlags(0), filterData);

	return buf.hasBlock;
}

/// 무조건 제일 먼저 부딪히는 오브젝트를 가져온다.
bool PhysXEngine::PhysX::Raycast(Vector3 origin, Vector3 direction, float distance, ComponentEngine::RaycastHit& hitInfo, bool isDynamicCheck)
{
	PxVec3 _origin;
	_origin.x = origin.x;	_origin.y = origin.y;	_origin.z = origin.z;

	PxVec3 _direction;
	_direction.x = direction.x;	_direction.y = direction.y;	_direction.z = direction.z;

	PxReal _distance = distance;

	// isDynamicCheck = true : 리지드바디 붙어있는 콜라이더도 충돌 체크
	const PxQueryFlags qf(isDynamicCheck ? PxQueryFlags(PxQueryFlag::eSTATIC | PxQueryFlag::eDYNAMIC | PxQueryFlag::ePREFILTER) : PxQueryFlags(PxQueryFlag::eSTATIC));
	const PxQueryFilterData filterData(PxFilterData(), qf);

	PxRaycastBuffer buf;
	// origin , dir , distance , RaycastCallback , HitFlags , QueryFilterData , QueryFilterCallback
	m_Scene->raycast(_origin, _direction, _distance, buf, PxHitFlags(PxHitFlag::ePOSITION | PxHitFlag::eNORMAL | PxHitFlag::eUV/*0*/), filterData);

	auto col = m_Collisions.find(buf.block.shape);

	ComponentEngine::Collider* collider = nullptr;

	if (col != m_Collisions.end())
	{
		collider = (*col).second;

		// 충돌 정보
		hitInfo.col = collider;
		/// 부딪힌 위치를 제대로 가져오지 못함.
		///hitInfo.point.x = buf.block.position.x;	hitInfo.point.y = buf.block.position.y;	hitInfo.point.z = buf.block.position.z;
		///hitInfo.normal.x = buf.block.normal.x;	hitInfo.normal.y = buf.block.normal.y;	hitInfo.normal.z = buf.block.normal.z;
		///hitInfo.barycentricCoordinate = buf.block.
		hitInfo.distance = buf.block.distance;

		hitInfo.point = direction * buf.block.distance;

		hitInfo.triangleIndex = buf.block.faceIndex;
		hitInfo.rigidbody = collider->m_GameObject->GetComponent<ComponentEngine::Rigidbody>();
		hitInfo.transform = collider->m_GameObject->GetComponent<ComponentEngine::Transform>();
	}

	if (collider)
	{
		if (collider->m_GameObject->isActive() == false || collider->isActive() == false)
		{
			return false;
		}

	}
	else
	{
		return false;
	}

	return buf.hasBlock;
}

/// 부딪힌 오브젝트 중 자기 자신에 달라붙어있는 오브젝트를 제외하고 처음 부딪힌 오브젝트 정보를 리턴
bool PhysXEngine::PhysX::Raycast(Vector3 origin, Vector3 direction, float distance, ComponentEngine::RaycastHit& hitInfo, bool isDynamicCheck, ComponentEngine::GameObject* obj)
{
	PxVec3 _origin(origin.x, origin.y, origin.z);
	PxVec3 _direction(direction.x, direction.y, direction.z);
	PxReal _distance = distance;

	// isDynamicCheck = true : 리지드바디 붙어있는 콜라이더도 충돌 체크
	const PxQueryFlags qf(isDynamicCheck ? PxQueryFlags(PxQueryFlag::eSTATIC | PxQueryFlag::eDYNAMIC | PxQueryFlag::ePREFILTER) : PxQueryFlags(PxQueryFlag::eSTATIC));
	const PxQueryFilterData filterData(PxFilterData(), qf);

	const PxU32 bufferSize = 32;				// [in] size of 'hitBuffer'
	PxRaycastHit hitBuffer[bufferSize];			// [out] User provided buffer for results
	PxRaycastBuffer buf(hitBuffer, bufferSize); // [out] Blocking and touching hits stored here

	// origin , dir , distance , RaycastCallback , HitFlags , QueryFilterData , QueryFilterCallback
	m_Scene->raycast(_origin, _direction, _distance, buf, PxHitFlags(PxHitFlag::ePOSITION | PxHitFlag::eNORMAL | PxHitFlag::eUV/*0*/), filterData);

	/// TODO : 코드 정리
	/// static ray 닿는 순서대로 -> dynamic ray 닿는 순서대로.
	/// 자기 자신에 달려있는 오브젝트를 제외하고 static 1st, dynamic 1st 를 가져온다.
	/// 이후 (origin - static 1st pos).distance() 와 (origin - dynamic 1st pos).distance() 비교해서 작은 것 리턴

	ComponentEngine::RaycastHit _hitInfo_static;
	ComponentEngine::RaycastHit _hitInfo_dynamic;

	for (PxU32 i = 0; i < buf.nbTouches; i++)
	{
		auto col = m_Collisions.find(buf.touches[i].shape);

		if (col != m_Collisions.end())
		{
			if (((*col).second)->m_GameObject == obj)
			{
				continue;
			}
			else
			{
				ComponentEngine::Rigidbody* _rig = (*col).second->m_GameObject->GetComponent<ComponentEngine::Rigidbody>();
				if (_rig)
				{
					_hitInfo_dynamic.col = (*col).second;
					_hitInfo_dynamic.distance = buf.touches[i].distance;
					_hitInfo_dynamic.point = direction * buf.touches[i].distance;
					_hitInfo_dynamic.triangleIndex = buf.touches[i].faceIndex;
					_hitInfo_dynamic.rigidbody = ((*col).second)->m_GameObject->GetComponent<ComponentEngine::Rigidbody>();
					_hitInfo_dynamic.transform = ((*col).second)->m_GameObject->GetComponent<ComponentEngine::Transform>();
					break;
				}
				else
				{
					_hitInfo_static.col = (*col).second;
					_hitInfo_static.distance = buf.touches[i].distance;
					_hitInfo_static.point = direction * buf.touches[i].distance;
					_hitInfo_static.triangleIndex = buf.touches[i].faceIndex;
					_hitInfo_static.rigidbody = ((*col).second)->m_GameObject->GetComponent<ComponentEngine::Rigidbody>();
					_hitInfo_static.transform = ((*col).second)->m_GameObject->GetComponent<ComponentEngine::Transform>();
				}
			}
		}
	}

	/// 두 종류가 모두 부딪혔을 때 두 개를 비교한다.
	if (_hitInfo_static.col && _hitInfo_dynamic.col)
	{
		///(origin - static 1st pos).distance() 와 (origin - dynamic 1st pos).distance() 비교해서 작은 것 리턴
		float _staticDistance = Vector3::Distance(origin, _hitInfo_static.transform->GetPosition());
		float _dynamicDistance = Vector3::Distance(origin, _hitInfo_dynamic.transform->GetPosition());

		// static 이 더 가까울 때
		if (_staticDistance <= _dynamicDistance)
		{
			hitInfo = _hitInfo_static;
		}
		else
		{
			hitInfo = _hitInfo_dynamic;
		}
		return true;
	}
	else
	{
		// 하나만 부딪혔다 or 둘 다 부딪히지 않음
		if (_hitInfo_static.col)
		{
			hitInfo = _hitInfo_static;
			return true;
		}
		if (_hitInfo_dynamic.col)
		{
			hitInfo = _hitInfo_dynamic;
			return true;
		}
		return false;
	}
	int a = 0;
}

/// 부딪힌 오브젝트 중 자기 자신에 달라붙어있는 오브젝트를 제외하고 처음 부딪힌 오브젝트 정보를 리턴 + 충돌체크하지 않을 오브젝트 설정
bool PhysXEngine::PhysX::Raycast(Vector3 origin, Vector3 direction, float distance, ComponentEngine::RaycastHit& hitInfo, bool isDynamicCheck, ComponentEngine::GameObject* obj, std::vector<std::string> ignoreList)
{
	PxVec3 _origin(origin.x, origin.y, origin.z);
	PxVec3 _direction(direction.x, direction.y, direction.z);
	PxReal _distance = distance;

	// isDynamicCheck = true : 리지드바디 붙어있는 콜라이더도 충돌 체크
	const PxQueryFlags qf(isDynamicCheck ? PxQueryFlags(PxQueryFlag::eSTATIC | PxQueryFlag::eDYNAMIC | PxQueryFlag::ePREFILTER) : PxQueryFlags(PxQueryFlag::eSTATIC));
	const PxQueryFilterData filterData(PxFilterData(), qf);

	const PxU32 bufferSize = 32;				// [in] size of 'hitBuffer'
	PxRaycastHit hitBuffer[bufferSize];			// [out] User provided buffer for results
	PxRaycastBuffer buf(hitBuffer, bufferSize); // [out] Blocking and touching hits stored here

	// origin , dir , distance , RaycastCallback , HitFlags , QueryFilterData , QueryFilterCallback
	m_Scene->raycast(_origin, _direction, _distance, buf, PxHitFlags(PxHitFlag::ePOSITION | PxHitFlag::eNORMAL | PxHitFlag::eUV/*0*/), filterData);

	/// TODO : 코드 정리
	/// static ray 닿는 순서대로 -> dynamic ray 닿는 순서대로.
	/// 자기 자신에 달려있는 오브젝트를 제외하고 static 1st, dynamic 1st 를 가져온다.
	/// 이후 (origin - static 1st pos).distance() 와 (origin - dynamic 1st pos).distance() 비교해서 작은 것 리턴

	ComponentEngine::RaycastHit _hitInfo_static;
	ComponentEngine::RaycastHit _hitInfo_dynamic;

	for (PxU32 i = 0; i < buf.nbTouches; i++)
	{
		auto col = m_Collisions.find(buf.touches[i].shape);

		if (col != m_Collisions.end())
		{
			if (((*col).second)->m_GameObject == obj)
			{
				continue;
			}
			else
			{
				bool _isIgnore = false;
				for (unsigned int i = 0; i < ignoreList.size(); i++)
				{
					if (((*col).second)->m_GameObject->GetLayerName() == ignoreList[i])
					{
						_isIgnore = true;
						break;
					}
				}

				if (_isIgnore)
				{
					continue;
				}

				ComponentEngine::Rigidbody* _rig = (*col).second->m_GameObject->GetComponent<ComponentEngine::Rigidbody>();
				if (_rig)
				{
					_hitInfo_dynamic.col = (*col).second;
					_hitInfo_dynamic.distance = buf.touches[i].distance;
					_hitInfo_dynamic.point = direction * buf.touches[i].distance;
					_hitInfo_dynamic.triangleIndex = buf.touches[i].faceIndex;
					_hitInfo_dynamic.rigidbody = ((*col).second)->m_GameObject->GetComponent<ComponentEngine::Rigidbody>();
					_hitInfo_dynamic.transform = ((*col).second)->m_GameObject->GetComponent<ComponentEngine::Transform>();
					break;
				}
				else
				{
					_hitInfo_static.col = (*col).second;
					_hitInfo_static.distance = buf.touches[i].distance;
					_hitInfo_static.point = direction * buf.touches[i].distance;
					_hitInfo_static.triangleIndex = buf.touches[i].faceIndex;
					_hitInfo_static.rigidbody = ((*col).second)->m_GameObject->GetComponent<ComponentEngine::Rigidbody>();
					_hitInfo_static.transform = ((*col).second)->m_GameObject->GetComponent<ComponentEngine::Transform>();
				}
			}
		}
	}

	/// 두 종류가 모두 부딪혔을 때 두 개를 비교한다.
	if (_hitInfo_static.col && _hitInfo_dynamic.col)
	{
		///(origin - static 1st pos).distance() 와 (origin - dynamic 1st pos).distance() 비교해서 작은 것 리턴
		float _staticDistance = Vector3::Distance(origin, _hitInfo_static.transform->GetPosition());
		float _dynamicDistance = Vector3::Distance(origin, _hitInfo_dynamic.transform->GetPosition());

		// static 이 더 가까울 때
		if (_staticDistance <= _dynamicDistance)
		{
			hitInfo = _hitInfo_static;
		}
		else
		{
			hitInfo = _hitInfo_dynamic;
		}
		return true;
	}
	else
	{
		// 하나만 부딪혔다 or 둘 다 부딪히지 않음
		if (_hitInfo_static.col)
		{
			hitInfo = _hitInfo_static;
			return true;
		}
		if (_hitInfo_dynamic.col)
		{
			hitInfo = _hitInfo_dynamic;
			return true;
		}
		return false;
	}
}

/// Ray에 부딪힌 모든 오브젝트를 vector 리스트로 리턴한다. (활성화되어있는 오브젝트만)
bool PhysXEngine::PhysX::RaycastAll(Vector3 origin, Vector3 direction, float distance, std::vector<ComponentEngine::RaycastHit>& hitInfoList, bool isDynamicCheck)
{
	PxVec3 _origin(origin.x, origin.y, origin.z);
	PxVec3 _direction(direction.x, direction.y, direction.z);
	PxReal _distance = distance;

	/// isDynamicCheck = true : 리지드바디 붙어있는 콜라이더도 충돌 체크
	const PxQueryFlags qf(isDynamicCheck ? PxQueryFlags(PxQueryFlag::eSTATIC | PxQueryFlag::eDYNAMIC | PxQueryFlag::ePREFILTER) : PxQueryFlags(PxQueryFlag::eSTATIC));
	const PxQueryFilterData filterData(PxFilterData(), qf);

	const PxU32 bufferSize = 32;				// [in] size of 'hitBuffer'
	PxRaycastHit hitBuffer[bufferSize];			// [out] User provided buffer for results
	PxRaycastBuffer buf(hitBuffer, bufferSize); // [out] Blocking and touching hits stored here

	/// origin , dir , distance , RaycastCallback , HitFlags , QueryFilterData , QueryFilterCallback
	m_Scene->raycast(_origin, _direction, _distance, buf, PxHitFlags(PxHitFlag::ePOSITION | PxHitFlag::eNORMAL | PxHitFlag::eUV/*0*/), filterData);

	ComponentEngine::Collider* _collider = nullptr;

	/// 부딪힌 개수만큼 돈다.
	for (PxU32 i = 0; i < buf.nbTouches; i++)
	{
		ComponentEngine::RaycastHit _hitInfo;

		auto col = m_Collisions.find(buf.touches[i].shape);

		if (col != m_Collisions.end())
		{
			_collider = (*col).second;

			if (_collider)
			{
				/// 콜라이더나 게임오브젝트가 비활성화 상태인 경우는 리스트에 넣지 않는다.
				if (_collider->m_GameObject->isActive() == false || _collider->isActive() == false)
				{
					continue;
				}
			}

			/// 부딪힌 콜라이더 정보 입력
			_hitInfo.col = (*col).second;
			_hitInfo.distance = buf.touches[i].distance;
			_hitInfo.point = direction * buf.touches[i].distance;
			//_hitInfo.point.x = buf.touches[i].position.x;	
			//_hitInfo.point.y = buf.touches[i].position.y;	
			//_hitInfo.point.z = buf.touches[i].position.z;
			_hitInfo.triangleIndex = buf.touches[i].faceIndex;
			_hitInfo.rigidbody = ((*col).second)->m_GameObject->GetComponent<ComponentEngine::Rigidbody>();
			_hitInfo.transform = ((*col).second)->m_GameObject->GetComponent<ComponentEngine::Transform>();
			hitInfoList.push_back(_hitInfo);
		}
	}

	// 리스트에 하나라도 들어가 있으면 raycast는 true
	if (hitInfoList.size() > 0)
	{
		return true;
	}
	// 아니면 false
	return false;
}

bool PhysXEngine::PhysX::Raycast_server(DirectX::SimpleMath::Vector3 origin, DirectX::SimpleMath::Vector3 direction, float distance, ComponentEngine::RaycastHit& hitInfo, bool isDynamicCheck)
{
	PxVec3 _origin;
	_origin.x = origin.x;	_origin.y = origin.y;	_origin.z = origin.z;

	PxVec3 _direction;
	_direction.x = direction.x;	_direction.y = direction.y;	_direction.z = direction.z;

	PxReal _distance = distance;

	// isDynamicCheck = true : 리지드바디 붙어있는 콜라이더도 충돌 체크
	const PxQueryFlags qf(isDynamicCheck ? PxQueryFlags(PxQueryFlag::eSTATIC | PxQueryFlag::eDYNAMIC | PxQueryFlag::ePREFILTER) : PxQueryFlags(PxQueryFlag::eSTATIC));
	const PxQueryFilterData filterData(PxFilterData(), qf);

	RayCastCallback CB;

	PxRaycastBuffer buf;
	// origin , dir , distance , RaycastCallback , HitFlags , QueryFilterData , QueryFilterCallback
	m_SceneList[m_NowSceneNum]->raycast(_origin, _direction, _distance, buf, PxHitFlags(0), filterData);

	auto col = m_Collisions.find(buf.block.shape);

	ComponentEngine::Collider* collider = nullptr;

	if (col != m_Collisions.end())
	{
		collider = (*col).second;

		// 충돌 정보
		hitInfo.col = collider;
		/// 부딪힌 위치를 제대로 가져오지 못함.
		///hitInfo.point.x = buf.block.position.x;	hitInfo.point.y = buf.block.position.y;	hitInfo.point.z = buf.block.position.z;
		///hitInfo.normal.x = buf.block.normal.x;	hitInfo.normal.y = buf.block.normal.y;	hitInfo.normal.z = buf.block.normal.z;
		///hitInfo.barycentricCoordinate = buf.block.
		hitInfo.distance = buf.block.distance;

		hitInfo.point = direction * buf.block.distance;

		hitInfo.triangleIndex = buf.block.faceIndex;
		hitInfo.rigidbody = collider->m_GameObject->GetComponent<ComponentEngine::Rigidbody>();
		hitInfo.transform = collider->m_GameObject->GetComponent<ComponentEngine::Transform>();
	}

	if (collider)
	{
		if (collider->m_GameObject->isActive() == false || collider->isActive() == false)
		{
			return false;
		}

	}
	else
	{
		return false;
	}

	return buf.hasBlock;
}

bool PhysXEngine::PhysX::Raycast_server(Vector3 origin, Vector3 direction, float distance, ComponentEngine::RaycastHit& hitInfo, bool isDynamicCheck, ComponentEngine::GameObject* obj, std::vector<std::string> ignoreList)
{
	PxVec3 _origin(origin.x, origin.y, origin.z);
	PxVec3 _direction(direction.x, direction.y, direction.z);
	PxReal _distance = distance;

	// isDynamicCheck = true : 리지드바디 붙어있는 콜라이더도 충돌 체크
	const PxQueryFlags qf(isDynamicCheck ? PxQueryFlags(PxQueryFlag::eSTATIC | PxQueryFlag::eDYNAMIC | PxQueryFlag::ePREFILTER) : PxQueryFlags(PxQueryFlag::eSTATIC));
	const PxQueryFilterData filterData(PxFilterData(), qf);

	const PxU32 bufferSize = 32;				// [in] size of 'hitBuffer'
	PxRaycastHit hitBuffer[bufferSize];			// [out] User provided buffer for results
	PxRaycastBuffer buf(hitBuffer, bufferSize); // [out] Blocking and touching hits stored here

	// origin , dir , distance , RaycastCallback , HitFlags , QueryFilterData , QueryFilterCallback
	m_SceneList[m_NowSceneNum]->raycast(_origin, _direction, _distance, buf, PxHitFlags(PxHitFlag::ePOSITION | PxHitFlag::eNORMAL | PxHitFlag::eUV/*0*/), filterData);

	/// TODO : 코드 정리
	/// static ray 닿는 순서대로 -> dynamic ray 닿는 순서대로.
	/// 자기 자신에 달려있는 오브젝트를 제외하고 static 1st, dynamic 1st 를 가져온다.
	/// 이후 (origin - static 1st pos).distance() 와 (origin - dynamic 1st pos).distance() 비교해서 작은 것 리턴

	ComponentEngine::RaycastHit _hitInfo_static;
	ComponentEngine::RaycastHit _hitInfo_dynamic;

	for (PxU32 i = 0; i < buf.nbTouches; i++)
	{
		auto col = m_Collisions.find(buf.touches[i].shape);

		if (col != m_Collisions.end())
		{
			if (((*col).second)->m_GameObject == obj)
			{
				continue;
			}
			else
			{
				bool _isIgnore = false;
				for (unsigned int i = 0; i < ignoreList.size(); i++)
				{
					if (((*col).second)->m_GameObject->GetLayerName() == ignoreList[i])
					{
						_isIgnore = true;
						break;
					}
				}

				if (_isIgnore)
				{
					continue;
				}

				ComponentEngine::Rigidbody* _rig = (*col).second->m_GameObject->GetComponent<ComponentEngine::Rigidbody>();
				if (_rig)
				{
					_hitInfo_dynamic.col = (*col).second;
					_hitInfo_dynamic.distance = buf.touches[i].distance;
					_hitInfo_dynamic.point = direction * buf.touches[i].distance;
					_hitInfo_dynamic.triangleIndex = buf.touches[i].faceIndex;
					_hitInfo_dynamic.rigidbody = ((*col).second)->m_GameObject->GetComponent<ComponentEngine::Rigidbody>();
					_hitInfo_dynamic.transform = ((*col).second)->m_GameObject->GetComponent<ComponentEngine::Transform>();
					break;
				}
				else
				{
					_hitInfo_static.col = (*col).second;
					_hitInfo_static.distance = buf.touches[i].distance;
					_hitInfo_static.point = direction * buf.touches[i].distance;
					_hitInfo_static.triangleIndex = buf.touches[i].faceIndex;
					_hitInfo_static.rigidbody = ((*col).second)->m_GameObject->GetComponent<ComponentEngine::Rigidbody>();
					_hitInfo_static.transform = ((*col).second)->m_GameObject->GetComponent<ComponentEngine::Transform>();
				}
			}
		}
	}

	/// 두 종류가 모두 부딪혔을 때 두 개를 비교한다.
	if (_hitInfo_static.col && _hitInfo_dynamic.col)
	{
		///(origin - static 1st pos).distance() 와 (origin - dynamic 1st pos).distance() 비교해서 작은 것 리턴
		float _staticDistance = Vector3::Distance(origin, _hitInfo_static.transform->GetPosition());
		float _dynamicDistance = Vector3::Distance(origin, _hitInfo_dynamic.transform->GetPosition());

		// static 이 더 가까울 때
		if (_staticDistance <= _dynamicDistance)
		{
			hitInfo = _hitInfo_static;
		}
		else
		{
			hitInfo = _hitInfo_dynamic;
		}
		return true;
	}
	else
	{
		// 하나만 부딪혔다 or 둘 다 부딪히지 않음
		if (_hitInfo_static.col)
		{
			hitInfo = _hitInfo_static;
			return true;
		}
		if (_hitInfo_dynamic.col)
		{
			hitInfo = _hitInfo_dynamic;
			return true;
		}
		return false;
	}
}

bool PhysXEngine::PhysX::Raycast_server(Vector3 origin, Vector3 direction, float distance, ComponentEngine::RaycastHit& hitInfo, bool isDynamicCheck, ComponentEngine::GameObject* obj, std::string ignoreList)
{
	PxVec3 _origin(origin.x, origin.y, origin.z);
	PxVec3 _direction(direction.x, direction.y, direction.z);
	PxReal _distance = distance;

	// isDynamicCheck = true : 리지드바디 붙어있는 콜라이더도 충돌 체크
	const PxQueryFlags qf(isDynamicCheck ? PxQueryFlags(PxQueryFlag::eSTATIC | PxQueryFlag::eDYNAMIC | PxQueryFlag::ePREFILTER) : PxQueryFlags(PxQueryFlag::eSTATIC));
	const PxQueryFilterData filterData(PxFilterData(), qf);

	const PxU32 bufferSize = 32;				// [in] size of 'hitBuffer'
	PxRaycastHit hitBuffer[bufferSize];			// [out] User provided buffer for results
	PxRaycastBuffer buf(hitBuffer, bufferSize); // [out] Blocking and touching hits stored here

	// origin , dir , distance , RaycastCallback , HitFlags , QueryFilterData , QueryFilterCallback
	m_SceneList[m_NowSceneNum]->raycast(_origin, _direction, _distance, buf, PxHitFlags(PxHitFlag::ePOSITION | PxHitFlag::eNORMAL | PxHitFlag::eUV/*0*/), filterData);

	/// TODO : 코드 정리
	/// static ray 닿는 순서대로 -> dynamic ray 닿는 순서대로.
	/// 자기 자신에 달려있는 오브젝트를 제외하고 static 1st, dynamic 1st 를 가져온다.
	/// 이후 (origin - static 1st pos).distance() 와 (origin - dynamic 1st pos).distance() 비교해서 작은 것 리턴

	ComponentEngine::RaycastHit _hitInfo_static;
	ComponentEngine::RaycastHit _hitInfo_dynamic;

	for (PxU32 i = 0; i < buf.nbTouches; i++)
	{
		auto col = m_Collisions.find(buf.touches[i].shape);

		if (col != m_Collisions.end())
		{
			if (((*col).second)->m_GameObject == obj)
			{
				continue;
			}
			else
			{
				if (((*col).second)->m_GameObject->GetLayerName() == ignoreList)
				{
					continue;
				}

				ComponentEngine::Rigidbody* _rig = (*col).second->m_GameObject->GetComponent<ComponentEngine::Rigidbody>();
				if (_rig)
				{
					_hitInfo_dynamic.col = (*col).second;
					_hitInfo_dynamic.distance = buf.touches[i].distance;
					_hitInfo_dynamic.point = direction * buf.touches[i].distance;
					_hitInfo_dynamic.triangleIndex = buf.touches[i].faceIndex;
					_hitInfo_dynamic.rigidbody = ((*col).second)->m_GameObject->GetComponent<ComponentEngine::Rigidbody>();
					_hitInfo_dynamic.transform = ((*col).second)->m_GameObject->GetComponent<ComponentEngine::Transform>();
					break;
				}
				else
				{
					_hitInfo_static.col = (*col).second;
					_hitInfo_static.distance = buf.touches[i].distance;
					_hitInfo_static.point = direction * buf.touches[i].distance;
					_hitInfo_static.triangleIndex = buf.touches[i].faceIndex;
					_hitInfo_static.rigidbody = ((*col).second)->m_GameObject->GetComponent<ComponentEngine::Rigidbody>();
					_hitInfo_static.transform = ((*col).second)->m_GameObject->GetComponent<ComponentEngine::Transform>();
				}
			}
		}
	}

	/// 두 종류가 모두 부딪혔을 때 두 개를 비교한다.
	if (_hitInfo_static.col && _hitInfo_dynamic.col)
	{
		///(origin - static 1st pos).distance() 와 (origin - dynamic 1st pos).distance() 비교해서 작은 것 리턴
		float _staticDistance = Vector3::Distance(origin, _hitInfo_static.transform->GetPosition());
		float _dynamicDistance = Vector3::Distance(origin, _hitInfo_dynamic.transform->GetPosition());

		// static 이 더 가까울 때
		if (_staticDistance <= _dynamicDistance)
		{
			hitInfo = _hitInfo_static;
		}
		else
		{
			hitInfo = _hitInfo_dynamic;
		}
		return true;
	}
	else
	{
		// 하나만 부딪혔다 or 둘 다 부딪히지 않음
		if (_hitInfo_static.col)
		{
			hitInfo = _hitInfo_static;
			return true;
		}
		if (_hitInfo_dynamic.col)
		{
			hitInfo = _hitInfo_dynamic;
			return true;
		}
		return false;
	}
}

PhysXEngine::CollisionCallback::CollisionCallback()
	: m_PhysicsEngine(PhysX::Ins())
{

}

PhysXEngine::CollisionCallback::~CollisionCallback()
{

}

void PhysXEngine::CollisionCallback::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
{
	for (physx::PxU32 i = 0; i < nbPairs; i++)
	{
		// 충돌 감지했지만 해당 오브젝트가 삭제되었을 경우
		if (pairs[i].flags & (physx::PxContactPairFlag::eREMOVED_SHAPE_0 | physx::PxContactPairFlag::eREMOVED_SHAPE_1))
		{
			continue;
		}

		auto iter1 = m_PhysicsEngine->m_Collisions.find(pairs[i].shapes[0]);
		auto iter2 = m_PhysicsEngine->m_Collisions.find(pairs[i].shapes[1]);
		if (iter1 != m_PhysicsEngine->m_Collisions.end() && iter2 != m_PhysicsEngine->m_Collisions.end())
		{
			ComponentEngine::Collider* col1 = (*iter1).second;
			ComponentEngine::Collider* col2 = (*iter2).second;
			if (pairs[i].events == physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
			{
				if (col1 && col2)
				{
					if (col1->isActive())
					{
						col1->m_GameObject->OnCollisionEnter(col2);
					}
					if (col2->isActive())
					{
						col2->m_GameObject->OnCollisionEnter(col1);
					}
				}
			}
			else if (pairs[i].events == physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
			{
				if (col1 && col2)
				{
					if (col1->isActive())
					{
						col1->m_GameObject->OnCollisionExit(col2);
					}
					if (col2->isActive())
					{
						col2->m_GameObject->OnCollisionExit(col1);
					}
				}
			}
			else if (pairs[i].events == physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
			{
				if (col1 && col2)
				{
					if (col1->isActive())
					{
						col1->m_GameObject->OnCollisionStay(col2);
					}
					if (col2->isActive())
					{
						col2->m_GameObject->OnCollisionStay(col1);
					}
				}
			}
		}
	}
}

void PhysXEngine::CollisionCallback::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
{
	for (physx::PxU32 i = 0; i < count; i++)
	{
		// 충돌 감지했지만 해당 오브젝트가 삭제되었을 경우
		if (pairs[i].flags & (physx::PxTriggerPairFlag::eREMOVED_SHAPE_OTHER | physx::PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER))
		{
			continue;
		}

		auto iter1 = m_PhysicsEngine->m_Collisions.find(pairs[i].triggerShape);
		auto iter2 = m_PhysicsEngine->m_Collisions.find(pairs[i].otherShape);
		if (iter1 != m_PhysicsEngine->m_Collisions.end() && iter2 != m_PhysicsEngine->m_Collisions.end())
		{
			ComponentEngine::Collider* col1 = (*iter1).second;
			ComponentEngine::Collider* col2 = (*iter2).second;
			if (pairs[i].status == physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
			{
				if (col1 && col2)
				{
					col1->m_GameObject->SetTriggerFlag(PhysX::ePairFlag::Enter);
					col2->m_GameObject->SetTriggerFlag(PhysX::ePairFlag::Enter);
					col1->m_GameObject->SetMyTriggerCollider(col1);
					col2->m_GameObject->SetMyTriggerCollider(col2);
					col1->m_GameObject->SetTargetTriggerCollider(col2);
					col2->m_GameObject->SetTargetTriggerCollider(col1);
					if (col1->isActive() && col1->IsTrigger())
					{
						col1->m_GameObject->OnTriggerEnter(col2);
					}
					if (col2->isActive() && col2->IsTrigger())
					{
						col2->m_GameObject->OnTriggerEnter(col1);
					}
					col1->m_GameObject->SetTriggerFlag(PhysX::ePairFlag::Stay);
					col2->m_GameObject->SetTriggerFlag(PhysX::ePairFlag::Stay);
				}
			}
			else if (pairs[i].status == physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
			{
				if (col1 && col2)
				{
					col1->m_GameObject->SetPrevTriggerFlag(PhysX::ePairFlag::Stay);
					col2->m_GameObject->SetPrevTriggerFlag(PhysX::ePairFlag::Stay);
					//if (col1->isActive() && col1->IsTrigger())
					if (col1->IsTrigger())
					{
						col1->m_GameObject->OnTriggerExit(col2);
					}
					//if (col2->isActive() && col2->IsTrigger())
					if (col2->IsTrigger())
					{
						col2->m_GameObject->OnTriggerExit(col1);
					}
					col1->m_GameObject->SetTriggerFlag(PhysX::ePairFlag::Exit);
					col2->m_GameObject->SetTriggerFlag(PhysX::ePairFlag::Exit);
 					//col1->m_GameObject->SetMyTriggerCollider(nullptr);
 					//col2->m_GameObject->SetMyTriggerCollider(nullptr);
 					//col1->m_GameObject->SetTargetTriggerCollider(nullptr);
 					//col2->m_GameObject->SetTargetTriggerCollider(nullptr);
				}
			}
		}
	}
}

PhysXEngine::RayCastCallback::RayCastCallback()
	: PxHitCallback(nullptr, 1), m_PhysicsEngine(PhysX::Ins())
{

}

PhysXEngine::RayCastCallback::~RayCastCallback()
{

}

physx::PxAgain PhysXEngine::RayCastCallback::processTouches(const physx::PxRaycastHit* buffer, physx::PxU32 nbHits)
{
	auto iter1 = m_PhysicsEngine->m_Collisions.find(buffer->shape);

	return true;
}

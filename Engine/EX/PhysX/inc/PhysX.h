#pragma once
#include <vector>
#include <map>
#include <string>
#include <SimpleMath.h>
#include "PhysXDefine.h"
#include "PxPhysicsAPI.h"
#include "RaycastHit.h"

#define PX_RELEASE(x)	if(x)	{ x->release(); x = NULL;	}
#define MAX_NUM_ACTOR_SHAPES 128

namespace ComponentEngine
{
	class Collider;
	class Physics;
}

namespace PhysXEngine
{
	class CollisionCallback;

	class PhysX
	{

	public:
		enum class ePairFlag
		{
			None,
			Stay,
			Enter = physx::PxPairFlag::eNOTIFY_TOUCH_FOUND,
			Exit = physx::PxPairFlag::eNOTIFY_TOUCH_LOST,
		};

	private:
		physx::PxDefaultAllocator		m_Allocator;
		physx::PxDefaultErrorCallback	m_ErrorCallback;
		physx::PxFoundation* m_Foundation;
		physx::PxPvd* m_PVD;
		physx::PxPhysics* m_Physics;
		physx::PxScene* m_Scene;

		std::vector<physx::PxScene*> m_SceneList;
		std::vector<physx::PxPhysics*> m_PhysicList;


		physx::PxDefaultCpuDispatcher* m_Dispatcher;

		// Plane 때문에 가지고 있는 Default Material
		physx::PxMaterial* m_Material;

		int m_NowSceneNum;



	public:
		std::map<physx::PxShape*, ComponentEngine::Collider*> m_Collisions;
		CollisionCallback* m_Callback;
		std::vector<CollisionCallback*> m_CallbackList;

#pragma region SINGLETON
	private:
		static PhysX* m_pInstance;

	public:
		static PhysX* Ins()
		{
			if (m_pInstance == nullptr)
			{
				m_pInstance = new PhysX();
			}
			return m_pInstance;
		}
#pragma endregion

	private:
		PhysX();
		~PhysX();

	public:
		void Init();
		void InitPhys();
		void AddPhysicScene();
		void Init_NonPVD();
		void Update();
		void Update(int index);
		void CleanUp();
		void Finalize();

		void SetNowScene(int value) { m_NowSceneNum = value; }
		int GetNowScene() { return m_NowSceneNum; }
	public:
		void AddPhysXActor(physx::PxActor& actor);
		void AddPhysXActor(physx::PxActor& actor, int index);

	public:
		physx::PxShape* CreatePxShape(physx::PxGeometry geometry, physx::PxMaterial* material, bool isExclusive);
		physx::PxRigidActor* CreateRigidStatic(physx::PxTransform transform);
		physx::PxRigidDynamic* CreateRigidDynamic(physx::PxTransform transform);

	public:
		void AddCollision(ComponentEngine::Collider* collision);
		void RemoveCollision(ComponentEngine::Collider* collision);

		bool Raycast(DirectX::SimpleMath::Vector3 origin, DirectX::SimpleMath::Vector3 direction, float distance, bool isDynamicCheck);
		bool Raycast(DirectX::SimpleMath::Vector3 origin, DirectX::SimpleMath::Vector3 direction, float distance, ComponentEngine::RaycastHit& hitInfo, bool isDynamicCheck);
		bool Raycast(DirectX::SimpleMath::Vector3 origin, DirectX::SimpleMath::Vector3 direction, float distance, ComponentEngine::RaycastHit& hitInfo, bool isDynamicCheck, ComponentEngine::GameObject* obj);
		bool Raycast(DirectX::SimpleMath::Vector3 origin, DirectX::SimpleMath::Vector3 direction, float distance, ComponentEngine::RaycastHit& hitInfo, bool isDynamicCheck, ComponentEngine::GameObject* obj, std::vector<std::string> ignoreList);

		bool RaycastAll(DirectX::SimpleMath::Vector3 origin, DirectX::SimpleMath::Vector3 direction, float distance, std::vector<ComponentEngine::RaycastHit>& hitInfoList, bool isDynamicCheck);
		bool Raycast_server(DirectX::SimpleMath::Vector3 origin, DirectX::SimpleMath::Vector3 direction, float distance, ComponentEngine::RaycastHit& hitInfo, bool isDynamicCheck);
		bool Raycast_server(DirectX::SimpleMath::Vector3 origin, DirectX::SimpleMath::Vector3 direction, float distance, ComponentEngine::RaycastHit& hitInfo, bool isDynamicCheck, ComponentEngine::GameObject* obj, std::vector<std::string> ignoreList);
		bool Raycast_server(DirectX::SimpleMath::Vector3 origin, DirectX::SimpleMath::Vector3 direction, float distance, ComponentEngine::RaycastHit& hitInfo, bool isDynamicCheck, ComponentEngine::GameObject* obj, std::string ignoreList);

	public:
		physx::PxTransform MakePxTransform(DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Quaternion rot);
		physx::PxTransform MakePxTransformPos(DirectX::SimpleMath::Vector3 pos);
		physx::PxTransform MakePxTransformRot(DirectX::SimpleMath::Quaternion rot);

	public:
		physx::PxPhysics* GetPxPhysics() const { return m_Physics; }
		physx::PxPhysics* GetPxPhysics(int index) const { return m_PhysicList[index]; }
	};

	class CollisionCallback :public physx::PxSimulationEventCallback
	{
	private:
		PhysX* m_PhysicsEngine;

	public:
		CollisionCallback();
		virtual ~CollisionCallback();

		void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) {}
		void onWake(physx::PxActor** actors, physx::PxU32 count) {}
		void onSleep(physx::PxActor** actors, physx::PxU32 count) {}
		void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs);
		void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count);
		void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) {}
	};

	class RayCastCallback : public physx::PxRaycastCallback
	{
	private:
		PhysX* m_PhysicsEngine;

	public:
		RayCastCallback();
		virtual ~RayCastCallback();

		physx::PxAgain processTouches(const physx::PxRaycastHit* buffer, physx::PxU32 nbHits);
		void finalizeQuery() {}
	};
}
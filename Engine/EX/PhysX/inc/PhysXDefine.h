#pragma once

#ifdef _WIN64
#pragma comment(lib, "PhysXExtensions_static_64.lib")
#pragma comment(lib, "PhysX_64.lib")
#pragma comment(lib, "PhysXPvdSDK_static_64.lib")
#pragma comment(lib, "PhysXVehicle_static_64.lib")
#pragma comment(lib, "PhysXCharacterKinematic_static_64.lib")
#pragma comment(lib, "PhysXCooking_64.lib")
#pragma comment(lib, "PhysXCommon_64.lib")
#pragma comment(lib, "SnippetUtils_static_64.lib")
#pragma comment(lib, "SnippetRender_static_64.lib")
#pragma comment(lib, "PhysXFoundation_64.lib")
#else
#pragma comment(lib, "PhysXExtensions_static_32.lib")
#pragma comment(lib, "PhysX_32.lib")
#pragma comment(lib, "PhysXPvdSDK_static_32.lib")
#pragma comment(lib, "PhysXVehicle_static_32.lib")
#pragma comment(lib, "PhysXCharacterKinematic_static_32.lib")
#pragma comment(lib, "PhysXCooking_32.lib")
#pragma comment(lib, "PhysXCommon_32.lib")
#pragma comment(lib, "SnippetUtils_static_32.lib")
#pragma comment(lib, "SnippetRender_static_32.lib")
#pragma comment(lib, "PhysXFoundation_32.lib")
#endif
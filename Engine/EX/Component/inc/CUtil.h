#pragma once
#include <SimpleMath.h>

#pragma warning(disable:4996)

#define DegreeToRadian(degree) ((degree) * (3.141592654f / 180.0f))
#define RadianToDegree(radian) ((radian) * (180.0f / 3.141592654f))

inline static float NormalizeAngle(float angle)
{
	while (angle > 360)
	{
		angle -= 360;
	}
	while (angle < 0)
	{
		angle += 360;
	}
	return angle;
}

inline static DirectX::SimpleMath::Vector3 NormalizeAngles(DirectX::SimpleMath::Vector3 angles)
{
	angles.x = NormalizeAngle(angles.x);
	angles.y = NormalizeAngle(angles.y);
	angles.z = NormalizeAngle(angles.z);
	return angles;
}


inline static DirectX::SimpleMath::Vector3 QuaternionToEulerAngles(DirectX::SimpleMath::Quaternion q)
{
	/// Unity Forum Code - Timboc, May 5, 2020
	/// https://forum.unity.com/threads/is-there-a-conversion-method-from-quaternion-to-euler.624007/
	float sqw = q.w * q.w;
	float sqx = q.x * q.x;
	float sqy = q.y * q.y;
	float sqz = q.z * q.z;
	float unit = sqx + sqy + sqz + sqw; // if normalised is one, otherwise is correction factor
	float test = q.x * q.w - q.y * q.z;
	float _roll, _pitch, _yaw;

	if (test > 0.4995f * unit)
	{ // singularity at north pole
		_yaw = 2.f * atan2f(q.y, q.w);
		_pitch = 3.141592654f / 2.f;
		_roll = 0;
		return NormalizeAngles(DirectX::SimpleMath::Vector3(RadianToDegree(_pitch), RadianToDegree(_yaw), RadianToDegree(_roll)));
		//return Vector3(RadianToDegree(_pitch), RadianToDegree(_yaw), RadianToDegree(_roll));
	}
	if (test < -0.4995f * unit)
	{ // singularity at south pole
		_yaw = -2.f * atan2f(q.y, q.x);
		_pitch = -3.141592654f / 2.f;
		_roll = 0;
		return NormalizeAngles(DirectX::SimpleMath::Vector3(RadianToDegree(_pitch), RadianToDegree(_yaw), RadianToDegree(_roll)));
		//return Vector3(RadianToDegree(_pitch), RadianToDegree(_yaw), RadianToDegree(_roll));
	}

	DirectX::SimpleMath::Quaternion rot = DirectX::SimpleMath::Quaternion(q.w, q.z, q.x, q.y);
	_yaw = atan2f(2.f * rot.x * rot.w + 2.f * rot.y * rot.z, 1 - 2.f * (rot.z * rot.z + rot.w * rot.w));     // Yaw
	_pitch = asinf(2.f * (rot.x * rot.z - rot.w * rot.y));													 // Pitch
	_roll = atan2f(2.f * rot.x * rot.y + 2.f * rot.z * rot.w, 1 - 2.f * (rot.y * rot.y + rot.z * rot.z));    // Roll
	return NormalizeAngles(DirectX::SimpleMath::Vector3(RadianToDegree(_pitch), RadianToDegree(_yaw), RadianToDegree(_roll)));
	//return Vector3(RadianToDegree(_pitch), RadianToDegree(_yaw), RadianToDegree(_roll));
}

inline static DirectX::SimpleMath::Quaternion EulerAnglesToQuarternion(DirectX::SimpleMath::Vector3 eulerAngle)
{
	float cy = cosf(static_cast<float>(eulerAngle.z) * 0.5f);
	float sy = sinf(static_cast<float>(eulerAngle.z) * 0.5f);
	float cp = cosf(static_cast<float>(eulerAngle.y) * 0.5f);
	float sp = sinf(static_cast<float>(eulerAngle.y) * 0.5f);
	float cr = cosf(static_cast<float>(eulerAngle.x) * 0.5f);
	float sr = sinf(static_cast<float>(eulerAngle.x) * 0.5f);

	DirectX::SimpleMath::Quaternion q;
	q.w = cr * cp * cy + sr * sp * sy;
	q.x = sr * cp * cy - cr * sp * sy;
	q.y = cr * sp * cy + sr * cp * sy;
	q.z = cr * cp * sy - sr * sp * cy;

	return q;
}

inline static TCHAR* StringToTCHAR(std::string& s)
{
	size_t origsize = 0, convertedChars = 0; // 원래 문자열 길이, 변환된 문자열 길이	
	origsize = strlen(s.c_str()) + 1;  // 변환시킬 문자열의 길이를 구함
	wchar_t* t = new wchar_t[origsize];
	mbstowcs_s(&convertedChars, t, origsize, s.c_str(), _TRUNCATE);
	return (TCHAR*)t;
}

inline static std::string TCHARToString(const TCHAR* ptsz)
{
	int len = wcslen((wchar_t*)ptsz);
	char* psz = new char[2 * len + 1];
	wcstombs(psz, (wchar_t*)ptsz, 2 * len + 1);
	std::string s = psz;
	delete[] psz;
	return s;
}

template <typename T>
inline void SafeDelete(T& p)
{
	if (p)
	{
		delete p;
		p = nullptr;
	}
}

template <typename T>
inline void SafeRelease(T& p)
{
	if (p)
	{
		p->Release();
		p = nullptr;
	}
}
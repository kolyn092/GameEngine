// pch.h: �̸� �����ϵ� ��� �����Դϴ�.
// �Ʒ� ������ ������ �� ���� �����ϵǾ�����, ���� ���忡 ���� ���� ������ ����մϴ�.
// �ڵ� ������ �� ���� �ڵ� �˻� ����� �����Ͽ� IntelliSense ���ɿ��� ������ ��Ĩ�ϴ�.
// �׷��� ���⿡ ������ ������ ���� �� ������Ʈ�Ǵ� ��� ��� �ٽ� �����ϵ˴ϴ�.
// ���⿡ ���� ������Ʈ�� ������ �߰����� ������. �׷��� ������ ���ϵ˴ϴ�.

#ifndef PCH_H
#define PCH_H

//#ifdef UNICODE
//#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
//#else
//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
//#endif

//#define _SERVERON
#define _CLIENTON

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
#pragma warning(disable:4996)

#pragma comment(lib, "3DGraphicsEngine")
#pragma comment(lib, "vfw32.lib")
#pragma comment(lib, "PhysXLib")

// ���� ����
#include "DxDefine.h"
#include "EngineInterface.h"
#include "DrawText_Debug.h"

// ���⿡ �̸� �������Ϸ��� ��� �߰�
#include "framework.h"

// 
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <Vfw.h>
#include "locale.h"

#endif //PCH_H
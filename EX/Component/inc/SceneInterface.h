#pragma once

__interface SceneInterface
{

public:
	virtual void UpdateScene(int type, float dTime) abstract;
	virtual void DrawScene(int type) abstract;

};

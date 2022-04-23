  ////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"

#include "textureshaderclass.h"
#include "bitmapclass.h"
#include "textclass.h"

#include "particleshaderclass.h"
#include "particlesystemclass.h"

#include "skymapclass.h"

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 2000.0f;
const float SCREEN_NEAR = 0.1f;


////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();
	bool Frame(int, int, float, int);

	bool isCollide;
	float moveLeftRight;
	float moveBackForward;

	float camYaw = 0.0f;
	float camPitch = 0.0f;

	XMVECTOR camPosition;

	XMFLOAT3 camPos;

	float ghost_rotate;
	float cross_rotate;
	float cross_height;
	float crossH;
	bool equip;
	float crossX, crossZ;
	
	float tempX;
	float tempY;
	float tempZ;

	bool sound;
	bool sound2;
	float limitTime;

	void initData();
	bool sceneWchange;
	bool sceneLchange;

	float gone_ghost_height;
	float gone_ghost_H;

	float temp=0.0f;

private:
	bool Render(float);
	bool RenderWin();
	bool RenderLose();
	bool RenderTitle();

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	//ModelClass* m_Model;
	ModelClass* m_Ghost;
	ModelClass* m_Terr;
	ModelClass* m_SkyBox;
	ModelClass* m_grave;
	ModelClass* m_cross;
	ModelClass* m_house;

	int num_total;
	int num_ghost;
	int num_grave;
	int num_house;
	int poly_total;
	float grave_line;
	float grave_term;
	int ghostRand[4];

	int screenW;
	int screenH;

	float m_time;
	float frametime;

	bool caught[4];

	float timecalc[4];

	LightShaderClass* m_LightShader;
	LightClass* m_Light;
	XMFLOAT4* ambient;
	XMFLOAT4* diffuse;
	XMFLOAT4* specular;
	float specularP;

	TextureShaderClass* m_TextureShader;
	BitmapClass* m_Bitmap;

	TextClass* m_Text;
	TextClass* m_TextR;

	ParticleShaderClass* m_ParticleShader;
	ParticleSystemClass* m_ParticleSystem[2];

	SkymapClass* m_SkyMap;

	float record;
};

#endif
#ifndef _ION_SCENE_CSCENEMANAGER_H_INCLUDED_
#define _ION_SCENE_CSCENEMANAGER_H_INCLUDED_

#include <list>

#include "CSceneObject.h"
#include "CMeshSceneObject.h"
#include "CFrameBufferObject.h"
#include "CSceneEffectManager.h"
#include "CPerspectiveCameraSceneObject.h"
#include "CScene.h"

#include <SLine3.h>
#include <SColor.h>
#include <SPosition2.h>
#include <SUniform.h>
#include <SAttribute.h>

#include "CDefaultColorRenderPass.h"
#include "CMultiOutRenderPass.h"


class CSceneManager : public CScene
{

	//! Current scene used for rendering
	IScene * CurrentScene;

	//! Default frame buffer used for drawing, allows post-processing passes to access scene texture without implementing a custom pass
	CFrameBufferObject * SceneFrameBuffer;
	//! Default frame and depth textures from render using defualt frame buffer
	CTexture * SceneFrameTexture, * SceneDepthTexture;

	//! Simple shader used to draw a texture to the screen
	CShader * QuadCopy;

	CSceneEffectManager * EffectManager, * DeferredManager, * DefaultManager;

	//! Current render context window size
	SSize2 ScreenSize;

	//! Built in default color rendering pass
	smartPtr<CDefaultColorRenderPass> DefaultColorRenderPass;

public:

	CSceneManager(SPosition2 const & screenSize);
	void init(bool const EffectsManager = true, bool const FrameBuffer = true);

	void OnWindowResized(SPosition2 const & screenSize);

	void drawAll();
	void endDraw();

	CFrameBufferObject * getSceneFrameBuffer();
	CTexture * getSceneFrameTexture();
	CTexture * getSceneDepthTexture();
	smartPtr<CDefaultColorRenderPass> getDefaultColorRenderPass();

	CSceneEffectManager * getEffectManager();
	void setEffectManager(CSceneEffectManager * effectManager);

	SSize2 const & getScreenSize() const;	
	static GLuint const getQuadHandle();

};

#endif

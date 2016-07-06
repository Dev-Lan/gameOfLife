#pragma once
#include <G3D/G3DAll.h>
#include <iostream>
#include <math.h>
#include <sstream>

// #include <stddef.h>


#include "tinyfiledialogs.h"
#include "AttemptTexture.hpp"
#include "config.hpp"

class App : public GApp {
public:
	
	App(const GApp::Settings& settings = GApp::Settings());
	
	virtual void onInit() override;

	virtual bool onEvent(const GEvent& event) override;
	virtual void onUserInput(UserInput *ui) override;
	virtual void onSimulation(RealTime rdt, SimTime sdt, SimTime idt) override;

	virtual void onGraphics3D(RenderDevice* rd, Array<shared_ptr<Surface> >& surface3D) override;
	virtual void onGraphics2D(RenderDevice* rd, Array<Surface2D::Ref>& surface2D) override;

protected:

	// switch file to new input file
	void openNewInput();
	void initializeImage(const G3D::String& str);
	void setWindowCoords();
	void randomizeColorfulLife();
	void randomizeNoireLife();
	bool _playing;
	unsigned int _stepCount;
	bool _justDisplayTexture;
	bool _displayBitTexture;
	bool _edgeLoops;
	float _targetFPS;
	float _elapsedTime;
	bool _frameRateSatisfied;
	int _tailPower; // controls length of tail, varies between [0,7]
	bool _resetSeed;

	shared_ptr<Texture> _lifeGridTexture;
	shared_ptr<Texture> _lastInitializedTexture;

	/// for the graphics card
	shared_ptr< VertexBuffer > _vbuf;
	AttributeArray _gpuVerts;
	IndexStream _gpuIndices;
};

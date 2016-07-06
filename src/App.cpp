#include "App.hpp"

// using namespace std;

App::App(const GApp::Settings& settings) : GApp(settings) {
	renderDevice->setColorClearValue(Color3(0.8, 0.8, 0.8));
}


void App::onInit() {
	GApp::onInit();

	if (DEBUG_ENABLED) {
		std::cout << "debug defined" << std::endl;
		createDeveloperHUD();
		debugWindow->setVisible(false);
		developerWindow->setVisible(false);
		developerWindow->cameraControlWindow->setVisible(false);
		developerWindow->sceneEditorWindow->setVisible(false);
	}
	showRenderingStats = false;
	_playing = true;
	_stepCount = 2; // my hack to get past the first couple 'setup' frames
	_justDisplayTexture = true;
	_displayBitTexture = false;
	_edgeLoops = true;
	_targetFPS = 60.f;
	_elapsedTime = 0.f;
	_frameRateSatisfied = false;
	_tailPower = 2;
	_resetSeed = false;
	// init Texture geo.
	setWindowCoords();
	initializeImage(DEFAULT_SEED);
}

bool App::onEvent(const GEvent& event) {
	if (event.type == GEventType::VIDEO_RESIZE)  {
		setWindowCoords();
		// todo : is there a better solution? one that saves the last frame before resize?
		initializeImage(DEFAULT_SEED);
		_playing = true;
		_stepCount = 2;
		_justDisplayTexture = true;
		//
		return true;
	}	
	// default g3d on event function
	if (GApp::onEvent(event)) { return true; }
	return false;
}

void App::onUserInput(UserInput *ui) {
	GApp::onUserInput(ui); // needed for widgets to advance (camera manipulators, GUIs)

	// toggle animation
	if (ui->keyPressed(GKey(' '))) {
		_playing = !_playing;
	}

	// increase speed
	const float eps = 0.125f;
	if (ui->keyDown(GKey::UP)) {
		const float maxFPS = 90.f;
		_targetFPS = min(_targetFPS + eps, maxFPS);
	}

	// decrease speed
	if (ui->keyDown(GKey::DOWN)) {
		const float minFPS = 0.05f;
		_targetFPS = max(_targetFPS - eps, minFPS);
	}

	// increase tail length
	if (ui->keyPressed(GKey::RIGHT)) {
		_tailPower = min(_tailPower+1,10);
	}

	// decrease tail length
	if (ui->keyPressed(GKey::LEFT)) {
		_tailPower = max(_tailPower-1,0);
	}

	// step once
	if (ui->keyPressed(GKey::RETURN)) {
		_playing = true;
		_stepCount++;
	}

	// open new input
	if (ui->keyPressed(GKey('o'))) {
		openNewInput();
		_justDisplayTexture = true;
		_playing = true;
		_stepCount = 2;
	}

	// randomize current life
	if (ui->keyDown(GKey::LSHIFT)) {
		if (ui->keyPressed(GKey('s'))) {
			randomizeColorfulLife();
			_justDisplayTexture = true;
			_playing = true;
			_stepCount = 2;
		}
	 } else {
		if (ui->keyPressed(GKey('s'))) {
			randomizeNoireLife();
			_justDisplayTexture = true;
			_playing = true;
			_stepCount = 2;
		}
	}

	// reload main screen
	if (ui->keyDown(GKey('h'))) {
		initializeImage(DEFAULT_SEED);
		_playing = true;
		_stepCount = 2;
		_justDisplayTexture = true;
	}

	// reload current seed
	if (ui->keyPressed(GKey('r'))) {
		_resetSeed = true;
		_playing = true;
		_stepCount = 2;
		_justDisplayTexture = true;
	}

	// load more info
	if (ui->keyPressed(GKey('q'))) {
		initializeImage(MORE_INFO_SCREEN);
		_playing = true;
		_stepCount = 2;
		_justDisplayTexture = true;
	}

	// change boundary condition
	if (ui->keyPressed(GKey('e'))) {
		_edgeLoops = !_edgeLoops;
	}
}

void App::openNewInput() {
	char const* newFilename;
	char* fileFilters[2];
	fileFilters[0] = "*.png";
	fileFilters[1] = "*.jpg";
	newFilename = tinyfd_openFileDialog("Choose the seed of life", "~", 2, fileFilters, 0);
	if (newFilename) {
		initializeImage(G3D::String(newFilename));
	}
}

void App::setWindowCoords() {
	Array< Vector2 > cpuVerts;
	Array< int > cpuIndices;
/*

	1                     2
	+---------------------+
	|              .......|
	|       .......       |
	|.......              |
	+---------------------+
    3                     4
*/
    Vector2 p1, p2, p3, p4;
    p1 = Vector2(0,0);
    p2 = Vector2(window()->width(),0);
    p3 = Vector2(0,window()->height());
    p4 = Vector2(window()->width(),window()->height());

	cpuVerts.append(p1,p2,p3,p4);
	cpuIndices.append(0,1,2); // triangle ordering
	cpuIndices.append(2,1,3); // triangle ordering
	_vbuf = VertexBuffer::create(sizeof(Vector2) * cpuVerts.size() + sizeof(int) * cpuIndices.size(), VertexBuffer::WRITE_ONCE); // not quite once, but close enough
	_gpuVerts = AttributeArray(cpuVerts, _vbuf);
	_gpuIndices = IndexStream(cpuIndices, _vbuf);
}

void App::initializeImage(const G3D::String& str) {
	_lifeGridTexture = AttemptTexture::attemptTextureFromFile(str);
	shared_ptr< Image > tmpImage = _lifeGridTexture->toImage();
	tmpImage->flipVertical();
	_lifeGridTexture = Texture::fromImage("", tmpImage);
	_lastInitializedTexture = Texture::fromImage("", tmpImage);
}

void App::randomizeColorfulLife() {
	int numPixels = window()->width() * window()->height();
	float newTexture[ numPixels ];
	srand((unsigned)time(NULL));
	for (int i = 0; i < numPixels; i++) {
		double c = ((double) rand() / (RAND_MAX));
		newTexture[i] = (float)c;
	}
	_lifeGridTexture = Texture::fromMemory("", newTexture, ImageFormat::RGBA8(), window()->width(), window()->height(), 1);
	_lastInitializedTexture = Texture::fromMemory("", newTexture, ImageFormat::RGBA8(), window()->width(), window()->height(), 1);
}


void App::randomizeNoireLife() {
	int numPixels = window()->width() * window()->height();
	int newTexture[ numPixels ];
	srand((unsigned)time(NULL));
	for (int i = 0; i < numPixels; i++) {
		double c = 255 * ((double) rand() / (RAND_MAX));
		int cc = (int)c;
		cc = cc & 255;
		cc = cc | (cc << 8);
		cc = cc | (cc << 8);
		newTexture[i] = cc;
	}
	_lifeGridTexture = Texture::fromMemory("", newTexture, ImageFormat::RGBA8(), window()->width(), window()->height(), 1);
	_lastInitializedTexture = Texture::fromMemory("", newTexture, ImageFormat::RGBA8(), window()->width(), window()->height(), 1);
}


void App::onSimulation(RealTime rdt, SimTime sdt, SimTime idt) {
	GApp::onSimulation(rdt, sdt, idt); // need for widgets to advance (camera manipulators, GUIs)
	_elapsedTime += rdt;
	if (_elapsedTime > (1.f / _targetFPS )) {
		_frameRateSatisfied = true;
		_elapsedTime = 0;
	} else {
		_frameRateSatisfied = false;
	}
}


void App::onGraphics3D(RenderDevice* rd, Array<shared_ptr<Surface> >& surface3D) {

}


void App::onGraphics2D(RenderDevice* rd, Array<Surface2D::Ref>& posed2D) {
	if (_frameRateSatisfied) {
		if (_playing) {
			if (_resetSeed) {
				Texture::copy(_lastInitializedTexture, _lifeGridTexture);
				_resetSeed = false;
			}
			swapBuffers();
			rd->clear();
			rd->pushState();
			{
				Args args;
				args.setUniform("screenSize", rd->viewport().extent());
				args.setUniform("tex", _lifeGridTexture, Sampler::video());
				args.setUniform("displayTexture", _justDisplayTexture);
				args.setUniform("displayBitTexture", _displayBitTexture);
				args.setUniform("edgeLoops", _edgeLoops);
				args.setUniform("tailPower", _tailPower);
				args.setAttributeArray("vert", _gpuVerts);
				args.setIndexArray(_gpuIndices);
				args.setPrimitiveType(PrimitiveType::TRIANGLES);
				LAUNCH_SHADER(GOL_SHADER, args);
			}
			rd->popState();
			rd->copyTextureFromScreen(_lifeGridTexture, rd->viewport().extent());
			_displayBitTexture = false;
			if (_justDisplayTexture) {
				_justDisplayTexture = false;
				_displayBitTexture = true;
			}
		}
		if (_stepCount > 0) {
			_stepCount--;
			if (_stepCount == 0) {
				_playing = false;
			}
		}
	}
}
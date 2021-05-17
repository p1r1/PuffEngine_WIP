#include "EditorScreen.h"

#include <PuffEngine\ResourceManager.h>

EditorScreen::EditorScreen(PuffEngine::Window* window) :
	window_(window),
	spriteFont_("Fonts/chintzy.ttf", 32){
	screenIndex_ = SCREEN_INDEX_EDITOR;
}

EditorScreen::~EditorScreen() {
}

int EditorScreen::getNextScreenIndex() const {
	return SCREEN_INDEX_NO_SCREEN;
}

int EditorScreen::getPreviousScreenIndex() const {
	return SCREEN_INDEX_MAINMENU;
}

void EditorScreen::build() {
	
}

void EditorScreen::destroy() {
	
}

void EditorScreen::onEntry() {
	//init camera
	camera_.init(window_->getScreenWidth(), window_->getScreenHeight());
	camera_.setScale(1.0f);

	initUI();
	spriteBatch_.init();
	//shader init
	// Compile our color shader
	textureProgram_.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	textureProgram_.addAttribute("vertexPosition");
	textureProgram_.addAttribute("vertexColor");
	textureProgram_.addAttribute("vertexUV");
	textureProgram_.linkShaders();

	blankTexture_ = PuffEngine::ResourceManager::getTexture("Assets/blank.png");
}

void EditorScreen::onExit() {
	gui_.destroy();
	textureProgram_.dispose();
}

void EditorScreen::update() {
	camera_.update();
	checkInput();
}

void EditorScreen::draw() {
	// clear buffers and set background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.2f, 1.0f);

	// use shader and fragment programs
	textureProgram_.use();

	//upload texture uniform
	GLint textureUniform = textureProgram_.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);
	glActiveTexture(GL_TEXTURE0);

	//camera matrix
	glm::mat4 projectionMatrix = camera_.getCameraMatrix();
	GLint pUniform = textureProgram_.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);
	
	spriteBatch_.begin();
	{// Draw the color picker quad
		const float QUAD_SIZE = 50.f;

		glm::vec4 destRect;
		destRect.x = bSlider_->getXPosition().d_scale * window_->getScreenWidth() + 20.0f - window_->getScreenWidth() * 0.5f + QUAD_SIZE / 2.0f;
		destRect.y = window_->getScreenHeight() * 0.5f - bSlider_->getYPosition().d_scale * window_->getScreenHeight() - bSlider_->getHeight().d_scale * window_->getScreenHeight();
		destRect.z = QUAD_SIZE;
		destRect.w = QUAD_SIZE;

		spriteBatch_.draw(destRect, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), blankTexture_.id, 0.0f, PuffEngine::ColorRGBA8((GLubyte)colorPickerRed_, (GLubyte)colorPickerGreen_, (GLubyte)colorPickerBlue_, 255));
	}

	{//draw labels for rigid and dynamic buttons
		glm::vec2 pos;
		pos.x = rigidRadioButton_->getXPosition().d_scale * window_->getScreenWidth() - window_->getScreenWidth() * 0.5f ;
		pos.y = window_->getScreenHeight() / 2.0f - rigidRadioButton_->getYPosition().d_scale * window_->getScreenHeight();
		spriteFont_.draw(spriteBatch_, "Rigid", pos, glm::vec2(0.4f), 0.0f, PuffEngine::ColorRGBA8(255, 255, 255, 255), PuffEngine::Justification::MIDDLE);
	
		pos.x = dynamicRadioButton_->getXPosition().d_scale * window_->getScreenWidth() - window_->getScreenWidth() * 0.5f + 
			dynamicRadioButton_->getWidth().d_offset / 2.0f;
		pos.y = window_->getScreenHeight() / 2.0f - dynamicRadioButton_->getYPosition().d_scale * window_->getScreenHeight();
		spriteFont_.draw(spriteBatch_, "Dynamic", pos, glm::vec2(0.4f), 0.0f, PuffEngine::ColorRGBA8(255, 255, 255, 255), PuffEngine::Justification::MIDDLE);

	
	}

	spriteBatch_.end();
	spriteBatch_.renderBatch();
	textureProgram_.unUse();
	
	gui_.draw();
}

void EditorScreen::initUI() {
	// Init the UI
	gui_.init("GUI"); //static class (not any more)
	gui_.loadScheme("TaharezLook.scheme");
	// do not include .font
	gui_.setFont("DejaVuSans-10");

	// COLOR PICKER
	{// Color picker scope
		const float X_DIM = 0.015f, Y_DIM = 0.1f, Y_POS = 0.05f, PADDING = 0.01f;
		rSlider_ =static_cast<CEGUI::Slider*>(gui_.createWidget("TaharezLook/Slider", glm::vec4(0.05f, Y_POS, X_DIM, Y_DIM), glm::vec4(0), "RedSlider"));
		rSlider_->setMaxValue(255.0f);
		rSlider_->setCurrentValue(colorPickerRed_);
		rSlider_->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&EditorScreen::onColorPickerRedChange, this));
		rSlider_->setClickStep(1.0f);

		gSlider_ = static_cast<CEGUI::Slider*>(gui_.createWidget("TaharezLook/Slider", glm::vec4(0.05f + X_DIM + PADDING, Y_POS, X_DIM, Y_DIM), glm::vec4(0), "GreenSlider"));
		gSlider_->setMaxValue(255.0f);
		gSlider_->setCurrentValue(colorPickerRed_);
		gSlider_->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&EditorScreen::onColorPickerGreenChange, this));
		gSlider_->setClickStep(1.0f);

		bSlider_ = static_cast<CEGUI::Slider*>(gui_.createWidget("TaharezLook/Slider", glm::vec4(0.05f + (X_DIM + PADDING) * 2.0f, Y_POS, X_DIM, Y_DIM), glm::vec4(0), "BlueSlider"));
		bSlider_->setMaxValue(255.0f);
		bSlider_->setCurrentValue(colorPickerRed_);
		bSlider_->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&EditorScreen::onColorPickerBlueChange, this));
		bSlider_->setClickStep(1.0f);

	}

	{// Add the physics mod radio button
		const float X_POS = 0.23f, DIMS_PIXELS = 25.0f, PADDING = 0.06f;
		rigidRadioButton_ = static_cast<CEGUI::RadioButton*>(gui_.createWidget("TaharezLook/RadioButton", glm::vec4(X_POS, 0.05f, 0.0f, 0.0f), glm::vec4(0.0f, 0.0f, DIMS_PIXELS, DIMS_PIXELS), "RigidRadioButton"));
		rigidRadioButton_->setSelected(true);
		rigidRadioButton_->subscribeEvent(CEGUI::RadioButton::EventMouseClick, CEGUI::Event::Subscriber(&EditorScreen::onRigidMouseClick, this));


		dynamicRadioButton_ = static_cast<CEGUI::RadioButton*>(gui_.createWidget("TaharezLook/RadioButton", glm::vec4(X_POS, 0.05f + PADDING, 0.0f, 0.0f), glm::vec4(0.0f, 0.0f + PADDING, DIMS_PIXELS, DIMS_PIXELS), "DynamicRadioButton"));
		dynamicRadioButton_->setSelected(false);
		dynamicRadioButton_->subscribeEvent(CEGUI::RadioButton::EventMouseClick, CEGUI::Event::Subscriber(&EditorScreen::onDynamicMouseClick, this));


	}

	// exit button setup
	CEGUI::PushButton* exitButton = static_cast<CEGUI::PushButton*>(gui_.createWidget("TaharezLook/Button", glm::vec4(0.45f, 0.75f, 0.1f, 0.05f), glm::vec4(0.0f), "ExitButton"));
	exitButton->setText("ExitGame");
	// set the event to be called when clicked
	exitButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&EditorScreen::onExitClicked, this));
	exitButton->disable();
	exitButton->setVisible(false);

	//mouse cursor setup
	gui_.setMouseCursor("TaharezLook/MouseArrow");
	gui_.showMouseCursor();
	SDL_ShowCursor(0);
}

void EditorScreen::checkInput() {
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		gui_.onSDLEvent(evnt);

		switch (evnt.type) {
		case SDL_QUIT:
			onExitClicked(CEGUI::EventArgs());
		default:
			break;
		}
	}
}

bool EditorScreen::onExitClicked(const CEGUI::EventArgs& e) {
	currentState_ = PuffEngine::ScreenState::EXIT_APPLICATION;
	return true;
}

bool EditorScreen::onColorPickerRedChange(const CEGUI::EventArgs & e) {
	colorPickerRed_ = rSlider_->getCurrentValue();
	return true;
}

bool EditorScreen::onColorPickerGreenChange(const CEGUI::EventArgs & e) {
	colorPickerGreen_ = gSlider_->getCurrentValue();
	return true;
}

bool EditorScreen::onColorPickerBlueChange(const CEGUI::EventArgs & e) {
	colorPickerBlue_ = bSlider_->getCurrentValue();
	return true;
}

bool EditorScreen::onRigidMouseClick(const CEGUI::EventArgs & e) {
	physicsMode = PhysicsMode::RIGID;
	return true;
}

bool EditorScreen::onDynamicMouseClick(const CEGUI::EventArgs & e) {
	physicsMode = PhysicsMode::DYNAMIC;
	return true;
}

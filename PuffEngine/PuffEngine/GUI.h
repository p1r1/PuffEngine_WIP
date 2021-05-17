#pragma once

#include <CEGUI\CEGUI.h>
#include <CEGUI\RendererModules\OpenGL\GL3Renderer.h>
#include <SDL\SDL_events.h>
#include <glm\glm.hpp>

namespace PuffEngine {

class GUI {
public:
	//call this after init sdl and glew and sh,t
	void init(const std::string& resourceDirectory);
	void destroy();

	void draw();
	void update();

	void setMouseCursor(const std::string& imageFile);
	void showMouseCursor();
	void hideMouseCursor();

	void onSDLEvent(SDL_Event& evnt);

	void loadScheme(const std::string& schemeFile);
	void setFont(const std::string& fontFile);
	CEGUI::Window* createWidget(const std::string& type, const glm::vec4& destRectPerc, const glm::vec4& destRectPix, const std::string& name = "");
	static void setWidgetDestRect(CEGUI::Window* widget, const glm::vec4& destRectPerc, const glm::vec4& destRectPix);
	
	//getters
	static CEGUI::OpenGL3Renderer* getRenderer() { return renderer_; }
	const CEGUI::GUIContext* getContext() { return context_; }

private:
	static CEGUI::OpenGL3Renderer* renderer_;
	CEGUI::GUIContext* context_ = nullptr;
	CEGUI::Window* root_ = nullptr;
	unsigned int lastTime_ = 0;
};

}
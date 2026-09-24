#include "GameEngine.h"
#include "GameContext.h"
#include "DrawContext.h"
#include "GraphicsObject.h"

/// @brief
namespace CMPUT350 {
#include "FontData.h"

GameEngine::GameEngine(unsigned int width, unsigned int height, const std::string& name) 
: mWindow(std::make_shared<sf::RenderWindow>(sf::VideoMode({width,height}), name)),
 mFont(std::make_shared<sf::Font>()), mCloseGame(false) {
    // Sample font loading code
    	if (!mFont->openFromMemory(&_font, _font_len))
    	{
    	    fprintf(stderr, "WARNING: Font did not load.\n");
    	}
}

inline void GameEngine::PollWindow() {
    while (const std::optional<sf::Event> event = mWindow->pollEvent()) {
        mCloseGame = event->is<sf::Event::Closed>();
    }

    /*
    pollEvent is for more than just processing inputs. It's needed to process any requests and events
    the OS sends to it via the message queue. So not calling it (even if the game takes no input) leads
    to the message queue being full of ignored requests from the OS. Eventually the OS will think that the program
    is frozen.
    */
}

GameEngine::~GameEngine() {
    // Cleanup resources
    // mWindow->close();
}

void GameEngine::AddGameObject(std::shared_ptr<GameObject> gameObject) {
    mGameObjects.push_back(gameObject);
}

/**
 * @method Run
 * @arguments None
 * @description Gives control to the game engine. Will not return until the game window is closed or
 * all objects have been destroyed.
 */
void GameEngine::Run() {
    GameContext ctx;
    ctx.mEngineView = this;
    DrawContext drawCtx = DrawContext(mWindow, mFont); // placeholder w/ the nullptr
    ctx.ScreenContext = &drawCtx; 
    mWindow->setFramerateLimit(30); // Needed since physics are tied to the engine.
    // Not sure if this is supposed to be hardcoded or if the user sets it yet.
    

    while (mWindow->isOpen())  // window is open
    {

        

        // 0. Remove any objects that are now dead

        // 1. Activate and initialize any objects added during the last frame

        // 2. Process events
        PollWindow();
        if (mCloseGame) return;

        // 3. Update game objects
        for (auto& obj : mGameObjects) obj->Update(&ctx);

        // 4. Process collision events

        // 5. Late updates

        // Clear window
        mWindow->clear();

        // 6. Render background
        for (auto& obj : mGameObjects) {
            std::shared_ptr<CMPUT350::GraphicsObject> ptr = std::dynamic_pointer_cast<GraphicsObject>(obj);
            ptr->RenderBackground(&ctx); // the spec says we can just do this
         // even though this is probably bad for performance.
        }

        // 7. Render foreground
        for (auto& obj : mGameObjects) {
            std::shared_ptr<CMPUT350::GraphicsObject> ptr = std::dynamic_pointer_cast<GraphicsObject>(obj);
            ptr->RenderForeground(&ctx); 
            
        }


        // Actually render to window
        mWindow->display();
    }
}

// Sample code for processing events

// bool GameEngine::ProcessEvents(GameContext *context)
//{
//	while (const std::optional event = mWindow->pollEvent())
//	{
//		if (event->is<sf::Event::Closed>())
//		{
//		}
//		else if (event->is<sf::Event::Resized>())
//		{
//		}
//		else if (const auto* keyPressed = event->getIf<sf::Event::TextEntered>())
//		{
//			// use keyPressed->unicode to get character
//		}
//	}
// }

}  // namespace CMPUT350

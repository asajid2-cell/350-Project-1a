#include "GameEngine.h"
#include "GameContext.h"
#include "DrawContext.h"
#include "GraphicsObject.h"
#include "CollisionObject.h"

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
{
	while (const std::optional event = mWindow->pollEvent())
	{
        mCloseGame = event->is<sf::Event::Closed>();
		if (event->is<sf::Event::Resized>())
		{
		}
		else if (const auto* keyPressed = event->getIf<sf::Event::TextEntered>())
		{
			// use keyPressed->unicode to get character
		}
	}
 }
}


inline void GameEngine::ProcessCollisions() {
    for (auto it = mGameObjects.begin(); it != std::prev(mGameObjects.end()); ++it) { // for each game obj...
        // iterators
        // Worst case O(n^2) collision checking given n GameObjects 

            if (std::shared_ptr<CMPUT350::CollisionObject> col_ptr = std::dynamic_pointer_cast<CollisionObject>(*it)) {
                // if that game obj is a collision object...

                for (auto second_obj_it = std::next(it) ; second_obj_it != mGameObjects.end(); ++second_obj_it) {
                    if (std::shared_ptr<CMPUT350::CollisionObject> scd_ptr = std::dynamic_pointer_cast<CollisionObject>(*second_obj_it)) {

                        col_ptr->CollisionEnter(scd_ptr); // compare it with every (collision) object that comes after it in the vector
                        scd_ptr->CollisionEnter(col_ptr); // actually not 100% of this needs to be done both ways.

                    }
                }

            }
        }

}

GameEngine::~GameEngine() {
    // Cleanup resources
    // mWindow->close();
}

void GameEngine::AddGameObject(std::shared_ptr<GameObject> gameObject) {
    mLateGameObjects.push_back(gameObject);
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
    DrawContext drawCtx = DrawContext(mWindow, mFont); 
    ctx.ScreenContext = &drawCtx;

    mWindow->setFramerateLimit(30); // Needed since physics are tied to the engine.
    // Not sure if this is supposed to be hardcoded or if the user sets it yet.
    

    while (mWindow->isOpen())  // window is open
    {

        

        // 0. Remove any objects that are now dead
        auto remove_inds = std::remove_if(mGameObjects.begin(),mGameObjects.end(), [](const std::shared_ptr<GameObject>& obj) -> bool {
            return !(obj->IsAlive());
        }); 

        mGameObjects.erase(remove_inds, mGameObjects.end()); // if only we were on c++20

        // 1. Activate and initialize any objects added during the last frame
        mGameObjects.reserve(mGameObjects.size() + mLateGameObjects.size());
        mGameObjects.insert(mGameObjects.end(), std::make_move_iterator(mLateGameObjects.begin()), std::make_move_iterator(mLateGameObjects.end()));
        mLateGameObjects.clear();

        // 2. Process events
        PollWindow();
        if (mCloseGame) return;

        // 3. Update game objects
        for (auto& obj : mGameObjects) obj->Update(&ctx);

        // 4. Process collision events
        ProcessCollisions();

        // 5. Late updates
        for (auto& obj : mGameObjects) obj->LateUpdate(&ctx);

        // Clear window
        mWindow->clear();

        // 6. Render background
        for (auto& obj : mGameObjects) {
            if (std::shared_ptr<CMPUT350::GraphicsObject> ptr = std::dynamic_pointer_cast<GraphicsObject>(obj)) {
                ptr->RenderBackground(&ctx); }
                // this is bad for performance but the spec said it's fine.
        }

        // 7. Render foreground
        for (auto& obj : mGameObjects) {
            if (std::shared_ptr<CMPUT350::GraphicsObject> ptr = std::dynamic_pointer_cast<GraphicsObject>(obj)) {
                ptr->RenderForeground(&ctx); }
                // this is bad for performance but the spec said it's fine.
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

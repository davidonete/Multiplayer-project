//
//  Scene.h
//  Multiplayer Game
//
//  Created by David Parra on 08/12/16.
//  Copyright � 2016 David Parra. All rights reserved.
//

#ifndef SCENE_H
#define SCENE_H

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>
#include <SFGUI/Renderers.hpp> 
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

#include "Game/Platform.h"
#include "Game/Player.h"

enum GameScene
{
	GameScene_LogIn,
	GameScene_WaitRoom,
	GameScene_Battle
};

class Scene
{
public:
	Scene();
	virtual ~Scene();

    virtual void Init();
	virtual void Input();
	virtual void Update();
	virtual void Render();

	inline b2World* GetWorld() { return World; }

protected:
    void AddPlayer(Vec2 position, float32 rotation, float32 density, float32 friction, b2World* world, bool localPlayer);
    void AddPlatform(Vec2 position, float32 rotation, float32 density, float32 friction, b2World* world);
    void AddObject(Vec2 position, Vec2 scale, float rotation, BodyType type, float32 density, float32 friction, const char* texturePath, b2World* world);

	std::vector<Object*> Objects;
	b2World* World;

    std::vector<Player*> players;

private:

};

#endif
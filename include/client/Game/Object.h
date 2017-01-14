//
//  Object.h
//  Multiplayer Game
//
//  Created by David Parra on 08/12/16.
//  Copyright � 2016 David Parra. All rights reserved.
//

#ifndef OBJECT_H
#define OBJECT_H

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

#include "System/CollisionManager.h"

#pragma once

class GameManager;

//Pass from pixels to box2D coords
#define SCALE 30.0f 

struct Vec2
{
  float32 x;
  float32 y;
  Vec2()
  {
      x = 0.0f;
      y = 0.0f;
  }
  Vec2(float32 _x, float32 _y)
  {
    x = _x;
    y = _y;
  }
};

enum ObjectType
{
  StaticBody,
  DynamicBody
};

struct ObjectSprite
{
    Vec2 position;
    float32 rotation;
    //The middle coordinate of the sprite (half of the size)
    Vec2 spriteOrigin;
    //The texture reference
    sf::Texture texture;
    //The sprite that is going to be rendered on screens (needs to be updated constantly)
    sf::Sprite sprite;
};

struct ObjectInfo
{
    b2Body* body;
    ObjectSprite sprite;
};

class Object
{
public:
  Object();
  ~Object();

  //Only for square shapes
  Object(Vec2 position, Vec2 scale, float32 rotation, ObjectType type, float32 density, float32 friction, const char* texturePath, b2World* world);

  virtual void Init();
  virtual void Input();
  virtual void Update();
  virtual void Render();

  Vec2 GetPosition();
  void SetPosition(Vec2 pos);

  virtual void OnCollisionDetected(Object* otherObject);
  inline bool GetIsCollisionEnabled() { return collisionEnabled; };

protected:
  float32 GetRotation();
  ObjectInfo* GetObject() { return &object; }

  inline void SetCollisionEnabled(bool enable) { collisionEnabled = enable; };
  void SetRotaiton(float32 angle);

  void ApplyForce(Vec2 force);
  void ApplyImpulse(Vec2 impulse);

  void UpdateSprite(ObjectSprite* sprite, Vec2 pos, float32 rot);
  void RenderSprite(ObjectSprite* sprite);

  GameManager* GM;

private:
  ObjectInfo object;
  sf::RenderWindow* Window;

  bool collisionEnabled = false;
  CollisionManager* collisionManager;
};

#endif

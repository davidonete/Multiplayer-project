#include "Game/Player.h"
#include "System/GameManager.h"

Player::Player(Vec2 position, float32 rotation, float32 density, float32 friction, b2World * world, unsigned int ID)
    : Object(position, Vec2(55.0f, 55.0f), rotation, DynamicBody, density, friction, "player.png", world)
{
    playerID = ID;
    isPlayer = (GameManager::GetInstance()->Network->GetClientID() == ID);

    bomb.texture.loadFromFile(GameManager::GetImagePath("bomb.png"));
    bomb.spriteOrigin = Vec2(30.0f / 2.0f, 40.0f / 2.0f);

    if (isPlayer)
    {
        playerMark.texture.loadFromFile(GameManager::GetImagePath("Mark.png"));
        playerMark.spriteOrigin = Vec2(20.0f / 2.0f, 110.0f / 2.0f);
    }

    SetCollisionEnabled(true);
    jumping = false;

    SetType(Type_Player);

    lastPacketSent.ID = 0;
    lastPacketSent.rotation = 0.0f;
    lastPacketSent.x = 0.0f;
    lastPacketSent.y = 0.0f;
}

Player::~Player() {}

void Player::Init() {}

void Player::Input()
{
    //Linear velocity
    //Body->SetLinearVelocity(b2Vec2(inputValue, 0.0f));

    //Impulse
    /*
    b2Vec2 vel = Body->GetLinearVelocity();
    float inputValue = 5.0f;
    float velChange = inputValue - vel.x;
    float impulse = Body->GetMass() * velChange;
    Body->ApplyLinearImpulse(b2Vec2(impulse, 0), Body->GetWorldCenter(), true);
    */

    if (isPlayer)
    {
        //Force
        if (GM->CheckInputPressed(InputData_RightPressed))
            ApplyForce(Vec2(speed, 0.0f));

        if (GM->CheckInputPressed(InputData_LeftPressed))
            ApplyForce(Vec2(-speed, 0.0f));

        if (GM->CheckInputPressed(InputData_SpacePressed))
        {
            if (!jumping)
            {
                jumping = true;
                ApplyImpulse(Vec2(0.0, -jumpForce));
            }
        }
    }
}

void Player::Update()
{
    Object::Update();

    if(hasBomb)
        UpdateSprite(&bomb, GetPosition(), 0.0f);
    if (isPlayer)
        UpdateSprite(&playerMark, GetPosition(), 0.0f);

    UpdatePlayerServer();
}

void Player::Render()
{
    Object::Render();

    if (hasBomb && isPlayer)
        RenderSprite(&bomb);
    if (isPlayer)
        RenderSprite(&playerMark);
}

void Player::SetBomb(bool bomb)
{
    if (bomb)
    {
        speed = speed * 1.3f;
        hasBomb = bomb;
    }
    else
    { 
        speed = speed / 1.3f;
        hasBomb = bomb;
    }
}

void Player::OnCollisionDetected(Object* otherObject)
{
    if (otherObject->GetType() == Type_Ground && jumping)
        jumping = false;
    else if (otherObject->GetType() == Type_Player)
    {
        if (GM->Network->IsAuthority())
        {
            if (HaveBomb())
                SetBomb(false);
            else
                SetBomb(true);
        }
    }
}

void Player::UpdatePlayerServer()
{
    if (isPlayer)
    {
        Vec2 pos = GetPosition();
        float rot = GetRotation();
        float offset = 1.0f;
        if (!(pos.x > (lastPacketSent.x - offset) && pos.x < (lastPacketSent.x + offset)) || !(pos.y >(lastPacketSent.y - offset) && pos.y < (lastPacketSent.y + offset)) || !(rot >(lastPacketSent.rotation - offset) && rot < (lastPacketSent.rotation + offset)))
        {
            NetworkManager* Network = GM->Network;
            GamePacket packet;
            packet.ID = Network->GetClientID();
            packet.rotation = rot;
            packet.x = pos.x;
            packet.y = pos.y;
            GM->Network->SendPacket(packet);
            lastPacketSent = packet;
        }
    }
}
#include "System/GameManager.h"
#include "Scenes/LoginScene.h"
#include "Scenes/WaitRoomScene.h"
#include "Scenes/BattleScene.h"

//Path of assets on the different OS targets
#ifdef _WIN32
    #define DATA_PATH "../../../data/"
#elif __APPLE__
    #define DATA_PATH "data/"
#endif

GameManager* GameManager::Instance = nullptr;

GameManager::GameManager() {}

GameManager::~GameManager() 
{
    delete Network;
    delete Window;
    delete SceneInstance;
    LastInputPressed.clear();
}

GameManager* GameManager::GetInstance()
{
    if (Instance == nullptr)
    {
        Instance = new GameManager();
        Instance->Init();
    }

    return Instance;
}

void GameManager::Init()
{
    //Initialize window (width, height, bits per pixel)
    Window = new sf::RenderWindow(sf::VideoMode(800, 600, 32), "Boom Box - v1.0");
    //Window = new sf::RenderWindow(sf::VideoMode::getDesktopMode(), "Super Awesome Game - v1.0");//, sf::Style::Fullscreen);
    Window->setFramerateLimit(60);

    Network = new NetworkManager();
    Network->Init();

    SceneInstance = nullptr;

    LastInputPressed.reserve(1);
}

void GameManager::Input()
{
	UpdateInput();
	SceneInstance->Input();
}

void GameManager::Update()
{
	SceneInstance->Update();
}

void GameManager::Render()
{
    Window->clear(sf::Color::White);

    SceneInstance->Render();

    //Update the window rendering
    Window->display();
}

void GameManager::UpdateInput()
{
    LastInputPressed.clear();
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        LastInputPressed.push_back(InputData_RightPressed);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        LastInputPressed.push_back(InputData_LeftPressed);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        LastInputPressed.push_back(InputData_SpacePressed);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
        LastInputPressed.push_back(InputData_EnterPressed);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
        LastInputPressed.push_back(InputData_TabPressed);
}

bool GameManager::CheckInputPressed(InputData InputType)
{
    for(unsigned int i = 0; i < LastInputPressed.size(); i++)
    {
        if(LastInputPressed[i] == InputType)
            return true;
    }
    return false;
}

void GameManager::ChangeScene(GameScene scene)
{
    if (SceneInstance != nullptr)
    {
        delete SceneInstance;
        SceneInstance = nullptr;
    }

    switch (scene)
    {
        case GameScene_LogIn:
            SceneInstance = new LoginScene();
        break;

        case GameScene_WaitRoom:
            SceneInstance = new WaitRoomScene();
        break;

        case GameScene_Battle:
            SceneInstance = new BattleScene();
        break;

        default:
            SceneInstance = new Scene();
        break;
    }

    SceneInstance->Init();
}

void GameManager::CloseGame()
{
    Network->LogOut();
    Network->Disconnect(TCP);
    Network->Disconnect(UDP);
    Window->close();
}

std::string GameManager::GetImagePath(const char* filename) 
{
    std::string path = DATA_PATH;
    std::string at = "images/";
    return path.append(at.append(filename));
}

std::string GameManager::GetFontPath(const char* filename)
{
    std::string path = DATA_PATH;
    std::string at = "fonts/";
    return path.append(at.append(filename));
}

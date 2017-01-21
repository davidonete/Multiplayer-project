//
//  NetworkManager.h
//  Multiplayer Game
//
//  Created by David Parra on 08/12/16.
//  Copyright � 2016 David Parra. All rights reserved.
//

#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include "SFML/Network.hpp"

enum ConnectionType
{
  TCP,
  UDP
};

enum ServerMessage
{
  Server_Denied,
  Server_Denied_NotAuthority,
  Server_Denied_AlreadyLogged,
  Server_Denied_NotEnoughPlayers,
  Server_Accepted,
  Server_AcceptedAuthority,
  Server_PlayerConnected,
  Server_PlayerDisconnected,
  Server_StartBattleScene,
  Server_StartBattle,
};

enum RequestMessage
{
  Request_GetPlayersInfo,
  Request_DisconnectPlayer,
  Request_StartBattleScene,
  Request_PlayerReady,
};

enum PacketType
{
  Type_Null,
  Type_GamePacket,
  Type_ChatPacket,
  Type_ServerMessagePacket,
  Type_PlayerInfoPacket,
  Type_ClientRequestPacket,
};

struct LogInPacket
{
  char username[32];
  char password[32];
};

struct GamePacket
{
  unsigned int ID;
  float x, y, rotation;
};

struct PlayerInfoPacket
{
  unsigned int ID;
  unsigned int wincount;
  unsigned int authority;
  char username[32];
};

struct ChatPacket
{
  unsigned int ID;
  char message[50];
};

struct ServerMessagePacket
{
  unsigned int msg;
};

struct ClientRequestPacket
{
  unsigned int ID;
  unsigned int msg;
};

struct PlayerInformation
{
  unsigned int ID;
  unsigned int wincount;
  bool authority;
  std::string username;
};

class NetworkManager
{
public:
  NetworkManager();
  ~NetworkManager();

  void Init();
  bool Connect(ConnectionType type);
  void Disconnect(ConnectionType type);

  bool SendPacket(LogInPacket packet);
  bool SendPacket(ChatPacket packet);
  bool SendPacket(ClientRequestPacket packet);
  bool SendPacket(GamePacket packet);

  bool ReceivePacket(ConnectionType Type, char buffer[]);
  bool GetPacketFromBytes(char bytes[], ChatPacket &packet);
  bool GetPacketFromBytes(char bytes[], ServerMessagePacket &packet);
  bool GetPacketFromBytes(char bytes[], ClientRequestPacket &packet);
  bool GetPacketFromBytes(char bytes[], PlayerInfoPacket &packet);
  bool GetPacketFromBytes(char bytes[], GamePacket &packet);

  static PacketType GetPacketType(char bytes[]);

  static inline ServerMessage GetServerMessage(unsigned int code) { return static_cast<ServerMessage>(code); }
  static inline unsigned int GetCodeFromRequest(RequestMessage msg) { return static_cast<unsigned int>(msg); }

  /** Gets and sets if the current client is the authority of the game. */
  inline void SetAuthority(bool authority) { client.authority = authority; }
  inline bool IsAuthority() { return client.authority; }

  /** Gets/Sets the id used by the server to identify this client. */
  inline unsigned int GetClientID() { return client.ID; }
  inline void SetClientID(unsigned int id) { client.ID = id; }
  inline std::string GetUsername() { return client.username; }
  inline void SetUsername(std::string username) { client.username = username; }

  static unsigned int GetSizeOfBytes(char bytes[]);
  void LogOut();

private:
  sf::TcpSocket tcpSocket;
  sf::UdpSocket udpSocket;

  PlayerInformation client;
};

#endif
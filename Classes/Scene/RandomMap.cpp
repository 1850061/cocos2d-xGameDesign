#include "AdventureMapScene.h"


#define EMPTY 0
#define BEGIN 1
#define BONUS 2
#define SHOP 3
#define ENEMY 4
#define BOSS 5
#define END 6

#define ROOM_NUM 7

#define EMPTY_TILE 0
#define BLACK_TILE 55

#define WALL_TILE_UP 12
#define WALL_TILE_DOWN 12
#define WALL_TILE_LEFT 12
#define WALL_TILE_RIGHT 12

#define DOOR_TILE_UP
#define DOOR_TILE_DOWN
#define DOOR_TILE_LEFT
#define DOOR_TILE_RIGHT

#define ROOM_TILE 56

#define ROAD_TILE 56


USING_NS_CC;


int coord[25][2] = {
		{11,11},{52,11},{93,11},{134,11},{175,11},
		{11,52},{52,52},{93,52},{134,52},{175,52},
		{11,93},{52,93},{93,93},{134,93},{175,93},
		{11,134},{52,134},{93,134},{134,134},{175,134},
		{11,175},{52,175},{93,175},{134,175},{175,175} };//25���������������


bool onMap(int x, int y)
{
	return x >= 0 && x <= 4 && y >= 0 && y <= 4;
}


//����һ��������ɵ���Ϸ��ͼ
void AdventureMapLayer::createRandomMap()
{
	m_tileMap = TMXTiledMap::create("AdventureMap_random.tmx");

	m_collidable = m_tileMap->getLayer("barrier");//��ȡ�ж���ײ���ϰ���

	m_ground = m_tileMap->getLayer("ground");//��ȡ�����

	m_wall = m_tileMap->getLayer("wall");//��ȡǽ�ڲ�

	m_road = m_tileMap->getLayer("road");//��ȡ������

	int rooms[5][5] = { 0 };//�����ռ�����

	std::vector<Vec2>occupiedRoom = {};//ռ�÷��������

	std::vector<std::pair<Vec2, Vec2>>roadPairs = {};//����������ĵ�·

	Vec2 startRoom = Vec2(2, 2);//��ʼ����

	int roomX = startRoom.x;
	int roomY = startRoom.y;

	rooms[roomX][roomY] = BEGIN;
	occupiedRoom.push_back(startRoom);

	Vec2 dir[4] = { {0,1},{0,-1},{1,0},{-1,0} };//�ĸ�����

	srand(time(nullptr));

	int randDir = rand() % 4;

	Vec2 nextRoom = Vec2(startRoom.x + dir[randDir].x, startRoom.y + dir[randDir].y);//��ʼ�������һ����������
	rooms[static_cast<int>(nextRoom.x)][static_cast<int>(nextRoom.y)] = ENEMY;
	occupiedRoom.push_back(nextRoom);

	roadPairs.push_back({ startRoom,nextRoom });
	
	int roomCnt = 2;

	while (roomCnt < ROOM_NUM)//�������7������
	{
		int nearRoomCnt = 0;//���ڷ���
		std::vector<int>dirVec = {};//�������ߵķ���
		for (int i = 0; i < 4; i++)//�����ĸ�����
		{
			if (onMap(nextRoom.x + dir[i].x, nextRoom.y + dir[i].y) && 
				rooms[static_cast<int>(nextRoom.x + dir[i].x)][static_cast<int>(nextRoom.y + dir[i].y)] == EMPTY)
			{
				//if ()
				{
					dirVec.push_back(i);//�����ߵķ������dirVec
				}
			}
		}
		while (nearRoomCnt != dirVec.size())//�����1~3������
		{
			randDir = rand()% dirVec.size();//ѡȡ�������
			roomX = static_cast<int>(nextRoom.x + dir[dirVec[randDir]].x);
			roomY = static_cast<int>(nextRoom.y + dir[dirVec[randDir]].y);
			if (rooms[roomX][roomY] == EMPTY)
			{
				occupiedRoom.push_back(Vec2(roomX, roomY));//���뷿��

				roadPairs.push_back({ nextRoom,Vec2(roomX, roomY) });//�����·

				nearRoomCnt++;
				roomCnt++;

				if (roomCnt == ROOM_NUM)
				{
					rooms[roomX][roomY] = END;
					break;
				}
				else if (roomCnt == ROOM_NUM - 2)
				{
					rooms[roomX][roomY] = BONUS;
				}
				else
				{
					rooms[roomX][roomY] = ENEMY;
				}
			}
		}
		nextRoom = Vec2(roomX, roomY);//���·���
	}

	for (auto elem : occupiedRoom)
	{
		int x = elem.x;
		int y = elem.y;
		int z = 5 * x + y;
		buildRoom(Vec2(coord[z][0], coord[z][1]));
	}

	for (auto elem : roadPairs)
	{
		buildRoad(elem);
	}
}

void AdventureMapLayer::buildRoom(Vec2 roomCoord)
{
	int roomX = roomCoord.x;
	int roomY = roomCoord.y;
	for (int i = roomX - 10; i <= roomX + 10; i++)
	{
		for (int j = roomY - 10; j <= roomY + 10; j++)
		{
			//m_ground->getTileAt(Vec2(i, j))->setVisible(true);
			m_ground->setTileGID(ROOM_TILE, Vec2(i, j));
			m_collidable->setTileGID(EMPTY_TILE, Vec2(i, j));
		}
	}
}

void AdventureMapLayer::buildRoad(std::pair<cocos2d::Vec2, cocos2d::Vec2> roadPair)
{
	Vec2 room1 = roadPair.first;
	Vec2 room2 = roadPair.second;
	int roomCoord1 = 5 * room1.x + room1.y;//��������ķ�������
	int roomCoord2 = 5 * room2.x + room2.y;
	if (room1.x == room2.x)//������������
	{
		int midTileCoordX = (coord[roomCoord1][0] + coord[roomCoord2][0]) / 2;//��������ת��Ϊ��Ƭ����
		int tileCoordY = coord[roomCoord1][1];
		for (int i = midTileCoordX - 9; i <= midTileCoordX + 10; i++)//��·
		{
			m_wall->setTileGID(WALL_TILE_DOWN, Vec2(i, tileCoordY + 3));
			m_wall->setTileGID(WALL_TILE_UP, Vec2(i, tileCoordY - 3));
			for (int j = tileCoordY - 2; j <= tileCoordY + 2; j++)
			{
				m_road->setTileGID(ROAD_TILE, Vec2(i, j));
				m_collidable->setTileGID(0, Vec2(i, j));
			}
		}
		int leftDoorX = midTileCoordX - 9;
		int rightDoorX = midTileCoordX + 10;
		for (int i = tileCoordY - 3; i <= tileCoordY + 3; i++)//��ǽ
		{
			m_wall->setTileGID(ROAD_TILE, Vec2(leftDoorX, i));
			m_wall->setTileGID(ROAD_TILE, Vec2(rightDoorX, i));
		}
	}
	else//������������
	{
		int tileCoordX = coord[roomCoord1][0];//��������ת��Ϊ��Ƭ����
		int midTileCoordY = (coord[roomCoord1][1] + coord[roomCoord2][1]) / 2;
		for (int i = midTileCoordY - 9; i <= midTileCoordY + 10; i++)
		{
			m_wall->setTileGID(WALL_TILE_LEFT, Vec2(tileCoordX - 3,i));
			m_wall->setTileGID(WALL_TILE_RIGHT, Vec2(tileCoordX + 3,i));
			for (int j = tileCoordX - 2; j <= tileCoordX + 2; j++)
			{
				m_road->setTileGID(ROAD_TILE, Vec2(j, i));
				m_collidable->setTileGID(EMPTY_TILE, Vec2(j, i));
			}
		}
		int upDoorX = midTileCoordY + 10;
		int downDoorX = midTileCoordY - 9;
		for (int i = tileCoordX - 3; i <= tileCoordX + 3; i++)//��ǽ
		{
			m_wall->setTileGID(ROAD_TILE, Vec2(i, upDoorX));
			m_wall->setTileGID(ROAD_TILE, Vec2(i, downDoorX));
		}
	}
}

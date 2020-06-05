#include "Scene/TollgateScene.h"
#include "SafeMapScene.h"

USING_NS_CC;

Scene* TollgateScene::createScene()
{
	auto scene = Scene::create();
	auto layer = TollgateScene::create();
	scene->addChild(layer);
	Director::getInstance()->setProjection(Director::Projection::_2D);
	return scene;
}

void TollgateScene::loadMap()
{
	m_map = AdventureMapLayer::create();
	this->addChild(m_map, 0, 100);//��Ϸ��ͼ tagΪ100

}

void TollgateScene::addPlayer()
{
	TMXObjectGroup* group = m_map->getMap()->getObjectGroup("objects");//��ȡ�����
	ValueMap spawnPoint = group->getObject("hero");//����hero�����λ�÷��þ���
	float x = spawnPoint["x"].asFloat();
	float y = spawnPoint["y"].asFloat();
	m_player = Ranger::create();
	m_player->setPosition(Vec2(x, y));

	m_player->setTiledMap(m_map);
	m_player->setCdBar(m_cdBar);
	m_map->addChild(m_player, 2, 200);
}

void TollgateScene::addLongRangeWeapon()
{
	m_player->setLongRange(CandyGun::create());
}

void TollgateScene::loadController()
{
	auto animate = m_player->stand();
	auto* playerController = PlayerController::create();
	playerController->setiXSpeed(0);
	playerController->setiYSpeed(0);
	this->addChild(playerController);
	m_player->setController(playerController);
	playerController->setPlayer(m_player);
	playerController->setIsRanger(typeid(*m_player) == typeid(Ranger));//�Ժ���memberSelect���
	playerController->setStandAnimate(animate);

}



bool TollgateScene::init()
{
	if (!Layer::init())
		return false;

	this->scheduleUpdate();

	loadMap();
	loadUI();
	addPlayer();
	//addLongRangeWeapon();
	loadController();
	loadMonsters();
	return true;
}

void TollgateScene::onEnter()
{
	Layer::onEnter();

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto okMenuItem = MenuItemImage::create("menu/CheckMark.png", "menu/CheckMark.png", CC_CALLBACK_1(TollgateScene::menuOkCallback, this));
	okMenuItem->setPosition(Vec2(visibleSize.width - 50, visibleSize.height - 100));

	auto menu = Menu::create(okMenuItem, nullptr);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);
	
	addLongRangeWeapon();

	loadUI();
}


void TollgateScene::loadUI()
{
	auto UI = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("INGAME_1.ExportJson");
	this->addChild(UI, 0, 0);
	m_cdBar = (LoadingBar*)Helper::seekWidgetByName(UI, "ability_loading_bar");

	/*auto pause_button = (Button*)Helper::seekWidgetByName(UI, "pause_button");
	pause_button->addTouchEventListener(this, toucheventselector(HelloWorld::pauseEvent));*/
}


void TollgateScene::loadMonsters()
{
	auto monsterMgr = MonsterManager::create();
	monsterMgr->bindMap(m_map);
	monsterMgr->bindPlayer((Sprite*)(this->m_player));
	auto playerPos = this->convertToNodeSpace(m_player->getPosition());
	playerPos.x -=   9 * 32;
	playerPos.y -=  9 * 32;
	monsterMgr->setPosition(playerPos);
	m_map->addChild(monsterMgr, 2);
}


const int coord[25][2] = {
		{11,11},{52,11},{93,11},{134,11},{175,11},
		{11,52},{52,52},{93,52},{134,52},{175,52},
		{11,93},{52,93},{93,93},{134,93},{175,93},
		{11,134},{52,134},{93,134},{134,134},{175,134},
		{11,175},{52,175},{93,175},{134,175},{175,175} };

Vec2 lastRoomCoord(2, 2);

void TollgateScene::updateMiniMap(TMXTiledMap* miniMap)
{
	auto miniMapLayer = miniMap->getLayer("miniMapLayer");
	auto playerPos = m_player->getPosition();
	auto roomCoord = m_map->roomCoordFromPosition(playerPos);

	miniMap->setPosition(playerPos + Vec2(200, 50));

	if (roomCoord == Vec2(-1, -1))
	{
		return;
	}

	miniMapLayer->setTileGID(2, 2 * lastRoomCoord);//ԭ����ǳ��
	miniMapLayer->setTileGID(1, 2 * Vec2(roomCoord.y, roomCoord.x));//�ַ������

	if (lastRoomCoord != Vec2(roomCoord.y, roomCoord.x))
	{
		if (lastRoomCoord.x == roomCoord.y)//��������
		{
			miniMapLayer->setTileGID(4, lastRoomCoord + Vec2(roomCoord.y, roomCoord.x));
		}
		else//��������
		{
			miniMapLayer->setTileGID(3, lastRoomCoord + Vec2(roomCoord.y, roomCoord.x));
		}
	}
	lastRoomCoord = Vec2(roomCoord.y, roomCoord.x);
}

void TollgateScene::update(float dt)
{
	auto playerPos = m_player->getPosition();
	auto barrier = m_map->getCollidable();
	auto map = m_map->getMap();
	auto miniMap = m_map->getMiniMap();
	auto wall = m_map->getWall();
	auto roadPairs = m_map->getRoadPairs();

	updateMiniMap(miniMap);

	auto roomCoord = m_map->roomCoordFromPosition(playerPos);//��������
	auto roomNum = roomCoord.x * 5 + roomCoord.y;//�������

	Vec2 dir[4] = { {0,1},{0,-1},{1,0},{-1,0} };//�ĸ�����

	if (true)//�����й���ķ��䣬��ʼս��
	{
		std::vector<int>dirVec;
		for (int i = 0; i < 4; i++)
		{
			for (auto elem : roadPairs)
			{
				if (elem.first == roomCoord && elem.second == dir[i] + roomCoord || 
					elem.second == roomCoord && elem.first == dir[i] + roomCoord)
				{
					dirVec.push_back(i);
				}
			}
		}
		for (auto elem : dirVec)
		{
			AdventureMapLayer::switchGate(wall, barrier, roomNum, elem, true);
		}
		auto t = time(nullptr);
		if (t%2)//����ս��
		{
			for (auto elem : dirVec)
			{
				AdventureMapLayer::switchGate(wall, barrier, roomNum, elem, false);
			}
		}
	}
}

void TollgateScene::menuOkCallback(cocos2d::Ref* pSender)
{
	auto scene = SafeMapLayer::createScene();
	Director::getInstance()->replaceScene(scene);
}

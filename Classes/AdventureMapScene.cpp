#include "AdventureMapScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* AdventureMapLayer::createScene()
{
    auto scene = Scene::create();

    auto layer = AdventureMapLayer::create();

    scene->addChild(layer);

    return scene;
}

// on "init" you need to initialize your instance
bool AdventureMapLayer::init()
{
    //1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    this->scheduleUpdate();

    createRandomMap();
    this->addChild(m_tileMap, 0, 100);//��Ϸ��ͼ tagΪ100

    TMXObjectGroup* group = m_tileMap->getObjectGroup("objects");//��ȡ�����
    ValueMap spawnPoint = group->getObject("hero");//����hero�����λ�÷��þ���
    
    float x = spawnPoint["x"].asFloat();
    float y = spawnPoint["y"].asFloat();

    m_player = Sprite::create("map/hero.png");
    m_player->setPosition(Vec2(x,y));

    this->addChild(m_player, 2, 200);//��Ϸ���� tagΪ200

    setViewpointCenter(m_player->getPosition());
    
    return true;
}

void AdventureMapLayer::onEnter()
{
    Layer::onEnter();

    auto listener = EventListenerKeyboard::create();//���������¼�

    listener->onKeyPressed = [=](EventKeyboard::KeyCode keycode, Event* event)
    {
        m_keyMap[keycode] = true;
    };
    
    listener->onKeyReleased = [=](EventKeyboard::KeyCode keycode, Event* event)
    {
        m_keyMap[keycode] = false;
    };

    EventDispatcher* eventDispatcher = Director::getInstance()->getEventDispatcher();
    eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void AdventureMapLayer::onExit()
{
    Layer::onExit();
    Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
}

void AdventureMapLayer::update(float dt)
{
    auto moveUp = EventKeyboard::KeyCode::KEY_W;
    auto moveDown = EventKeyboard::KeyCode::KEY_S;
    auto moveLeft = EventKeyboard::KeyCode::KEY_A;
    auto moveRight = EventKeyboard::KeyCode::KEY_D;

    Vec2 offset(0, 0);//ƫ����
    
    float dx = 0, dy = 0;

    //ÿ֡�ƶ���������
    if (m_keyMap[moveUp])
    {
        offset.y = 4;
    }
    if (m_keyMap[moveDown])
    {
        offset.y = -4;
    }
    if (m_keyMap[moveLeft])
    {
        offset.x = -4;
    }
    if (m_keyMap[moveRight])
    {
        offset.x = 4;
    }

    auto playerPos = m_player->getPosition();
    this->setPlayerPosition(playerPos + offset,offset.x,offset.y);

    //auto moveBy = MoveBy::create(0.001f, m_player->getPosition()-playerPos);
    //m_player->runAction(moveBy);
}



void AdventureMapLayer::setPlayerPosition(Vec2 position,int dx,int dy)
{
    Vec2 tileCoord = this->tileCoordFromPosition(position);//��������ת��Ϊ��Ƭ����

    int tileGid = m_collidable->getTileGIDAt(tileCoord);//�����Ƭ��GID

    if (tileGid != 0)//��Ƭ�Ƿ���ڣ�������ʱtileGid==0��
    {
        
        auto prop = m_tileMap->getPropertiesForGID(tileGid);
        auto valueMap = prop.asValueMap();
        bool collision = valueMap["Collidable"].asBool();
        if (collision)//��ײ���
        {
            m_player->setPosition(position-Vec2(dx,dy));//�ص�������Ῠǽ��
            return;
        }
    }
    m_player->setPosition(position);//�ƶ�����

    this->setViewpointCenter(m_player->getPosition());//������ͼ
}

//��������ת��Ϊ��Ƭ����
cocos2d::Vec2 AdventureMapLayer::tileCoordFromPosition(cocos2d::Vec2 pos)
{
    int x = pos.x / m_tileMap->getTileSize().width;
    int y = ((m_tileMap->getMapSize().height * m_tileMap->getTileSize().height) - pos.y) / m_tileMap->getTileSize().height;

    return Vec2(x, y);
}

//�����ﱣ������Ļ�м�
void AdventureMapLayer::setViewpointCenter(cocos2d::Vec2 position)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();

    //��ֹ��Ļ��߳�����ͼ
    float x = MAX(position.x, visibleSize.width / 2);
    float y = MAX(position.y, visibleSize.height / 2);
    //��ֹ��Ļ�ұ߳�����ͼ
    x = MIN(x, (m_tileMap->getMapSize().width * m_tileMap->getMapSize().width) - visibleSize.width / 2);
    y = MIN(y, (m_tileMap->getMapSize().height * m_tileMap->getMapSize().height) - visibleSize.height / 2);

    Vec2 pointA = Vec2(visibleSize.width / 2, visibleSize.height / 2);//��Ļ���е�
    Vec2 pointB = Vec2(x, y);//Ŀ���

    Vec2 offset = pointA - pointB;//ƫ����

    this->setPosition(offset);
}



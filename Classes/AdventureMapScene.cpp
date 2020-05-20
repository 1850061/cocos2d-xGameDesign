#include "AdventureMapScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();

    auto layer = HelloWorld::create();

    scene->addChild(layer);

    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    this->scheduleUpdate();

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    _tileMap = TMXTiledMap::create("AdventureMapLevel1.tmx");//������ͼ
    addChild(_tileMap, 0, 100);//��Ϸ��ͼ tagΪ100

    TMXObjectGroup* group = _tileMap->getObjectGroup("objects");//��ȡ�����
    ValueMap spawnPoint = group->getObject("hero");//����hero�����λ�÷��þ���
    
    float x = spawnPoint["x"].asFloat();
    float y = spawnPoint["y"].asFloat();

    _player = Sprite::create("hero.png");
    _player->setPosition(Vec2(x,y));
    log("starting position:(%d,%d)", x, y);
    addChild(_player, 2, 200);//��Ϸ���� tagΪ200

    setViewpointCenter(_player->getPosition());

    _collidable = _tileMap->getLayer("barrier");//��ȡ�ж���ײ���ϰ���
    //_collidable->setVisible(false);

    return true;
}

void HelloWorld::onEnter()
{
    Layer::onEnter();

    auto listener = EventListenerKeyboard::create();//���������¼�

    listener->onKeyPressed = [=](EventKeyboard::KeyCode keycode, Event* event)
    {
        _keyMap[keycode] = true;
    };
    
    listener->onKeyReleased = [=](EventKeyboard::KeyCode keycode, Event* event)
    {
        _keyMap[keycode] = false;
    };

    EventDispatcher* eventDispatcher = Director::getInstance()->getEventDispatcher();
    eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void HelloWorld::onExit()
{
    Layer::onExit();
    Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
}

void HelloWorld::update(float dt)
{
    auto moveUp = EventKeyboard::KeyCode::KEY_W;
    auto moveDown = EventKeyboard::KeyCode::KEY_S;
    auto moveLeft = EventKeyboard::KeyCode::KEY_A;
    auto moveRight = EventKeyboard::KeyCode::KEY_D;

    Vec2 offset(0, 0);//ƫ����
    //ÿ֡�ƶ���������
    if (_keyMap[moveUp])
    {
        offset.y = 2;
    }
    if (_keyMap[moveDown])
    {
        offset.y = -2;
    }
    if (_keyMap[moveLeft])
    {
        offset.x = -2;
    }
    if (_keyMap[moveRight])
    {
        offset.x = 2;
    }

    auto playerPos = _player->getPosition();
    this->setPlayerPosition(playerPos + offset,offset.x,offset.y);

    auto moveBy = MoveBy::create(0.01f, _player->getPosition()-playerPos);
    _player->runAction(moveBy);
}

void HelloWorld::setPlayerPosition(Vec2 position,int dx,int dy)
{
    Vec2 tileCoord = this->tileCoordFromPosition(position);//��������ת��Ϊ��Ƭ����

    int tileGid = _collidable->getTileGIDAt(tileCoord);//�����Ƭ��GID
 
    if (tileGid != 0)//��Ƭ�Ƿ���ڣ�������ʱtileGid==0��
    {
        
        auto prop = _tileMap->getPropertiesForGID(tileGid);
        auto valueMap = prop.asValueMap();
        bool collision = valueMap["Collidable"].asBool();
        if (collision == true)//��ײ���
        {
            _player->setPosition(position-Vec2(dx,dy));//�ص�������Ῠǽ��
            return;
        }
    }
    _player->setPosition(position);//�ƶ�����

    this->setViewpointCenter(_player->getPosition());//������ͼ
}

//��������ת��Ϊ��Ƭ����
cocos2d::Vec2 HelloWorld::tileCoordFromPosition(cocos2d::Vec2 pos)
{
    int x = pos.x / _tileMap->getTileSize().width;
    int y = ((_tileMap->getMapSize().height * _tileMap->getTileSize().height) - pos.y) / _tileMap->getTileSize().height;

    return Vec2(x, y);
}

//�����ﱣ������Ļ�м�
void HelloWorld::setViewpointCenter(cocos2d::Vec2 position)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();

    //��ֹ��Ļ��߳�����ͼ
    float x = MAX(position.x, visibleSize.width / 2);
    float y = MAX(position.y, visibleSize.height / 2);
    //��ֹ��Ļ�ұ߳�����ͼ
    x = MIN(x, (_tileMap->getMapSize().width * _tileMap->getMapSize().width) - visibleSize.width / 2);
    y = MIN(y, (_tileMap->getMapSize().height * _tileMap->getMapSize().height) - visibleSize.height / 2);

    Vec2 pointA = Vec2(visibleSize.width / 2, visibleSize.height / 2);//��Ļ���е�
    Vec2 pointB = Vec2(x, y);//Ŀ���

    Vec2 offset = pointA - pointB;//ƫ����

    this->setPosition(offset);
}

#include "Scene\PauseScene.h"
#include "Scene\HomeScene.h"
#include "Scene\SettingScene.h"
#include "GameData.h"

bool PauseScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	return true;
}

Scene* PauseScene::createScene(CCRenderTexture* background, int player)
{
	Scene* scene = Scene::create();
	PauseScene* layer = PauseScene::create();
	layer->setPlayer(player);
	layer->loadUI();
	scene->addChild(layer, 1);

	Size visible_size = Director::getInstance()->getVisibleSize();
	Sprite* background_picture = Sprite::createWithTexture(background->getSprite()->getTexture());
	background_picture->setPosition(visible_size.width / 2, visible_size.height / 2);
	background_picture->setFlippedY(true);
	scene->addChild(background_picture, 0);

	return scene;
}

void PauseScene::backToHomeMenu(Ref*, TouchEventType type)
{
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		Director::getInstance()->popScene();
		GameData::setCoinNum(0);
		GameData::setLastRoomCoord(Vec2(2, 2));
		GameData::setLevel(1);
		Director::getInstance()->replaceScene(HomeMenuLayer::createScene());
		break;
	}
}

void PauseScene::resume(Ref*, TouchEventType type)
{
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		Director::getInstance()->popScene();
		break;
	}
}

void PauseScene::goToSettingMenu(Ref*, TouchEventType type)
{
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		Director::getInstance()->pushScene(SettingLayer::createScene());
		break;
	}
}

void PauseScene::loadUI()
{
	auto UI = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("PauseScene_1.ExportJson");
	this->addChild(UI, 0, 0);

	UI->setTouchEnabled(false);

	auto home_button = (Button*)Helper::seekWidgetByName(UI, "home_button");
	home_button->addTouchEventListener(this, toucheventselector(PauseScene::backToHomeMenu));
	auto resume_button = (Button*)Helper::seekWidgetByName(UI, "resume_button");
	resume_button->addTouchEventListener(this, toucheventselector(PauseScene::resume));
	auto setting_button = (Button*)Helper::seekWidgetByName(UI, "setting_button");
	setting_button->addTouchEventListener(this, toucheventselector(PauseScene::goToSettingMenu));
	auto player_image = (ImageView*)Helper::seekWidgetByName(UI, "player_image");

	if (m_player == 2)
	{
		player_image->loadTexture("priest_image.jpg");
	}
}

void PauseScene::setPlayer(int player)
{
	m_player = player;
}
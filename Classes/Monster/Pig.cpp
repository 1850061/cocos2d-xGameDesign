#include "Pig.h"

bool Pig::init()
{
	m_fIsFacingRight = false;
	setMonsterSpeed(0.5);
	m_damageMsg = FlowWord::create();
	this->addChild(m_damageMsg);
	setHp(this->m_initHp);
	setResTrack("Pig.png");
	this->bindSprite(Sprite::create(this->getResTrack()),0.6f,0.6f);
	auto sword = MonsterSword::create();
	//sword->setPosition(this->getPosition());
	setMonsterWeapon(sword);
	this->addChild(sword, 1);
	m_isAlive = true;
	return true;
}

void Pig::resetPropoties() 
{
	show();
	setHp(this->m_initHp);
	m_weapon->setVisible(true);
	m_fIsTaunted = 0;
	m_isAlive = true;
}
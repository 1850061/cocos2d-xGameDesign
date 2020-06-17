﻿#include "Entity\Weapons\Weapon.h"
#define PI 3.1415926

Weapon::Weapon()
{
	m_power_cost = 0;
	m_range = 0;
	m_attack_speed = 1.0f;
}

bool Weapon::init()
{
	return true;
}

void Weapon::attack(Point pos)
{

}

std::vector<Bullet*> Weapon::getBullet() const
{
	return std::vector<Bullet*>();
}

int Weapon::getPowerCost()const
{
	return m_power_cost;
}

int Weapon::getRange()const
{
	return m_range;
}

void Weapon::bindMap(AdventureMapLayer* map)
{
	m_map = map;
}

void Weapon::setCritRate(float crit_rate)
{
	m_crit_rate = crit_rate;
}

float Weapon::getAttackSpeed()const
{
	return m_attack_speed;
}

void Weapon::setAttackSpeed(float speed)
{
	m_attack_speed = speed;
}

void Weapon::resetPosition()
{
	getSprite()->setRotation(0.0f);
}

void Weapon::setDamage(int damage)
{

}

int Weapon::getDamage()const
{
	return 0;
}

void Weapon::setRotationByPos(Point pos)
{

}

void Weapon::flipped(bool status)
{

}

void Weapon::upgrade()
{

}

bool Weapon::isCloseWeapon()const
{
	return false;
}

Weapon::~Weapon()
{

}
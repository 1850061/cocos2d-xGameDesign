#ifndef __MonsterManager_H__
#define __MonsterManager_H__

#include "cocos2d.h"
#include "Monster\Monster.h"
#include "Entity\Entity.h"
#include "Monster\Slime.h"
#include "Monster\Pig.h"
USING_NS_CC;
class MonsterManager : public Node {
public:
	MonsterManager();
	~MonsterManager();
	void createMonsters();
	CREATE_FUNC(MonsterManager);
	void bindMap(AdventureMapLayer* map);
	void bindPlayer(Sprite* player);
	virtual bool init();
	virtual void update(float dt);
	void createMonsterPos();
	std::vector<Bullet*> getMonsterBullets();
	CC_SYNTHESIZE(AdventureMapLayer*, m_map, ManagerMap);
	CC_SYNTHESIZE(Sprite*, m_player, Player);
private:
	std::vector<Monster*> m_monsterList;
	std::vector<Bullet*> m_monsterBullet;
	
	//���ù������������ɹ������֮����ʧ������һ�����
	//�ṩ�ӵ�������
	//Player* m_player;
};
#endif
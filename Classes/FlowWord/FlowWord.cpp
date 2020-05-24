#include "FlowWord.h"

bool FlowWord::init()
{
	m_message = Label::create("", "Arial", 24);
	m_message->setColor(Color3B(255, 0, 0));
	m_message->setVisible(false);

	this->addChild(m_message);
	return true;
}

void FlowWord::showWord(const char* text, Point pos)
{
	m_message->setPosition(Point(pos.x + 30, pos.y + 30));
	m_message->setString(text);

	m_message->setVisible(true);
	auto scaleUp = ScaleBy::create(0.2f, 1.2);
	auto scaleDown = ScaleTo::create(0.2f, 1);

	auto callBack=CallFunc::create
	(
		[&]()
		{
			m_message->setVisible(false);
			m_message->removeFromParentAndCleanup(true);
		}
	);
	auto actions = Sequence::create(scaleUp, scaleDown, callBack, NULL);

	this->runAction(actions);
}
#pragma once


class Entity {
public:
	virtual ~Entity() {};
	virtual void Update(float delta) = 0;
};
#pragma once


class iEntity
{
public:

	virtual ~iEntity() {};

	virtual void Update(float dt) = 0;
};
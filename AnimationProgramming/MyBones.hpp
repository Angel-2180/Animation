#pragma once
#include "stdafx.h"

class MyBones
{
private:
	std::string     m_name;
	lm::FVec3		m_position;
	lm::FQuat       m_rotation;
	lm::FVec3		m_localPos;
	lm::FQuat		m_localRot;
	lm::FMat4		m_inverseBindPos;
	MyBones* 		m_parent;

public:

	MyBones() = default;
	MyBones(lm::FVec3 position, lm::FQuat rotation, std::string name, MyBones* parent);
	MyBones(std::string name, MyBones* parent);
	MyBones(lm::FVec3 position, lm::FQuat rotation);
	~MyBones() = default;
	// set
	void setLocalPosition(lm::FVec3 position);
	void setLocalRotation(lm::FQuat rotation);
	void setSkinningMat(lm::FVec3 position, lm::FQuat rotation);
	void setPosition(lm::FVec3 position);
	void setRotation(lm::FQuat rotation);

	std::string getName();
	lm::FVec3	getPosition();
	lm::FQuat   getRotation();
	lm::FVec3   getLocalPosition();
	lm::FQuat   getLocalRotation();
	lm::FMat4   getInverseBindPos();

	MyBones* getParent();
	void        print();
};
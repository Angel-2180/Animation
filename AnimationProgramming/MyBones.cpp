#include "MyBones.hpp"

MyBones::MyBones(lm::FVec3 position, lm::FQuat rotation, std::string name, MyBones* parent)
{
	m_position = position;
	m_rotation = rotation;
	m_name = name;
	m_parent = parent;
	m_inverseBindPos = lm::FMat4::Inverse(lm::FMat4(position, rotation));
}

MyBones::MyBones(std::string name, MyBones* parent)
{
	m_name = name;
	m_parent = parent;
}

MyBones::MyBones(lm::FVec3 position, lm::FQuat rotation)
{
	m_position = position;
	m_rotation = rotation;
}

void MyBones::setLocalPosition(lm::FVec3 position)
{
	m_localPos = position;
}

void MyBones::setLocalRotation(lm::FQuat rotation)
{
	m_localRot = rotation;
}

void MyBones::setSkinningMat(lm::FVec3 position, lm::FQuat rotation)
{
	m_inverseBindPos = lm::FMat4::Inverse(lm::FMat4(position, rotation));
}

void MyBones::setPosition(lm::FVec3 position)
{
	m_position = position;
}

void MyBones::setRotation(lm::FQuat rotation)
{
	m_rotation = rotation;
}

std::string MyBones::getName()
{
	return m_name;
}

lm::FVec3 MyBones::getPosition()
{
	return m_position;
}

lm::FQuat MyBones::getRotation()
{
	return m_rotation;
}

lm::FVec3 MyBones::getLocalPosition()
{
	return m_localPos;
}

lm::FQuat MyBones::getLocalRotation()
{
	return m_localRot;
}

lm::FMat4 MyBones::getInverseBindPos()
{
	return m_inverseBindPos;
}

MyBones* MyBones::getParent()
{
	return m_parent;
}

void MyBones::print()
{
	std::cout << m_name << std::endl;
	std::cout << "Position: " << m_position << std::endl;
	std::cout << "Rotation: " << m_rotation << std::endl;
	std::cout << "Local Position: " << m_localPos << std::endl;
	std::cout << "Local Rotation: " << m_localRot << std::endl;
	std::cout << "Skinning Matrix: " << std::endl << m_inverseBindPos << std::endl;

	if (m_parent != nullptr)
	{
		std::cout << "Parent: " << m_parent->m_name << std::endl;
	}
	else
	{
		std::cout << "Parent: " << "NULL" << std::endl;
	}
	std::cout << std::endl;
}

// Path: AnimationProgramming\MyBones.cpp
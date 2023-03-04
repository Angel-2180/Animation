#pragma once
#include "Simulation.h"
#include "stdafx.h"
#include "MyBones.hpp"

constexpr float     g_ColorMaxValue = 255.0f;

inline lm::FVec3     rgb(float r, float g, float b)
{
	return lm::FVec3(r / g_ColorMaxValue, g / g_ColorMaxValue, b / g_ColorMaxValue);
}

class Simulation : public ISimulation
{
public:

	void								Init()override;
	void								Update(float frameTime)override;

	void								drawLine(lm::FVec3 start, lm::FVec3 end, lm::FVec3 color);
	void								getBoneTransform(int boneIndex, lm::FVec3& position, lm::FQuat& rotation);
	void								getAnimLocalTransform(int boneIndex, int keyFrameIndex, lm::FVec3& position, lm::FQuat& rotation);

private:

	void								drawGizmo();
	void								readSkeleton();
	void								setupAnimation();
	void								updateKeyFrames(float frameTime);
	std::vector<MyBones>				smoothingAnimation(std::vector<MyBones>& p_currentPose,
		std::vector<MyBones>& p_nextPose, float p_Alpha);
	void								createBindBone(int index);
	void								createAnimBone(int index, int keyFrameIndex);
	void								drawBones(std::vector<MyBones> p_pose);
	void								drawMesh(int index, const lm::FVec3& p_position, const lm::FQuat& p_rotation);
	bool								isIK(std::string);

	std::vector<MyBones>				m_bones;
	std::vector<std::vector<MyBones>>	m_keyFrames;
	std::vector<lm::FMat4>				m_skinningMatrices;
	std::vector<char>					m_parentIdxs;
	int									m_keyFrameCount;
	float 								m_frameElapsed;
	float								m_timeScale;
	int									m_currentKeyFrame;
};
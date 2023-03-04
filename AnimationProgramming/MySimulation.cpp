#include "MySimulation.hpp"
#include "Engine.h"
#include "stdafx.h"

const lm::FVec3 g_distanceFromModel = { 0, -50, 0 };
constexpr const char* g_animationFile = "ThirdPersonRun.anim";

void Simulation::Update(float frameTime)
{
	updateKeyFrames(frameTime);

	drawGizmo();

	float alpha;
	alpha = m_frameElapsed - static_cast<int>(m_frameElapsed);
	int nextKeyFrame = (m_currentKeyFrame + 1) % m_keyFrameCount;

	drawBones(smoothingAnimation(m_keyFrames[m_currentKeyFrame], m_keyFrames[nextKeyFrame], alpha));

	SetSkinningPose((float*)m_skinningMatrices.data(), m_skinningMatrices.size());
	m_skinningMatrices.clear();
}

void Simulation::Init()
{
	m_skinningMatrices.reserve(m_bones.size());
	m_timeScale = 10;
	readSkeleton();
	setupAnimation();
}

void Simulation::drawGizmo()
{
	// X axis
	drawLine(lm::FVec3(0, 0, 0), lm::FVec3(100, 0, 0), lm::FVec3(1, 0, 0));

	// Y axis
	drawLine(lm::FVec3(0, 0, 0), lm::FVec3(0, 100, 0), lm::FVec3(0, 1, 0));

	// Z axis
	drawLine(lm::FVec3(0, 0, 0), lm::FVec3(0, 0, 100), lm::FVec3(0, 0, 1));
}

void Simulation::readSkeleton()
{
	int boneCount = GetSkeletonBoneCount();
	m_bones.reserve(boneCount);
	m_parentIdxs.resize(boneCount);
	for (int i = 0; i < boneCount; i++)
	{
		createBindBone(i);
	}
}

void Simulation::drawLine(lm::FVec3 start, lm::FVec3 end, lm::FVec3 color)
{
	DrawLine(
		start.x, start.y, start.z,
		end.x, end.y, end.z,
		color.x, color.y, color.z
	);
}

void Simulation::getBoneTransform(int boneIndex, lm::FVec3& position, lm::FQuat& rotation)
{
	GetSkeletonBoneLocalBindTransform(
		boneIndex,
		position.x, position.y, position.z,
		rotation.w, rotation.x, rotation.y, rotation.z
	);
}

bool Simulation::isIK(std::string name)
{
	if (name.find("ik") != std::string::npos)
	{
		return true;
	}

	return false;
}

void Simulation::getAnimLocalTransform(int boneIndex, int keyFrameIndex, lm::FVec3& position, lm::FQuat& rotation)
{
	GetAnimLocalBoneTransform(
		g_animationFile,
		boneIndex,
		keyFrameIndex,
		position.x, position.y, position.z,
		rotation.w, rotation.x, rotation.y, rotation.z
	);
}

void Simulation::setupAnimation()
{
	int boneCount = GetSkeletonBoneCount();
	m_keyFrameCount = GetAnimKeyCount(g_animationFile);
	m_keyFrames.resize(m_keyFrameCount);

	for (int i = 0; i < m_keyFrameCount; i++)
	{
		m_keyFrames[i].reserve(boneCount);
		for (int j = 0; j < boneCount; j++)
		{
			createAnimBone(j, i);
		}
	}
}

void Simulation::updateKeyFrames(float frameTime)
{
	static float elapsed = 0;
	elapsed += frameTime;

	m_frameElapsed = elapsed * m_timeScale;

	m_currentKeyFrame = (int)m_frameElapsed % m_keyFrameCount;
}

std::vector<MyBones> Simulation::smoothingAnimation(
	std::vector<MyBones>& p_currentPose,
	std::vector<MyBones>& p_nextPose,
	float p_Alpha)
{
	std::vector<MyBones> newBones;

	size_t boneCount = p_currentPose.size();

	newBones.reserve(boneCount);

	for (size_t boneIdx = 0; boneIdx < boneCount; boneIdx++)
	{
		lm::FVec3 currentPos = p_currentPose[boneIdx].getPosition();
		lm::FVec3 nextPos = p_nextPose[boneIdx].getPosition();
		lm::FQuat currentRot = p_currentPose[boneIdx].getRotation();
		lm::FQuat nextRot = p_nextPose[boneIdx].getRotation();

		lm::FVec3 newPos = lm::FVec3::Lerp(currentPos, nextPos, p_Alpha);
		lm::FQuat newRot = lm::FQuat::SLerp(currentRot, nextRot, p_Alpha);

		newBones.emplace_back(newPos, newRot);
	}
	return newBones;
}

void Simulation::createBindBone(int index)
{
	std::string name = GetSkeletonBoneName(index);
	if (!isIK(name))
	{
		int parentIndex = GetSkeletonBoneParentIndex(index);
		m_parentIdxs[index] = static_cast<char>(parentIndex);
		MyBones* parent = nullptr;
		lm::FVec3 pos;
		lm::FQuat rot;
		lm::FVec3 localPos;
		lm::FQuat localRot;
		getBoneTransform(index, localPos, localRot);
		if (parentIndex != -1)
		{
			pos = localPos;
			rot = localRot;
			parent = &m_bones[parentIndex];
			pos = parent->getPosition() + (parent->getRotation() * pos);
			rot = parent->getRotation() * rot;
		}
		m_bones.emplace_back(pos, rot, name, parent);
		m_bones[index].setLocalPosition(localPos);
		m_bones[index].setLocalRotation(localRot);
	}
}

void Simulation::createAnimBone(int index, int keyFrameIndex)
{
	std::string name = GetSkeletonBoneName(index);
	if (!isIK(name))
	{
		int parentIndex = GetSkeletonBoneParentIndex(index);
		MyBones* parent = nullptr;
		lm::FVec3 pos;
		lm::FQuat rot;

		getAnimLocalTransform(index, keyFrameIndex, pos, rot);
		if (parentIndex != -1)
		{
			parent = &m_keyFrames[keyFrameIndex][parentIndex];
			pos = pos + m_bones[index].getLocalPosition();
			pos = parent->getPosition() + (parent->getRotation() * pos);
			rot = m_bones[index].getLocalRotation() * rot;
			rot = parent->getRotation() * rot;
		}
		m_keyFrames[keyFrameIndex].emplace_back(pos, rot, name, parent);
	}
}

void Simulation::drawBones(std::vector<MyBones> p_pose)
{
	for (size_t boneIdx = 0; boneIdx < p_pose.size(); boneIdx++)
	{
		lm::FVec3 position = p_pose[boneIdx].getPosition();
		lm::FQuat rotation = p_pose[boneIdx].getRotation();
		lm::FVec3 color = rgb(1, 20, 146);

		drawMesh(boneIdx, position, rotation);
		if (m_parentIdxs[boneIdx] != -1)
		{
			lm::FVec3 parentPos = p_pose[m_parentIdxs[boneIdx]].getPosition();
			drawLine(
				position + g_distanceFromModel,
				parentPos + g_distanceFromModel,
				color);
		}
	}
}

void Simulation::drawMesh(int index, const lm::FVec3& p_position, const lm::FQuat& p_rotation)
{
	lm::FMat4 boneToAnimSpace(p_position, p_rotation);

	lm::FMat4 skinningMat = boneToAnimSpace * m_bones[index].getInverseBindPos();

	m_skinningMatrices.push_back(lm::FMat4::Transpose(skinningMat));
}
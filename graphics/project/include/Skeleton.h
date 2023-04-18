////////////////////////////////////////
// Skeleton.h
// By Matthew Lawrence
////////////////////////////////////////

#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include "core.h"
#include "Pose.h"
#include "Joint.h"

////////////////////////////////////////////////////////////////////////////////

// The Skeleton class for holding joints of a skeleton tree structure.

class Skeleton 
{
public:
	Skeleton()
	{
		root = nullptr;
	}
	~Skeleton()
	{
		delete root;
	}

	bool Load(const char* file);
	void Update();
	void Draw(const glm::mat4& viewProjMtx, GLuint shader);
	void Show();	// GUI

	glm::mat4 GetWorldMatrix(int joint);

	void SetPose(const Pose pose);
	void MoveForward(float delta = 0.25);
	void MoveBackward(float delta = 0.25);
	void MoveRight(float delta = 0.25);
	void MoveLeft(float delta = 0.25);
	void TurnAround();
	void TurnLeft();
	void TurnRight();
private:
	std::vector<Joint*> joints;	// random access

	Joint* root;	// tree structure

	void SetJointDOFs(int joint, glm::vec3 dof);
	Joint* GetJoint(int j);
	Joint* FindJointInTree(int j);
};

////////////////////////////////////////////////////////////////////////////////
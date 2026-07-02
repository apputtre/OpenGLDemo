#ifndef CAMERA_H
#define CAMERA_H

#include "tl3d/transformations.h"
#include "linalg_types.h"

#define _USE_MATH_DEFINES

#include <math.h>

class Camera
{
public:
	enum Input
	{
		None,
		// left / right movement
		Right,
		// forward / back movement
		Front,
		// up / down movement
		Up,
		Roll,
		Pitch,
		Yaw,
		Zoom
	};
	
	Camera(vec3 pos = { 0, 0, 0 }, vec3 front = { 0, 0, -1 }, vec3 up = { 0, 1, 0 })
		: pos{ pos }, front{front}, up{up}
	{}

	vec3 getPos() const;
	void setPos(const vec3& pos);
	vec3 getFront() const;
	void setFront(const vec3& front);
	vec3 getUp() const;
	void setUp(const vec3& up);
	vec3 getRight() const;
	void setRight(const vec3& right);
	mat4 getTransform();
	void update(float delta);
	void handleInput(Input inputEvent, double magnitude = 1);

private:
	// the camera's position
	vec3 pos;
	// the front vector which serves as the camera's orthonormal basis
	vec3 front;
	// the up vector which serves as the camera's orthonormal basis
	vec3 up;
	// the right vector which serves as the camera's orthonormal basis
	vec3 right;
	// the factor by which the amount of roll is multiplied when updating camera rotation
	float roll_speed = 3;
	// the factor by which the amount of pitch is multiplied when updating camera rotation
	float pitch_speed = 0.5;
	// the factor by which the amount of yaw is multiplied when updating camera rotation
	float yaw_speed = 0.5;
	// the rate at which the camera moves
	float translation_speed = 10;
	// the matrix which represents the effects of moving and panning the camera
	mat4 transform = mat4(1);
	// does the transform matrix need to be updated?
	bool transform_dirty = true;
	// the amount by which the camera needs to be moved during the next call to update()
	vec3 translation_input;
	// the amount by which the camera needs to be rotated during the next call to update()
	vec3 rotation_input;

	void updateTransform();
};

#endif
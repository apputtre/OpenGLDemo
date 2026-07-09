#include "Camera.h"

#include "tl3d/transformations.h"
#include "linalg_types.h"

vec3 Camera::getPos() const
{
	return pos;
}

void Camera::setPos(const vec3& pos)
{
	this->pos = pos;

	transform_dirty = true;
}

vec3 Camera::getFront() const
{
	return front;
}

void Camera::setFront(const vec3& front)
{
	throw std::runtime_error("Not implemented");

	transform_dirty = true;
}

vec3 Camera::getUp() const
{
	return up;
}

void Camera::setUp(const vec3& up)
{
	throw std::runtime_error("Not implemented");

	transform_dirty = true;
}

vec3 Camera::getRight() const
{
	return right;
}

void Camera::setRight(const vec3& right)
{
	throw new std::runtime_error("Not implemented");

	transform_dirty = true;
}

mat4 Camera::getTransform()
{
	if (transform_dirty)
		updateTransform();

	return transform;
}

void Camera::handleInput(Camera::Input inputEvent, double magnitude)
{
	switch (inputEvent)
	{
	case (Front):
		translation_input += magnitude * getFront();
		break;
	case (Right):
		translation_input += magnitude * getRight();
		break;
	case (Up):
		translation_input += magnitude * getUp();
		break;
	case (Roll):
		rotation_input += magnitude * roll_speed * getFront();
		break;
	case (Pitch):
		rotation_input += magnitude * pitch_speed * getRight();
		break;
	case (Yaw):
		rotation_input += magnitude * yaw_speed * getUp();
		break;
	}

	translation_input = translation_input.norm();
}

void Camera::update(float delta)
{
	setPos(pos + translation_input * translation_speed * delta / 1000);

	quat rotation = quat() + delta / 1000 / 2 * quat(0, rotation_input.x, rotation_input.y, rotation_input.z).norm();

	mat4 m(1);
	vec4 temp = (tl3d::rotate(m, rotation) * vec4(front, 0)).norm();
	front = {temp.x, temp.y, temp.z};

	m = mat4(1);
	temp = (tl3d::rotate(m, rotation) * vec4(up, 0)).norm();
	up = {temp.x, temp.y, temp.z};

	right = front.cross(up).norm();

	translation_input = 0;
	rotation_input = 0;

	transform_dirty = true;
}

void Camera::updateTransform()
{
	/*
	To transform points in world space to view space, we must undo the camera's rotation and translation.

	The camera's rotation can be represented as a matrix with columns 1, 2, and 3 being the right, up, and front basis
	vectors, respectively.

	The right, up, and front basis vectors have unit length and are orthogonal, so this matrix is an orthonormal matrix and 
	its inverse is the transpose of itself.
	*/

	// Invert the rotation by transposing the rotation matrix (note how the basis vectors form the rows and not the columns)
	transform = mat4 {
			{right.x, right.y, right.z, 0},
			{up.x, up.y, up.z, 0},
			/*
			Right (x), up (y), and front (z) form a left-handed coordinate system. The projection matrix expects coordinates
			to be right-handed. To convert to right-handed coordinates, we reverse the direction of the front basis vector.
			*/
			{-front.x, -front.y, -front.z, 0},
			{0, 0, 0, 1}
	};

	mat4 temp(1.0f);
	tl3d::trans(temp, -pos);
	
	transform = transform * temp;

	// Multiplying this transform matrix by the position of a point in world space gives that point's position in view space.

	transform_dirty = false;
}
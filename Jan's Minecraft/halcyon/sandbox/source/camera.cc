// camera.cc

#include "camera.h"

frustum::frustum() {
	for (int index = 0; index < plane::PLANE_COUNT; index++) {
		planes_[index].normal_ = glm::vec3(0.0f);
		planes_[index].D_ = 0.0f;
	}
}

// source: https://www.gamedevs.org/uploads/fast-extraction-viewing-frustum-planes-from-world-view-projection-matrix.pdf
void frustum::construct_from_view_matrix(const glm::mat4 &view) {
	// note: dirextx
#if 1
   // note: extract left plane
	planes_[plane::PLANE_LEFT].normal_ = glm::vec3(view[0][3] + view[0][0],
		view[1][3] + view[1][0],
		view[2][3] + view[2][0]);
	planes_[plane::PLANE_LEFT].D_ = view[3][3] + view[3][0];

	// note: extract right plane
	planes_[plane::PLANE_RIGHT].normal_ = glm::vec3(view[0][3] - view[0][0],
		view[1][3] - view[1][0],
		view[2][3] - view[2][0]);
	planes_[plane::PLANE_RIGHT].D_ = view[3][3] - view[3][0];

	// note: extract top plane
	planes_[plane::PLANE_TOP].normal_ = glm::vec3(view[0][3] - view[0][1],
		view[1][3] - view[1][1],
		view[2][3] - view[2][1]);
	planes_[plane::PLANE_TOP].D_ = view[3][3] - view[3][1];

	// note: extract bottom plane
	planes_[plane::PLANE_BOTTOM].normal_ = glm::vec3(view[0][3] + view[0][1],
		view[1][3] + view[1][1],
		view[2][3] + view[2][1]);
	planes_[plane::PLANE_BOTTOM].D_ = view[3][3] + view[3][1];

	// note: extract far plane
	planes_[plane::PLANE_FAR].normal_ = glm::vec3(view[0][3] - view[0][2],
		view[1][3] - view[1][2],
		view[2][3] - view[2][2]);
	planes_[plane::PLANE_FAR].D_ = view[3][3] - view[3][2];

	// note: extract near plane
	planes_[plane::PLANE_NEAR].normal_ = glm::vec3(view[0][2],
		view[1][2],
		view[2][2]);
	planes_[plane::PLANE_NEAR].D_ = view[3][2];
#else
   // note: opengl
	planes_[plane::PLANE_LEFT].normal_ = glm::vec3(view[3][0] + view[0][0],
		view[3][1] + view[0][1],
		view[3][2] + view[0][2]);

	planes_[plane::PLANE_LEFT].D_ = view[3][3] + view[0][3];

	planes_[plane::PLANE_RIGHT].normal_ = glm::vec3(view[3][0] - view[0][0],
		view[3][1] - view[0][1],
		view[3][2] - view[0][2]);
	planes_[plane::PLANE_RIGHT].D_ = view[3][3] - view[0][3];

	planes_[plane::PLANE_BOTTOM].normal_ = glm::vec3(view[3][0] + view[1][0],
		view[3][1] + view[1][1],
		view[3][2] + view[1][2]);
	planes_[plane::PLANE_BOTTOM].D_ = view[3][3] + view[1][3];

	planes_[plane::PLANE_TOP].normal_ = glm::vec3(view[3][0] - view[1][0],
		view[3][1] - view[1][1],
		view[3][2] - view[1][2]);
	planes_[plane::PLANE_TOP].D_ = view[3][3] - view[1][3];

	planes_[plane::PLANE_NEAR].normal_ = glm::vec3(view[3][0] + view[2][0],
		view[3][1] + view[2][1],
		view[3][2] + view[2][2]);
	planes_[plane::PLANE_NEAR].D_ = view[3][3] + view[2][3];

	planes_[plane::PLANE_FAR].normal_ = glm::vec3(view[3][0] - view[2][0],
		view[3][1] - view[2][1],
		view[3][2] - view[2][2]);
	planes_[plane::PLANE_FAR].D_ = view[3][3] - view[2][3];
#endif

	for (int i = 0; i < plane::PLANE_COUNT; i++) {
		float length = glm::length(planes_[i].normal_);
		planes_[i].normal_ /= length;
		planes_[i].D_ /= length;
	}
}

bool frustum::is_inside(const glm::vec3 &point) const {
	for (int index = 0; index < plane::PLANE_COUNT; index++) {
		plane::plane_type_id id = (plane::plane_type_id)index;
		float dist = glm::dot(planes_[id].normal_, point) + planes_[id].D_;
		if (dist < -8.0f)
			return false;
	}

	return true;
}

bool frustum::is_inside(const glm::vec3 &center, const float radius) const {
	for (int index = 0; index < plane::PLANE_COUNT; index++) {
		plane::plane_type_id id = (plane::plane_type_id)index;
		float dist = glm::dot(planes_[id].normal_, center) + planes_[id].D_;
		if (dist < -radius)
			return false;
	}

	return true;
}

camera::camera()
	: pitch_(0.0f)
	, yaw_(0.0f)
	, roll_(0.0f)
	, x_axis_(1.0f, 0.0f, 0.0f)
	, y_axis_(0.0f, 1.0f, 0.0f)
	, z_axis_(0.0f, 0.0f, 1.0f)
	, position_(0.0f, 0.0f, 0.0f)
	, projection_(1.0f) // note: really wierd way of saying identity matrix
	, view_(1.0f) {}

void camera::set_projection(float width, float height, float fov_radians,
	float znear, float zfar) {
	projection_ = glm::perspectiveFovLH_ZO(fov_radians, width, height, znear, zfar);
}

glm::mat4 camera::getInvertedProjection() {
	glm::mat4 invertedProjection;
	invertedProjection[0][0] = 1.0f / projection_[0][0];
	invertedProjection[1][1] = 1.0f / projection_[1][1];
	invertedProjection[2][3] = 1.0f / projection_[3][2];
	invertedProjection[3][2] = 1.0f / projection_[2][3];
	invertedProjection[3][3] = -projection_[2][2] / (projection_[3][2] * projection_[2][3]);

	return invertedProjection;
}

glm::mat4 camera::getInvertedView() {
	glm::mat4 invertedProjection;
	invertedProjection[0][0] = 1.0f / view_[0][0];
	invertedProjection[1][1] = 1.0f / view_[1][1];
	invertedProjection[2][3] = 1.0f / view_[3][2];
	invertedProjection[3][2] = 1.0f / view_[2][3];
	invertedProjection[3][3] = -view_[2][2] / (view_[3][2] * view_[2][3]);

	return invertedProjection;
}

bool camera::is_inside(const glm::vec3 &point) const {
	return frustum_.is_inside(point);
}

bool camera::is_inside(const glm::vec3 &center, const float radius) const {
	return frustum_.is_inside(center, radius);
}

void camera::move_forward(float amount) {
	position_ += z_axis_ * amount;
}

void camera::move_strafe(float amount) {
	position_ += x_axis_ * amount;
}

void camera::rotate_y(float amount) {
	yaw_ += amount;
}

void camera::rotate_x(float amount) {
	pitch_ += amount;
}

void camera::update() {
	// note: calculate axis
	glm::vec3 x(1.0f, 0.0f, 0.0f);
	glm::vec3 y(0.0f, 1.0f, 0.0f);
	glm::vec3 z(0.0f, 0.0f, 1.0f);

	glm::mat4 ry = glm::mat4(1.0f);
	ry = glm::rotate(ry, yaw_, y);

	x = ry * glm::vec4(x, 0.0f);
	z = ry * glm::vec4(z, 0.0f);

	glm::mat4 rx = glm::mat4(1.0f);
	rx = glm::rotate(rx, pitch_, x);

	y = rx * glm::vec4(y, 0.0f);
	z = rx * glm::vec4(z, 0.0f);

	// note: we ignore roll for now...

	// note: construct view matrix
	view_ = glm::mat4(1.0f);
	view_[0][0] = x.x; view_[1][0] = x.y; view_[2][0] = x.z;
	view_[0][1] = y.x; view_[1][1] = y.y; view_[2][1] = y.z;
	view_[0][2] = z.x; view_[1][2] = z.y; view_[2][2] = z.z;

	view_[3][0] = -glm::dot(position_, x);
	view_[3][1] = -glm::dot(position_, y);
	view_[3][2] = -glm::dot(position_, z);

	// note: update helpers
	x_axis_ = x;
	y_axis_ = y;
	z_axis_ = z;

	// note: update frustum
	//set_projection(1280.0f, 720.0f, glm::radians(45.0f), 1.5f, 10.0f);
	frustum_.construct_from_view_matrix(projection_ * view_);
	//set_projection(1280.0f, 720.0f, glm::radians(45.0f), 0.5f, 100.0f);
}

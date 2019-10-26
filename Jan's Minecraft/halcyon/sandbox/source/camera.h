// camera.h

#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include <halcyon.h>

struct plane {
	enum plane_type_id {
		PLANE_NEAR,
		PLANE_FAR,
		PLANE_LEFT,
		PLANE_RIGHT,
		PLANE_TOP,
		PLANE_BOTTOM,
		PLANE_COUNT,
	};

	glm::vec3 normal_;
	float D_;
};

struct frustum {
	frustum();

	void construct_from_view_matrix(const glm::mat4 &view);

	bool is_inside(const glm::vec3 &point) const;
	bool is_inside(const glm::vec3 &center, const float radius) const;

	plane planes_[plane::PLANE_COUNT];
};

struct camera {
	camera();

	void set_projection(float width, float height, float fov_radians,
		float znear, float zfar);
	glm::mat4 getInvertedProjection();
	glm::mat4 getInvertedView();
	bool is_inside(const glm::vec3 &point) const;
	bool is_inside(const glm::vec3 &center, const float radius) const;

	void move_forward(float amount);
	void move_strafe(float amount);
	void rotate_y(float amount);
	void rotate_x(float amount);

	void update();

	float pitch_;
	float yaw_;
	float roll_;

	glm::vec3 x_axis_;
	glm::vec3 y_axis_;
	glm::vec3 z_axis_;
	glm::vec3 position_;

	glm::mat4 projection_;
	glm::mat4 view_;
	frustum frustum_;
};

#endif // CAMERA_H_INCLUDED

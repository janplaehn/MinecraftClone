// sandbox.h

#include <halcyon.h>

#include "debug_renderer.h"
#include "camera.h"
#include <random>
#include <vector>
#include "skybox.h"
#include <string>
#include "perlin_noise.h"

struct model {
	v4 sphere_;
	int count_;
	vertex_buffer buffer_;
};

struct sandbox_app : application {
	sandbox_app();
	~sandbox_app();

	perlin_noise* noise;
	const int SEED = 247;

	float clickCoolDown;

	skybox skybox_;

	texture stoneTexture,
			grassTexture, 
			logTexture,
			leafTexture;

	struct cube {
		glm::mat4 transform;
		texture texture;
		glm::vec3 position;
	};

	struct chunk {
		glm::vec2 index;
		std::vector<cube> cubes;
		glm::vec3 position;
	};

	virtual bool init() final;
	virtual bool tick(double app_time, double frame_time,
		const keyboard_device *keyboard,
		const mouse_device *mouse) final;
	virtual void render() final;
	virtual void exit() final;
	//virtual void setChunk(glm::vec3 origin);
	virtual void setChunk(glm::vec2 index);
	virtual void setSlice(glm::vec3 position);
	virtual void setCube(glm::vec3 position, texture texture);
	virtual void setTree(glm::vec3 position);
	virtual void drawCube(cube cube);
	virtual void drawChunk(chunk chunk);
	virtual void sendRay(glm::vec3 ray);
	int random(int min, int max);

	const int	CHUNKSIZE = 16,
				RENDERDISTANCE = 3,
				LANDSCAPEWIDTH = 10;

	const glm::vec3 ORIGIN{ 0, 2, 0 };

	std::vector<cube> allCubes;
	std::vector<chunk> allChunks;

	render_system render_sys_;
	debug_renderer debug_renderer_;

	v2 prev_mouse_position_;
	camera cam_;
	glm::mat4 world_;


	texture unit_cube_texture_;
	model unit_cube_model_;

	shader_program program_;
	sampler_state linear_sampler_;
	vertex_format vertex_format_;
};

// sandbox.cc

#include "sandbox.h"
#include "image.h"
#include <stdarg.h>
#include <stdio.h>

struct vertex {
	v3 position_;
	v2 texcoord_;
};

sandbox_app::sandbox_app()
	: debug_renderer_(render_sys_), skybox_(render_sys_) {}

sandbox_app::~sandbox_app() {}

bool sandbox_app::init() {
	cam_.set_projection(1600.0f, 900.0f, glm::radians(45.0f), 0.5f, 100.0f);
	cam_.position_ = { 0.0f, 9.0f, 0.0f };

	clickCoolDown = 0;

	if (!skybox_.init()) {
		HC_ASSERT(false);
	}

	noise = new perlin_noise();

	// note: cube textures
	{
		image cube_image;
		if (!cube_image.load_from_file("assets/grass.jpg"))
			HC_ASSERT(false);
		if (!render_sys_.create_texture(grassTexture,
			RENDER_SYSTEM_TEXTURE_FORMAT_RGBA8,
			cube_image.width_,
			cube_image.height_,
			cube_image.data_)) {
			HC_ASSERT(false);
		}
		cube_image.release();
	}

	{
		image cube_image;
		if (!cube_image.load_from_file("assets/stone.png"))
			HC_ASSERT(false);
		if (!render_sys_.create_texture(stoneTexture,
			RENDER_SYSTEM_TEXTURE_FORMAT_RGBA8,
			cube_image.width_,
			cube_image.height_,
			cube_image.data_)) {
			HC_ASSERT(false);
		}
		cube_image.release();
	}

	{
		image cube_image;
		if (!cube_image.load_from_file("assets/log.png"))
			HC_ASSERT(false);
		if (!render_sys_.create_texture(logTexture,
			RENDER_SYSTEM_TEXTURE_FORMAT_RGBA8,
			cube_image.width_,
			cube_image.height_,
			cube_image.data_)) {
			HC_ASSERT(false);
		}
		cube_image.release();
	}

	{
		image cube_image;
		if (!cube_image.load_from_file("assets/leaves.png"))
			HC_ASSERT(false);
		if (!render_sys_.create_texture(leafTexture,
			RENDER_SYSTEM_TEXTURE_FORMAT_RGBA8,
			cube_image.width_,
			cube_image.height_,
			cube_image.data_)) {
			HC_ASSERT(false);
		}
		cube_image.release();
	}


	// note: unit cube
	{
		unit_cube_model_.sphere_.xyz_ = { 0.0f, 0.0f, 3.0f, };
		unit_cube_model_.sphere_.w_ = 0.5f;

		const float Q = 0.5f;
		vertex vertices[] =
		{
			// front
			{ { -Q,  Q, -Q, }, { 0.5f, 0.0f } },
			{ {  Q,  Q, -Q, }, { 1.0f, 0.0f } },
			{ {  Q, -Q, -Q, }, { 1.0f, 1.0f } },
			{ {  Q, -Q, -Q, }, { 1.0f, 1.0f } },
			{ { -Q, -Q, -Q, }, { 0.5f, 1.0f } },
			{ { -Q,  Q, -Q, }, { 0.5f, 0.0f } },

			// right
			{ {  Q,  Q, -Q, }, { 0.5f, 0.0f } },
			{ {  Q,  Q,  Q, }, { 1.0f, 0.0f } },
			{ {  Q, -Q,  Q, }, { 1.0f, 1.0f } },
			{ {  Q, -Q,  Q, }, { 1.0f, 1.0f } },
			{ {  Q, -Q, -Q, }, { 0.5f, 1.0f } },
			{ {  Q,  Q, -Q, }, { 0.5f, 0.0f } },

			// back
			{ {  Q,  Q,  Q, }, { 0.5f, 0.0f } },
			{ { -Q,  Q,  Q, }, { 1.0f, 0.0f } },
			{ { -Q, -Q,  Q, }, { 1.0f, 1.0f } },
			{ { -Q, -Q,  Q, }, { 1.0f, 1.0f } },
			{ {  Q, -Q,  Q, }, { 0.5f, 1.0f } },
			{ {  Q,  Q,  Q, }, { 0.5f, 0.0f } },

			// left
			{ { -Q,  Q,  Q, }, { 0.5f, 0.0f } },
			{ { -Q,  Q, -Q, }, { 1.0f, 0.0f } },
			{ { -Q, -Q, -Q, }, { 1.0f, 1.0f } },
			{ { -Q, -Q, -Q, }, { 1.0f, 1.0f } },
			{ { -Q, -Q,  Q, }, { 0.5f, 1.0f } },
			{ { -Q,  Q,  Q, }, { 0.5f, 0.0f } },

			// top
			{ { -Q,  Q,  Q, }, { 0.0f, 0.0f } },
			{ {  Q,  Q,  Q, }, { 0.5f, 0.0f } },
			{ {  Q,  Q, -Q, }, { 0.5f, 1.0f } },
			{ {  Q,  Q, -Q, }, { 0.5f, 1.0f } },
			{ { -Q,  Q, -Q, }, { 0.0f, 1.0f } },
			{ { -Q,  Q,  Q, }, { 0.0f, 0.0f } },

			// bottom
			{ { -Q, -Q, -Q, }, { 0.0f, 0.0f } },
			{ {  Q, -Q, -Q, }, { 0.5f, 0.0f } },
			{ {  Q, -Q,  Q, }, { 0.5f, 0.5f } },
			{ {  Q, -Q,  Q, }, { 0.5f, 0.5f } },
			{ { -Q, -Q,  Q, }, { 0.0f, 0.5f } },
			{ { -Q, -Q, -Q, }, { 0.0f, 0.0f } },
		};

		unit_cube_model_.count_ = sizeof(vertices) / sizeof(vertices[0]);
		if (!render_sys_.create_vertex_buffer(unit_cube_model_.buffer_,
			RENDER_SYSTEM_BUFFER_ACCESS_STATIC,
			sizeof(vertices),
			vertices)) {
			HC_ASSERT(false);
		}
	}

	{
		// note: load vertex shader
		file_content vertex_shader_file_content = {};
		if (!read_file_from_disk("assets/vertex_shader.txt", &vertex_shader_file_content)) { HC_ASSERT(false); }

		// note: load fragment shader
		file_content fragment_shader_file_content = {};
		if (!read_file_from_disk("assets/fragment_shader.txt", &fragment_shader_file_content)) { HC_ASSERT(false); }

		// note: create shader program
		if (!render_sys_.create_shader(program_,
			(const char *)vertex_shader_file_content.data_,
			(const char *)fragment_shader_file_content.data_)) {
			HC_ASSERT(false);
		}

		// note: release shader sources
		release_file_content(&vertex_shader_file_content);
		release_file_content(&fragment_shader_file_content);
		
	}

	//setting all Chunks;
	for (int i = 0; i < LANDSCAPEWIDTH; i++) {
		for (int j = 0; j < LANDSCAPEWIDTH; j++) {
			setChunk({ i - LANDSCAPEWIDTH/2, j - LANDSCAPEWIDTH / 2 });
		}
	}

	// note: create sampler stated
	{
		if (!render_sys_.create_sampler_state(linear_sampler_,
			RENDER_SYSTEM_SAMPLER_FILTER_NEAREST,				//"NEAREST" for nearest neightbor upscaling
			RENDER_SYSTEM_SAMPLER_ADDRESS_MODE_CLAMP,
			RENDER_SYSTEM_SAMPLER_ADDRESS_MODE_CLAMP)) {
			HC_ASSERT(false);
		}
	}

	// note: build vertex format
	vertex_format_.add_attribute(vertex_format::ATTRIBUTE_SEMANTIC_POSITION,
		vertex_format::ATTRIBUTE_FORMAT_FLOAT,
		3,
		false);
	vertex_format_.add_attribute(vertex_format::ATTRIBUTE_SEMANTIC_TEXCOORD,
		vertex_format::ATTRIBUTE_FORMAT_FLOAT,
		2,
		false);


	if (!debug_renderer_.init(1600, 900))
		return false;

	return true;
}

bool sandbox_app::tick(double app_time, double frame_time, const keyboard_device *keyboard, const mouse_device *mouse) {
	static bool is_first_frame = false;
	const float deltatime = (float)frame_time;
	// note: camera and mouse settings
	const float camera_speed = 5.0f;
	const float mouse_sensitivity = 2.0f;
	const float mouse_yaw_factor = -0.022f;
	const float mouse_pitch_factor = 0.022f;
	clickCoolDown += deltatime;
	// todo: camera controller
	if (mouse->buttons_[MOUSE_BUTTON_RIGHT].down_) {
		v2 mouse_position = { (float)mouse->x_, (float)mouse->y_ };
		v2 mouse_delta =
		{
			mouse_position.x_ - prev_mouse_position_.x_,
			mouse_position.y_ - prev_mouse_position_.y_,
		};
		prev_mouse_position_ = mouse_position;
		if (is_first_frame) {
			is_first_frame = false;
		}
		else {
			// note: 1.0f = 1.0000000001
			if (mouse_delta.x_ != 0.0f) {
				cam_.rotate_y(mouse_delta.x_ * mouse_yaw_factor * mouse_sensitivity * deltatime *-1);
			}
			if (mouse_delta.y_ != 0.0f) {
				cam_.rotate_x(mouse_delta.y_ * mouse_pitch_factor * mouse_sensitivity * deltatime);
			}
		}
	}
	else {
		prev_mouse_position_ = { (float)mouse->x_, (float)mouse->y_ };
	}

	//CREATING A RAY FOR DESTROYING A BLOCK
	if (mouse->buttons_[MOUSE_BUTTON_LEFT].pressed_ && clickCoolDown > 0.1) {
		clickCoolDown = 0;
		float x = (2.0f * (float)mouse->x_) / 1600.0f - 1.0f;
		float y = (2.0f * (float)mouse->y_) / 900.0f - 1.0f;
		float z = 1.0f;
		x *= -1;
		glm::vec3 ray_nds = glm::vec3(x, y, z);
		glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0, 1.0);
		glm::vec4 ray_eye = glm::inverse(cam_.projection_) * ray_clip;
		ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
		glm::vec3 ray_wor = glm::vec3((glm::inverse(cam_.view_) * ray_eye).x, (glm::inverse(cam_.view_) * ray_eye).y, (glm::inverse(cam_.view_) * ray_eye).z);
		ray_wor = glm::normalize(ray_wor);
		sendRay(ray_wor);
	}


	if (keyboard->keys_[KEYCODE_LEFT].down_ || keyboard->keys_[KEYCODE_KEY_A].down_) {
		cam_.move_strafe(deltatime * -camera_speed);
	}
	if (keyboard->keys_[KEYCODE_RIGHT].down_ || keyboard->keys_[KEYCODE_KEY_D].down_) {
		cam_.move_strafe(deltatime * camera_speed);
	}
	if (keyboard->keys_[KEYCODE_DOWN].down_ || keyboard->keys_[KEYCODE_KEY_S].down_) {
		cam_.move_forward(deltatime * -camera_speed);
	}
	if (keyboard->keys_[KEYCODE_UP].down_ || keyboard->keys_[KEYCODE_KEY_W].down_) {
		cam_.move_forward(deltatime * camera_speed);
	}
	cam_.update();

	// note: cube model transform
	world_ = glm::mat4(1.0f);
	world_ = glm::translate(world_, glm::vec3(0.0f, 0.0f, 3.0f));
	world_ = glm::rotate(world_, (float)app_time,
		glm::normalize(
			glm::vec3(sinf((float)app_time), cosf((float)app_time), 0.0f)
		));

	debug_renderer_.draw_text_format({ 2.0f, 2.0f }, "%dx%d", mouse->x_, mouse->y_);
	debug_renderer_.draw_text_format({ 2.0f, 12.0f },
		"x: %f y: %f z: %f",
		cam_.position_.x,
		cam_.position_.y,
		cam_.position_.z);
	debug_renderer_.draw_text_format({ 2.0f, 20.0f },
		"pitch: %f",
		cam_.pitch_);
	debug_renderer_.draw_text_format({ 2.0f, 28.0f },
		"yaw:   %f",
		cam_.yaw_);
	debug_renderer_.draw_text_format({ 2.0f, 36.0f },
		"roll:  %f",
		cam_.roll_);

	return !keyboard->keys_[KEYCODE_ESCAPE].pressed_;
}

void sandbox_app::render() {
	render_sys_.clear({ 0.51f, 0.67f, 1.0f, 1.0f });
	render_sys_.set_viewport(0, 0, 1600, 900);

	skybox_.render(cam_);

	// note: pipeline settings
	render_sys_.set_shader_program(program_);
	render_sys_.set_blend_state(true);
	render_sys_.set_depth_state(true, true);
	render_sys_.set_rasterizer_state(RENDER_SYSTEM_CULL_MODE_BACK);

	// note: shader uniforms
	render_sys_.set_shader_uniform(program_,
		RENDER_SYSTEM_UNIFORM_TYPE_MATRIX,
		"u_projection",
		1,
		glm::value_ptr(cam_.projection_));

	render_sys_.set_shader_uniform(program_,
		RENDER_SYSTEM_UNIFORM_TYPE_MATRIX,
		"u_view",
		1,
		glm::value_ptr(cam_.view_));


	//DRAWING ALL CHUNKS
	for (int i = 0; i < allChunks.size(); i++) {
		drawChunk(allChunks[i]);
	}

	// note: render all debug text
	debug_renderer_.render();

}

void sandbox_app::exit() {
	delete noise;

	debug_renderer_.exit();
	skybox_.exit();

	// note: clean up of unit cube
	render_sys_.destroy_shader(program_);
	render_sys_.destroy_texture(unit_cube_texture_);
	render_sys_.destroy_sampler_state(linear_sampler_);
	render_sys_.destroy_vertex_buffer(unit_cube_model_.buffer_);
	 
	debug_renderer_.exit();
}

void sandbox_app::setChunk(glm::vec2 index) {
	chunk chunk;
	chunk.index = index;
	for (int i = 0; i < CHUNKSIZE; i++) {
		for (int j = 0; j < CHUNKSIZE; j++) {
			int height = (int)(10 * noise->noise((ORIGIN.x + index.x * CHUNKSIZE + i) * 0.1, (ORIGIN.z + index.y * CHUNKSIZE + j) * 0.1));
			setSlice(glm::vec3(ORIGIN.x + index.x * CHUNKSIZE + i, (int)height, ORIGIN.z + index.y * CHUNKSIZE + j));
		}
	}
	chunk.position = glm::vec3(ORIGIN.x + index.x * CHUNKSIZE + CHUNKSIZE/2, ORIGIN.y + CHUNKSIZE / 2, ORIGIN.z + index.y * CHUNKSIZE + CHUNKSIZE / 2);
	for (int i = 0; i < allCubes.size(); i++) {
		chunk.cubes.push_back(allCubes[i]);
	}
	allCubes.clear();
	allChunks.push_back(chunk);
}

void sandbox_app::setSlice(glm::vec3 position) {    // A SLICE IS A COLUMN OF CUBES IN THE GAME WORLD (ALL CUBES WITH THE SAME x AND z POSITION
	for (int i = (int)position.y; i >= ORIGIN.y; i--) {
		if (i == (int)position.y) {

			int r = random(0, 1000);
			if (r > 995) {
				setTree(position);
			}

			setCube(glm::vec3(position.x, i, position.z), grassTexture);
		}
		else {
			setCube(glm::vec3(position.x, i, position.z), stoneTexture);
		}
	}
}

void sandbox_app::setCube(glm::vec3 position, texture texture) {
	cube cube;
	cube.position = position;
	cube.transform = glm::mat4(1.0f);
	cube.transform = glm::translate(cube.transform, position);
	cube.texture = texture;
	allCubes.push_back(cube);
}

void sandbox_app::setTree(glm::vec3 position) {
	for (int height = 1; height <= 5; height++) {
		setCube({ position.x, position.y + height, position.z }, logTexture);
	}
	for (int width = 0; width < 5; width++) {
		for (int depth = 0; depth < 5; depth++) {
			for (int height = 0; height < 3; height++) {
				setCube({ position.x - 2 + width, position.y + 4 + height, position.z - 2 + depth }, leafTexture);
			}
		}
	}
}

void sandbox_app::drawCube(cube cube) {
			// note: frustum culling
		render_sys_.set_shader_uniform(program_,
			RENDER_SYSTEM_UNIFORM_TYPE_MATRIX,
			"u_world",
			1,
			glm::value_ptr(cube.transform));

		// note: input assembly
		render_sys_.set_vertex_buffer(unit_cube_model_.buffer_);
		render_sys_.set_vertex_format(vertex_format_);

		// note: resource bindings
		render_sys_.set_texture(cube.texture, 0);
		render_sys_.set_sampler_state(linear_sampler_, 0);

		// note: draw call
		render_sys_.draw(RENDER_SYSTEM_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
			0,
			36);
}

void sandbox_app::drawChunk(chunk chunk) {
	for (int i = 0; i < chunk.cubes.size(); i++) {
		float chunkX;
		float chunkZ;
		chunkX = cam_.position_.x / 16;
		chunkZ = cam_.position_.z / 16;
		if (chunkX < 0) chunkX -= 1;
		if (chunkZ < 0) chunkZ -= 1;
		int xDistance = (int)abs((int)chunkX - chunk.index.x);
		int zDistance = (int)abs((int)chunkZ - chunk.index.y);

		if (xDistance <= RENDERDISTANCE && zDistance <= RENDERDISTANCE && cam_.is_inside(chunk.position)) {
					drawCube(chunk.cubes[i]);
		}
	}
}

void sandbox_app::sendRay(glm::vec3 ray) {
	bool hasDeleted = false;
	for (size_t i = 0; i < 15; i ++) {
		glm::vec3 point = cam_.position_ - glm::vec3(ray.x *i, ray.y *i, ray.z *i);
		for (int j = 0; j < allChunks.size(); j++) {
			float chunkX;
			float chunkZ;
			chunkX = cam_.position_.x / 16;
			chunkZ = cam_.position_.z / 16;
			if (chunkX < 0) chunkX -= 1;
			if (chunkZ < 0) chunkZ -= 1;
			int xDistance = (int)abs((int)chunkX - allChunks[j].index.x);
			int zDistance = (int)abs((int)chunkZ - allChunks[j].index.y);
			if (xDistance <= 0 && zDistance <= 0 && cam_.is_inside(allChunks[j].position)) {
				for (int k = 0; k < allChunks[j].cubes.size(); k++) {
					if (point.x > allChunks[j].cubes[k].position.x - 0.5 &&
						point.y > allChunks[j].cubes[k].position.y - 0.5 &&
						point.z > allChunks[j].cubes[k].position.z - 0.5) {
						if (point.x < allChunks[j].cubes[k].position.x + 0.5 && 
							point.y < allChunks[j].cubes[k].position.y + 0.5 && 
							point.z < allChunks[j].cubes[k].position.z + 0.5) {
							allChunks[j].cubes.erase(allChunks[j].cubes.begin() + k);
							hasDeleted = true;
							break;
						}
					}
				}
			}
			if (hasDeleted) break;
		}
		if (hasDeleted) break;
	}
}

int sandbox_app::random(int min, int max) {
	static std::random_device rd; //obtain a random seed from hardware
	static std::mt19937 eng(rd()); //seed the generator
	std::uniform_int_distribution<> distr(min, max); //define the range
	/*std::uniform_real_distribution<> //use if you want floats!*/
	return distr(eng);
}

// note: we need to implement this function to enable library winmain 
//       to create an instance of our application
application *create_application(int /*argc*/, char ** /*argv*/) {
	return new sandbox_app;
}
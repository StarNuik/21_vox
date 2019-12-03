#include "Player/Player.h"
#include "Engine/Locator.hpp"
#include <glm/gtx/euler_angles.hpp>
// #include <glm/gtx/quaternion.hpp>

Player::Player(Game* game) {
	_game = game;
	_position = glm::vec3(0.f, 0.f, 3.f);
	_rotation = glm::identity<glm::quat>();
	_camera = new Camera(_game->GetRenderer(), 90.f, 1.f, 100.f);
	_game->GetRenderer()->SetActiveCamera(_camera);

	ResourceLoader* r = _game->GetResources();
	RenderModel* model = new RenderModel(_game->GetRenderer(), r->GetShader("Base"), r->GetTexture("Stone"), r->GetGeometry("Box"));
}

Player::~Player() {
	_game->RemoveEntity(this);
	_game->GetRenderer()->SetActiveCamera(nullptr);
	delete _camera;
}

void Player::Update() {
	Input* input = _game->GetInput();

	glm::ivec2 mousePos = input->MousePos();
	_rotation = glm::quat(-glm::vec3(glm::radians((float)mousePos.y), glm::radians((float)mousePos.x), 0.f));
	glm::vec3 forward = glm::mat4_cast(_rotation) * glm::vec4(0.f, 0.f, -1.f, 0.f);
	glm::vec3 up = glm::mat4_cast(_rotation) * glm::vec4(0.f, 1.f, 0.f, 0.f);
	glm::vec3 right = glm::mat4_cast(_rotation) * glm::vec4(1.f, 0.f, 0.f, 0.f);
	if (input->KeyPressed(GLFW_KEY_W)) {
		_position += forward * FIXED_DELTA;
	}
	if (input->KeyPressed(GLFW_KEY_S)) {
		_position -= forward * FIXED_DELTA;
	}
	if (input->KeyPressed(GLFW_KEY_SPACE)) {
		_position += up * FIXED_DELTA;
	}
	if (input->KeyPressed(GLFW_KEY_LEFT_SHIFT)) {
		_position -= up * FIXED_DELTA;
	}
	if (input->KeyPressed(GLFW_KEY_D)) {
		_position += right * FIXED_DELTA;
	}
	if (input->KeyPressed(GLFW_KEY_A)) {
		_position -= right * FIXED_DELTA;
	}
	_camera->SetRotation(_rotation);
	_camera->SetPosition(_position);
}
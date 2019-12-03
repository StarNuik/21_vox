#include "Player/Player.h"
#include "Engine/Locator.hpp"

Player::Player(Game* game) {
	_game = game;
	_position = glm::vec3(0.f, 0.f, 3.f);
	_rotation = glm::identity<glm::quat>();
	_camera = new Camera(_game->GetRenderer(), 90.f, 1.f, 100.f);
	_game->GetRenderer()->SetActiveCamera(_camera);

	ResourceLoader* r = _game->GetResources();
	RenderModel* model = new RenderModel(r->GetShader("Base"), r->GetTexture("Stone"), r->GetGeometry("Box"));
	_game->GetRenderer()->AddModel(model);
}

Player::~Player() {
	_game->RemoveEntity(this);
	_game->GetRenderer()->SetActiveCamera(nullptr);
	delete _camera;
}

void Player::Update() {
	Input* input = _game->GetInput();

	glm::vec3 dir_vector(0.f);
	if (input->KeyPressed(GLFW_KEY_SPACE)) {
		dir_vector += glm::vec3(0.f, 1.f, 0.f);
	}
	if (input->KeyPressed(GLFW_KEY_LEFT_SHIFT)) {
		dir_vector += glm::vec3(0.f, -1.f , 0.f);
	}
	if (input->KeyPressed(GLFW_KEY_W)) {
		dir_vector += glm::vec3(0.f, 0.f, -1.f);
	}
	if (input->KeyPressed(GLFW_KEY_S)) {
		dir_vector += glm::vec3(0.f, 0.f , 1.f);
	}
	if (input->KeyPressed(GLFW_KEY_A)) {
		dir_vector += glm::vec3(-1.f, 0.f, 0.f);
	}
	if (input->KeyPressed(GLFW_KEY_D)) {
		dir_vector += glm::vec3(1.f, 0.f , 0.f);
	}
	_position = _position + dir_vector * MS_PER_UPDATE;
	_camera->SetPosition(_position);
}
#include <thread>

#include "Player/Player.h"
#include "Utilities/Log.h"
#include "Engine/Game.h"
#include "Render/GLRenderer.h"
#include "Render/Camera.h"
#include "Input/Input.h"
#include "UI/UIController.h"
#include "World/World.h"
// #include <glm/gtx/euler_angles.hpp>
// #include <glm/gtx/quaternion.hpp>

Player::Player(Game* game) {
	_camAngleX = 0.f;
	_camAngleY = 0.f;
	_game = game;
	_rotateCamera = true;
	//! Looks like a fucking costil
	_game->GetUI()->SetPlayer(this);
	_position = glm::vec3(0.f, 120.f, 3.f);
	_rotation = glm::identity<glm::quat>();
	_camera = new Camera(_game->GetRenderer(), 90.f, 0.1f, 300.f);
	_game->GetRenderer()->SetActiveCamera(_camera);
}

Player::~Player() {
	_game->RemoveEntity(this);
	_game->GetRenderer()->SetActiveCamera(nullptr);
	delete _camera;
}

void Player::Update(float delta) {
	Input* input = _game->GetInput();

	if (input->KeyJustPressed(GLFW_KEY_E)) {
		bool state = !_game->GetUI()->GetState();
		_game->GetUI()->SetState(state);
		_game->GetRenderer()->SetCursor(state);
		_rotateCamera = !state;
	}
	if (_rotateCamera) {
		glm::ivec2 mousePos = input->MousePosDelta();
		_camAngleX += mousePos.x;
		_camAngleY += mousePos.y;
		_camAngleY = glm::clamp(_camAngleY, -89.5f, 89.5f);
	}
	_rotation = glm::quat(-glm::vec3(glm::radians(_camAngleY), glm::radians(_camAngleX), 0.f));
	glm::vec3 forward = glm::mat4_cast(_rotation) * glm::vec4(0.f, 0.f, -1.f, 0.f) * SPEED;
	glm::vec3 up = glm::mat4_cast(_rotation) * glm::vec4(0.f, 1.f, 0.f, 0.f) * SPEED;
	glm::vec3 right = glm::mat4_cast(_rotation) * glm::vec4(1.f, 0.f, 0.f, 0.f) * SPEED;
	if (input->KeyPressed(GLFW_KEY_W)) {
		_position += forward * delta;
	}
	if (input->KeyPressed(GLFW_KEY_S)) {
		_position -= forward * delta;
	}
	if (input->KeyPressed(GLFW_KEY_SPACE)) {
		_position += up * delta;
	}
	if (input->KeyPressed(GLFW_KEY_LEFT_SHIFT)) {
		_position -= up * delta;
	}
	if (input->KeyPressed(GLFW_KEY_D)) {
		_position += right * delta;
	}
	if (input->KeyPressed(GLFW_KEY_A)) {
		_position -= right * delta;
	}
	if (_rotateCamera) {
		_camera->SetRotation(_rotation);
	}
	_camera->SetPosition(_position);
	// std::thread t(&Player::DoChunks, this);
	// t.detach();
	//! KILL ME WITH FIRE
	DoChunks();
}

//! KILL ME WITH FIRE
void Player::DoChunks() {
	World* w = _game->GetWorld();
	int x = _position.x;
	int z = _position.z;
	x = x >= 0 ? x >> 4 : (x + 1) / 16 - 1;
	z = z >= 0 ? z >> 4 : (z + 1) / 16 - 1;
	const glm::ivec2 chunkPos(x, z);
	for (x = -12; x <= 12; x++)
		for (z = -12; z <= 12; z++) {
			int r = x * x + z * z;
			w->SetChunkState(chunkPos + glm::ivec2(x, z), r <= 100);
		}
	// Log::Basic("Active chunks: " + std::to_string(activeChunks));
};
//! KILL ME WITH FIRE

glm::vec3 Player::GetPosition() {return _position;};
glm::vec3 Player::GetDirection() {return glm::mat4_cast(_rotation) * glm::vec4(0.f, 0.f, -1.f, 0.f);};
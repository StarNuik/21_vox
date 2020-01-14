#include "Player/Player.h"
#include "Utilities/Log.h"
#include "Engine/Game.h"
#include "Render/GLRenderer.h"
#include "Render/Camera.h"
#include "Input/Input.h"
#include "UI/UIController.h"
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
	_world = _game->GetWorld();
	MovementProperty _movementPropety;
}

Player::~Player() {
	_game->RemoveEntity(this);
	_game->GetRenderer()->SetActiveCamera(nullptr);
	delete _camera;
}

float Player::RayCastDist(const glm::vec3 _position, glm::vec3 direction, const float rayLength, float rayStep)
{
	BlockType block;
	glm::vec3 tmpDirection;


	if (rayStep >= rayLength)
		rayStep *= 0.25f;

	direction = glm::normalize(direction);

	for (float step = 0.f; step <= rayLength; step += rayStep)
	{
		tmpDirection = glm::vec3(direction.x * step, direction.y * step, direction.z * step);
		block = _world->GetBlock(glm::ivec3(_position + tmpDirection));
		if (block != BlockType::Air)
			return step;
	}
	return INFINITY;
}

void Player::PlayerCollision(glm::vec3& _position, glm::vec3& forward, glm::vec3& right)
{
	const float halfObjectHeight = _movementPropety.objectHeight * 0.5f;

	float upDist = RayCastDist(_position, _movementPropety.vecUp, halfObjectHeight, 0.02f);
	float buttomDist = RayCastDist(_position, -_movementPropety.vecUp, _movementPropety.objectHeight + 1.f, 0.1f);

	forward.y = 0.f;
	right.y = 0.f;

	if (buttomDist > _movementPropety.objectHeight)
		_position += -_movementPropety.vecUp;
	else if (buttomDist < _movementPropety.objectHeight && upDist >= halfObjectHeight)
		_position.y = (_position.y - buttomDist) + _movementPropety.objectHeight;

	glm::vec3 upperBody = _position;

	float upperForwardDist = RayCastDist(upperBody, forward, 5.f, 0.1f);
	float upperRightDist = RayCastDist(upperBody, right, 0.7f, 0.1f);
	float upperLeftDist = RayCastDist(upperBody, -right, 0.7f, 0.1f);
	float upperBackforwardDist = RayCastDist(upperBody, -forward, 0.7f, 0.1f);

	// std::cout << "upperBody[xyz]: " << upperBody.x << " " << upperBody.y << " " << upperBody.z << std::endl;
	// std::cout<<"upperForward: "<< upperForwardDist << " upperRight: " << upperRightDist << " upperLeft: " << upperLeftDist << " upperBackforward: " << upperBackforwardDist << std::endl;

	if (upperForwardDist < _movementPropety.avoidBlockDistance || upperBackforwardDist < _movementPropety.avoidBlockDistance
	|| upperRightDist < _movementPropety.avoidBlockDistance || upperLeftDist < _movementPropety.avoidBlockDistance)
		_position = _movementPropety.oldObjectPos;

	return;
}

void Player::Update(float delta) {
	Input* input = _game->GetInput();
	glm::vec3 forward;
	glm::vec3 up;
	glm::vec3 right;

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

	if (input->KeyJustPressed(GLFW_KEY_G)) {
		_movementPropety.godMode = (_movementPropety.godMode + 1) % 2;
	}
	_movementPropety.oldObjectPos = _position;

	_rotation = glm::quat(-glm::vec3(glm::radians(_camAngleY), glm::radians(_camAngleX), 0.f));
	forward = glm::mat4_cast(_rotation) * glm::vec4(0.f, 0.f, -1.f, 0.f) * SPEED;
	up = glm::mat4_cast(_rotation) * glm::vec4(0.f, 1.f, 0.f, 0.f) * SPEED;
	right = glm::mat4_cast(_rotation) * glm::vec4(1.f, 0.f, 0.f, 0.f) * SPEED;


	if (!_movementPropety.godMode) {
		PlayerCollision(_position, forward, right);
	}

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
}

glm::vec3 Player::GetPosition() {return _position;};
glm::vec3 Player::GetDirection() {return glm::mat4_cast(_rotation) * glm::vec4(0.f, 0.f, -1.f, 0.f);};
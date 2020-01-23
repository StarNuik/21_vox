#include "Player/Player.h"
#include "Utilities/Log.h"
#include "Engine/Game.h"
#include "Render/GLRenderer.h"
#include "Render/Camera.h"
#include "Input/Input.h"
#include "Render/RenderModel.h"
#include "World/ResourceLoader.h"
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
	ResourceLoader* rs = _game->GetResources();
	_monkey = new RenderModel(_game->GetRenderer(), rs->GetShader("Base"), rs->GetMaterial(BlockType::Cobblestone), rs->GetGeometry("BoxC"));
}

Player::~Player() {
	_game->RemoveEntity(this);
	_game->GetRenderer()->SetActiveCamera(nullptr);
	delete _camera;
}

float Player::RayCastDist(const glm::vec3 _position, glm::vec3 direction, const float rayLength, float rayStep)
{
	Block block;
	glm::vec3 tmpDirection;


	if (rayStep >= rayLength)
		rayStep = rayLength * 0.25f;

	direction = glm::normalize(direction);

	for (float step = 0.f; step <= rayLength; step += rayStep)
	{
		tmpDirection = glm::vec3(direction.x * step, direction.y * step, direction.z * step);
		block = _world->GetBlock(_position + tmpDirection);
		if (block != Block::Air)
			return step;
	}
	return INFINITY;
}

Player::RayCastHitInfo Player::RayCast(const glm::vec3 _position, glm::vec3 direction, const float rayLength, float rayStep)
{

	RayCastHitInfo ray;
	glm::vec3 tmpDirection;

	if (rayStep >= rayLength)
		rayStep = rayLength * 0.25f;

	direction = glm::normalize(direction);
	ray.hitRayPos = glm::vec3(0.f, 0.f, 0.f);
	for (float step = 0.f; step <= rayLength; step += rayStep)
	{
		ray.lastRayStep = ray.hitRayPos;
		tmpDirection = glm::vec3(direction.x * step, direction.y * step, direction.z * step);
		ray.hitRayPos = glm::vec3(_position + tmpDirection);
		ray.hitBlock = _world->GetBlock(ray.hitRayPos);
		if (ray.hitBlock != Block::Air)
		{
			ray.hit = true;
			ray.distance = step;
			return ray;
		}
	}
	ray.hitRayPos = glm::vec3(INFINITY, INFINITY, INFINITY);
	ray.lastRayStep = glm::vec3(INFINITY, INFINITY, INFINITY);
	ray.hit = false;
	return ray;
}

void Player::PutBlock(glm::vec3& _position, glm::vec3& forward, BlockType blockType)
{
	const float maxBlockDist = 4.f;
	Block lastBlock;
	RayCastHitInfo ray;

	ray = RayCast(_position, forward, maxBlockDist, 0.25f);
	lastBlock = _world->GetBlock(ray.lastRayStep);
	// std::cout << "currBlockHitPos[xyz]: " << ray.hitRayPos.x << " " << ray.hitRayPos.y << " " << ray.hitRayPos.z << std::endl;
	// std::cout << "lastBlockHitPos[xyz]: " << ray.lastRayStep.x << " " << ray.lastRayStep.y << " " << ray.lastRayStep.z << std::endl;
	if (ray.hit && lastBlock == Block::Air) {
		_world->PlayerSetBlock(ray.lastRayStep, blockType);
	}

	return;
}

void Player::DestroyBlock(glm::vec3& _position, glm::vec3& forward)
{
	RayCastHitInfo ray;
	const float maxBlockDist = 4.f;

	ray = RayCast(_position, forward, maxBlockDist, 0.5f);
	if (ray.hitRayPos != glm::vec3(INFINITY, INFINITY, INFINITY) && (!_movementPropety.godMode && ray.hitBlock != Block::Bedrock)) {
		_world->PlayerSetBlock(ray.hitRayPos, BlockType::Air);
	}
	else if (ray.hit && _movementPropety.godMode) {
		_world->PlayerSetBlock(ray.hitRayPos, BlockType::Air);
	}
	return;
}

void Player::PlayerMovement(Input* input, glm::vec3& forward, glm::vec3& right, glm::vec3& up, const float delta)
{
	RayCastHitInfo upperRayInfo;
	RayCastHitInfo lowerRayInfo;

	forward.y = 0.f;
	right.y = 0.f;
	up.x = 0.f;
	up.z = 0.f;

	if (input->KeyPressed(GLFW_KEY_W)) {
		_movementPropety.velocity += forward;
		upperRayInfo = RayCast(_position, _movementPropety.velocity, 0.7f, 0.1f);
		if (upperRayInfo.hit && upperRayInfo.distance <= _movementPropety.avoidBlockDistance) {
			_movementPropety.velocity = glm::vec3(0);
		}
	}
	if (input->KeyPressed(GLFW_KEY_S)) {
		_movementPropety.velocity -= forward;
		upperRayInfo = RayCast(_position, _movementPropety.velocity, 0.7f, 0.1f);
		if (upperRayInfo.hit && upperRayInfo.distance <= _movementPropety.avoidBlockDistance) {
			_movementPropety.velocity = glm::vec3(0);
		}
	}
	if (input->KeyJustPressed(GLFW_KEY_SPACE) && !_movementPropety.isAir) {
		upperRayInfo = RayCast(_position, _movementPropety.vecUp, 0.7f, 0.1f);
		if (!_movementPropety.isAir && upperRayInfo.hit && upperRayInfo.distance <= _movementPropety.avoidBlockDistance) {
			_movementPropety.velocity.y = 0.f;
		}
		else {
			_movementPropety.velocity += _movementPropety.vecUp * _movementPropety.jumpForce * delta;
			_movementPropety.isAir = true;
		}
	}
	if (input->KeyPressed(GLFW_KEY_D)) {
		_movementPropety.velocity += right;
		upperRayInfo = RayCast(_position, _movementPropety.velocity, 0.7f, 0.1f);
		if (upperRayInfo.hit && upperRayInfo.distance <= _movementPropety.avoidBlockDistance) {
			_movementPropety.velocity = glm::vec3(0);
		}
	}
	if (input->KeyPressed(GLFW_KEY_A)) {
		_movementPropety.velocity -= right;
		upperRayInfo = RayCast(_position, _movementPropety.velocity, 0.7f, 0.1f);
		if (upperRayInfo.hit && upperRayInfo.distance <= _movementPropety.avoidBlockDistance) {
			_movementPropety.velocity = glm::vec3(0);
		}
	}

	upperRayInfo = RayCast(_position, -_movementPropety.vecUp, _movementPropety.objectHeight + 0.50f, 0.20f);
	if (!upperRayInfo.hit) {
		_movementPropety.velocity.y += -(_movementPropety.g * delta);
		_movementPropety.isAir = true;
	}
	else if (upperRayInfo.hit && _position.y - glm::ceil(upperRayInfo.hitRayPos.y) <= _movementPropety.objectHeight) {
		_movementPropety.isAir = false;
		_movementPropety.velocity.y = 0.f;
		_position.y = glm::ceil(_position.y - upperRayInfo.distance) + _movementPropety.objectHeight;
	}

	if (!_movementPropety.isAir) {
		_movementPropety.velocity.y = 0.f;
	}
}
void Player::GodMovement(Input* input, glm::vec3& forward, glm::vec3& right, glm::vec3& up)
{
	if (input->KeyPressed(GLFW_KEY_W)) {
		_movementPropety.velocity += forward;
	}
	if (input->KeyPressed(GLFW_KEY_S)) {
		_movementPropety.velocity -= forward;
	}
	if (input->KeyPressed(GLFW_KEY_SPACE)) {
		_movementPropety.velocity += up;
	}
	if (input->KeyPressed(GLFW_KEY_LEFT_SHIFT)) {
		_movementPropety.velocity -= up;
	}
	if (input->KeyPressed(GLFW_KEY_D)) {
		_movementPropety.velocity += right;
	}
	if (input->KeyPressed(GLFW_KEY_A)) {
		_movementPropety.velocity -= right;
	}
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
	forward = glm::mat4_cast(_rotation) * glm::vec4(0.f, 0.f, -1.f, 0.f);
	up = glm::mat4_cast(_rotation) * glm::vec4(0.f, 1.f, 0.f, 0.f);
	right = glm::mat4_cast(_rotation) * glm::vec4(1.f, 0.f, 0.f, 0.f);

	if (input->KeyPressed(GLFW_KEY_R)) {
		RayCastHitInfo info = RayCast(_position, forward, 25.f, 0.1f);
		_monkey->SetPosition(info.hitRayPos);
	}


	if (input->MouseKeyJustPressed(GLFW_MOUSE_BUTTON_LEFT)) {
		DestroyBlock(_position, forward);
	}

	if (input->MouseKeyJustPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
		PutBlock(_position, forward, BlockType::Dirt);
	}


	if (!_movementPropety.godMode) {
		_movementPropety.velocity = glm::vec3(0.f, _movementPropety.velocity.y, 0.f);
		PlayerMovement(input, forward, right, up, delta);
		_position += _movementPropety.velocity * delta * SPEED;
	}
	else {
		_movementPropety.velocity = glm::vec3(0);
		GodMovement(input, forward, right, up);
		if (_movementPropety.velocity !=  glm::vec3(0)) {
		_movementPropety.velocity = glm::normalize(_movementPropety.velocity);
		}
		_position += _movementPropety.velocity * delta * SPEED;
	}

	if (_rotateCamera) {
		_camera->SetRotation(_rotation);
	}

	_camera->SetPosition(_position);
}

glm::vec3 Player::GetPosition() {return _position;};
glm::vec3 Player::GetDirection() {return glm::mat4_cast(_rotation) * glm::vec4(0.f, 0.f, -1.f, 0.f);};
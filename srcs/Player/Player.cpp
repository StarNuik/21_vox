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
		tmpDirection = direction * step;
		ray.hitRayPos = _position + tmpDirection;
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

Player::CollisionInfo Player::CheckCollision(const glm::vec3& direction, const glm::vec3& upperBody, const glm::vec3& middleBody, const glm::vec3& lowerBody)
{
	CollisionInfo col;
	col.isCollision = false;
	col.side = glm::vec3(1.f, 0.f, 1.f);

	if (!direction.x && !direction.z) {
		return col;
	}
	glm::vec3 tmpPosition = _position;
	tmpPosition.x = glm::abs(tmpPosition.x);
	tmpPosition.z = glm::abs(tmpPosition.z);
	float distX = tmpPosition.x - floorf(tmpPosition.x);
	float distZ = tmpPosition.z - floorf(tmpPosition.z);
	distX = (distX > 0.5f ? 1.f - distX : distX);
	distZ = (distZ > 0.5f ? 1.f - distZ : distZ);

	if (distX > _movementPropety.avoidBlockDistance && distZ > _movementPropety.avoidBlockDistance)
		return col;
	else
		col.isCollision = true;

	if (distX < _movementPropety.avoidBlockDistance) {
		glm::vec3 offset = glm::vec3(direction.x < 0 ? -.5f : .5f, 0.f, 0.f);
		Block blockX = _world->GetBlock(lowerBody + offset);
		if (blockX != Block::Air) {
			col.side.z = 0.f;
		}
		blockX = _world->GetBlock(middleBody + offset);
		if (blockX != Block::Air){
			col.side.z = 0.f;
		}
		blockX = _world->GetBlock(upperBody + offset);
		if (blockX != Block::Air) {
			col.side.z = 0.f;
		}
	}
	if (distZ < _movementPropety.avoidBlockDistance) {
		glm::vec3 offset = glm::vec3(0.f, 0.f, direction.z < 0 ? -.5f : .5f);
		Block blockZ = _world->GetBlock(lowerBody + offset);
		if (blockZ != Block::Air) {
			col.side.x = 0.f;
		}
		blockZ = _world->GetBlock(middleBody + offset);
		if (blockZ != Block::Air) {
			col.side.x = 0.f;
		}
		blockZ = _world->GetBlock(upperBody + offset);
		if (blockZ != Block::Air) {
			col.side.x = 0.f;
		}
	}

	return col;
}

void Player::PlayerHorizontalMovement(Input* input, glm::vec3& forward, glm::vec3& right)
{
	RayCastHitInfo upperRayInfo;
	RayCastHitInfo lowerRayInfo;
	_upperBody = GetUpperBody();
	_middleBody = GetMiddleBody();
	_lowerBody = GetLowerBody();

	forward.y = 0.f;
	right.y = 0.f;

	glm::vec3 myMovement = glm::vec3(0.f);
	if (input->KeyPressed(GLFW_KEY_W)) {
		myMovement += forward;
	}
	if (input->KeyPressed(GLFW_KEY_S)) {
		myMovement -= forward;
	}
	if (input->KeyPressed(GLFW_KEY_D)) {
		myMovement += right;
	}
	if (input->KeyPressed(GLFW_KEY_A)) {
		myMovement -= right;
	}

	_movementPropety.velocity += myMovement;
	CollisionInfo col;
	if (_movementPropety.velocity != glm::vec3(0.f))
		col = CheckCollision(myMovement, _upperBody, _middleBody, _lowerBody);

	if (!col.isCollision) {
		return;
	}
	if (_movementPropety.velocity != glm::vec3(0.f)) {
		float velY = _movementPropety.velocity.y;
		_movementPropety.velocity = glm::normalize(_movementPropety.velocity);
		_movementPropety.velocity = glm::vec3(_movementPropety.velocity.x * col.side.z, 0.f, _movementPropety.velocity.z * col.side.x);
		_movementPropety.velocity.y = velY;
	}
	return;
}


void Player::PlayerVerticalMovement(Input* input)
{
	RayCastHitInfo upperRayInfo;
	glm::vec3 velocityY = _movementPropety.velocity;

	if (input->KeyPressed(GLFW_KEY_LEFT_SHIFT) && !_movementPropety.isJump) {
		_movementPropety.isCrouch = true;
		_movementPropety.currObjectHeight = _movementPropety.maxObjectHeight * 0.75f;
		_upperBody = glm::vec3(_upperBody.x, _position.y - _movementPropety.currObjectHeight, _upperBody.z);
	}
	else {
		_movementPropety.isCrouch = false;
		_movementPropety.currObjectHeight = _movementPropety.maxObjectHeight;
		_upperBody = GetUpperBody();
	}

	if (input->KeyJustPressed(GLFW_KEY_SPACE) && !_movementPropety.isJump) {
	upperRayInfo = RayCast(_upperBody, _movementPropety.vecUp, 0.7f, 0.1f);
	velocityY += _movementPropety.vecUp * _movementPropety.jumpForce;
	_movementPropety.isAir = true;
	_movementPropety.isJump = false;
	}

	upperRayInfo = RayCast(_upperBody, _movementPropety.vecUp, 0.5f, 0.1f);
	if (_movementPropety.isAir && upperRayInfo.hit && upperRayInfo.distance <= _movementPropety.avoidBlockDistance && velocityY.y > 0.f) {
		velocityY.y = 0.f;
	}
	upperRayInfo = RayCast(_upperBody, -_movementPropety.vecUp, _movementPropety.currObjectHeight + 0.50f, 0.20f);
	if (!upperRayInfo.hit) {
		velocityY.y += -(_movementPropety.g * _delta);
		_movementPropety.isAir = true;
		_movementPropety.isJump = true;
	}
	else if (upperRayInfo.hit && _upperBody.y - glm::ceil(upperRayInfo.hitRayPos.y) <= _movementPropety.currObjectHeight) {
		_movementPropety.isAir = false;
		_movementPropety.isJump = false;
		velocityY.y = 0.f;
		_upperBody.y = glm::ceil(_upperBody.y - upperRayInfo.distance) + _movementPropety.currObjectHeight;
	}
	_position.y = _upperBody.y;
	_movementPropety.velocity.y = velocityY.y;
	if (!_movementPropety.isCrouch) {
		Move(_movementPropety.velocity, SPEED);
	}
	else {
		Move(_movementPropety.velocity, CROUCHING_SPEED);
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
	if (_movementPropety.velocity !=  glm::vec3(0)) {
		_movementPropety.velocity = glm::normalize(_movementPropety.velocity);
	}
	Move(_movementPropety.velocity, SPEED);

}

void Player::Update(float delta) {
	Input* input = _game->GetInput();
	glm::vec3 forward;
	glm::vec3 up;
	glm::vec3 right;

	_delta = delta;
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
		PlayerHorizontalMovement(input, forward, right);
		PlayerVerticalMovement(input);
	}
	else {
		_movementPropety.velocity = glm::vec3(0);
		GodMovement(input, forward, right, up);
	}

	if (_rotateCamera) {
		_camera->SetRotation(_rotation);
	}

	_camera->SetPosition(_position);
}

glm::vec3 Player::GetPosition() {return _position;};
glm::vec3 Player::GetDirection() {return glm::mat4_cast(_rotation) * glm::vec4(0.f, 0.f, -1.f, 0.f);};
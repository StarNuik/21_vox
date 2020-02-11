#include "Player/Player.h"
#include "Engine/Physics.h"
#include "Utilities/Log.h"
#include "Utilities/FloatComparison.h"
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
	_physics = _game->GetPhysics();
	//! Looks like a fucking costil
	_game->GetUI()->SetPlayer(this);
	_position = glm::vec3(0.f, 120.f, 3.f);
	_rotation = glm::identity<glm::quat>();
	_camera = new Camera(_game->GetRenderer(), 90.f, 0.1f, 300.f);
	_game->GetRenderer()->SetActiveCamera(_camera);
	_world = _game->GetWorld();
	MovementProperty _movementPropety;
	ResourceLoader* rs = _game->GetResources();
	_selectedBlock = Block::Dirt;
	_monkey = new RenderModel(_game->GetRenderer(), rs->GetShader("Base"), rs->GetMaterial(Block::Cobblestone), rs->GetGeometry("BoxC"));
}

Player::~Player() {
	_game->RemoveEntity(this);
	_game->GetRenderer()->SetActiveCamera(nullptr);
	delete _camera;
}

void Player::PutBlock(glm::vec3& _position, glm::vec3& forward, Block blockType)
{
	const float maxBlockDist = 4.f;
	Block lastBlock;
	Physics::RayCastHitInfo ray;

	ray = _physics->RayCast(_position, forward, maxBlockDist, 0.25f);
	lastBlock = _world->GetBlock(ray.lastRayStep);
	if (ray.hit && lastBlock == Block::Air) {
		_world->PlayerSetBlock(ray.lastRayStep, blockType);
	}
	return;
}

void Player::DestroyBlock(glm::vec3& _position, glm::vec3& forward)
{
	Physics::RayCastHitInfo ray;
	const float maxBlockDist = 4.f;

	ray = _physics->RayCast(_position, forward, maxBlockDist, 0.5f);
	if (ray.hitRayPos != glm::vec3(INFINITY, INFINITY, INFINITY) && (!_movementPropety.godMode && ray.hitBlock != Block::Bedrock)) {
		_world->PlayerSetBlock(ray.hitRayPos, Block::Air);
	}
	else if (ray.hit && _movementPropety.godMode) {
		_world->PlayerSetBlock(ray.hitRayPos, Block::Air);
	}
	return;
}

void Player::PlayerHorizontalMovement(Input* input, glm::vec3& forward, glm::vec3& right)
{
	Physics::CollisionInfo col;
	_upperBody = GetUpperBody();
	_middleBody = GetMiddleBody();
	_lowerBody = GetLowerBody();

	forward.y = 0.f;
	right.y = 0.f;
	forward = glm::normalize(forward);
	right = glm::normalize(right);
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
	if (_movementPropety.velocity != glm::vec3(0.f))
		col = _physics->CheckCollision(myMovement, _upperBody, _middleBody, _lowerBody, _position, _movementPropety.avoidBlockDistance);


	if (col.isCollision) {
		if (_movementPropety.velocity != glm::vec3(0.f)) {
			float velY = _movementPropety.velocity.y;
			_movementPropety.velocity = glm::normalize(_movementPropety.velocity);
			_movementPropety.velocity = glm::vec3(_movementPropety.velocity.x * col.sideNormal.z, 0.f, _movementPropety.velocity.z * col.sideNormal.x);
			_movementPropety.velocity.y = velY;
		}
	}

	if (!col.sideNormal.z && glm::abs(_movementPropety.velocity.x) * SPEED * _delta > col.distX) {
		_movementPropety.velocity.x = col.distX / _delta / SPEED * (_movementPropety.velocity.x > 0.f ? 1.f : -1.f);
	}
	if (!col.sideNormal.x && glm::abs(_movementPropety.velocity.z) * SPEED * _delta  > col.distZ) {
		_movementPropety.velocity.z = col.distZ / _delta / SPEED * (_movementPropety.velocity.z > 0.f ? 1.f : -1.f);
	}

	if (!_movementPropety.isCrouch) {
		Move(glm::vec3(_movementPropety.velocity.x, 0.f, _movementPropety.velocity.z), SPEED);
	}
	else {
		Move(glm::vec3(_movementPropety.velocity.x, 0.f, _movementPropety.velocity.z), CROUCHING_SPEED);
	}

	return;
}


void Player::PlayerVerticalMovement(Input* input)
{
	Physics::RayCastHitInfo upperRayInfo;
	Physics::RayCastHitInfo lowerRayInfo;
	float velocityY = _movementPropety.velocity.y;

	if (input->KeyPressed(GLFW_KEY_SPACE) && !_movementPropety.isAir) {
		velocityY += _movementPropety.jumpForce;
		_movementPropety.isAir = true;
		_movementPropety.isJump = true;
	}

	if (input->KeyPressed(GLFW_KEY_LEFT_SHIFT) && !_movementPropety.isJump && !_movementPropety.isAir) {
		_movementPropety.isCrouch = true;
		_movementPropety.currObjectHeight = _movementPropety.maxObjectHeight * 0.75f;
		_upperBody = glm::vec3(_upperBody.x, _position.y - _movementPropety.currObjectHeight, _upperBody.z);
	}
	else {
		_movementPropety.isCrouch = false;
		_movementPropety.currObjectHeight = _movementPropety.maxObjectHeight;
		_upperBody = GetUpperBody();
	}


	upperRayInfo = _physics->RayCastWalkthrough(_upperBody, glm::vec3(0.f, 1.f, 0.f), 0.7f, 0.1f);
	if (_movementPropety.isAir && upperRayInfo.hit && upperRayInfo.distance <= _movementPropety.avoidBlockDistance && velocityY > 0.f) {
		velocityY = 0.f;
	}

	lowerRayInfo = _physics->RayCastWalkthrough(_lowerBody, glm::vec3(0.f, -1.f, 0.f), 0.3f, 0.051f);
	if (lowerRayInfo.hit && FloatLessThan(_lowerBody.y - glm::ceil(lowerRayInfo.hitRayPos.y), 0.12f) && !_movementPropety.isJump) {
		_movementPropety.isAir = false;
		velocityY = 0.f;
		_upperBody.y = glm::ceil(_lowerBody.y - lowerRayInfo.distance) + _movementPropety.currObjectHeight;
	}
	else {
		velocityY += -(_movementPropety.g * _delta);
		_movementPropety.isAir = true;
		_movementPropety.isJump = false;
	}

	_position.y = _upperBody.y;
	_movementPropety.velocity.y = velocityY;
	if (!_movementPropety.isCrouch) {
		Move(glm::vec3(0.f, velocityY, 0.f), SPEED);
	}
	else {
		Move(glm::vec3(0.f, velocityY, 0.f), CROUCHING_SPEED);
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

void Player::BlockSelection() {
	Input* input = _game->GetInput();
	
	if (input->KeyJustPressed(GLFW_KEY_KP_ADD)) {
		_selectedBlock++;
		if (_selectedBlock > Block::Last) {
			_selectedBlock = Block::First;
		}
	}
	if (input->KeyJustPressed(GLFW_KEY_KP_SUBTRACT)){
		_selectedBlock--;
		if (_selectedBlock < Block::First) {
			_selectedBlock = Block::Last;
		}
	}
}

void Player::Update(float delta) {
	Input* input = _game->GetInput();
	glm::vec3 forward;
	glm::vec3 up;
	glm::vec3 right;

	_delta = delta;
	BlockSelection();
	if (input->KeyJustPressed(GLFW_KEY_E)) {
		bool state = !_game->GetUI()->GetState();
		_game->GetUI()->SetState(state);
		_game->GetRenderer()->SetCursor(state);
		_rotateCamera = !state;
	}
	if (_rotateCamera) {
		glm::ivec2 mousePos = input->MousePosDelta();
		_camAngleX += mousePos.x * MOUSE_SENSITIVITY;
		_camAngleY += mousePos.y * MOUSE_SENSITIVITY;
		_camAngleY = glm::clamp(_camAngleY, -89.5f, 89.5f);
	}

	if (input->KeyJustPressed(GLFW_KEY_G)) {
		_movementPropety.godMode = (_movementPropety.godMode + 1) % 2;
	}

	_rotation = glm::quat(-glm::vec3(glm::radians(_camAngleY), glm::radians(_camAngleX), 0.f));
	forward = glm::mat4_cast(_rotation) * glm::vec4(0.f, 0.f, -1.f, 0.f);
	up = glm::mat4_cast(_rotation) * glm::vec4(0.f, 1.f, 0.f, 0.f);
	right = glm::mat4_cast(_rotation) * glm::vec4(1.f, 0.f, 0.f, 0.f);

	if (input->KeyPressed(GLFW_KEY_R)) {
		Physics::RayCastHitInfo info = _physics->RayCast(_position, forward, 25.f, 0.1f);
		_monkey->SetPosition(info.hitRayPos);
	}

	if (input->MouseKeyJustPressed(GLFW_MOUSE_BUTTON_LEFT)) {
		DestroyBlock(_position, forward);
	}

	if (input->MouseKeyJustPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
		PutBlock(_position, forward, _selectedBlock);
	}

	if (!_movementPropety.godMode) {
		_movementPropety.velocity = glm::vec3(0.f, _movementPropety.velocity.y, 0.f);
		PlayerHorizontalMovement(input, forward, right);
		PlayerVerticalMovement(input);
	} else {
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
#include "Player/Player.h"
#include "Utilities/Log.h"
#include "Utilities/FloatComparison.h"
#include "Engine/Game.h"
#include "Render/GLRenderer.h"
#include "Render/Camera.h"
#include "Input/Input.h"
#include "Render/RenderModel.h"
#include "World/ResourceLoader.h"
#include "UI/UIController.h"
#include "Animation/AnimationModel.h"

Player::Player(Game* game) {
	_camAngleX = 0.f;
	_camAngleY = 0.f;
	_game = game;
	_rotateCamera = true;
	//! Looks like a fucking costil
	_game->GetUI()->SetPlayer(this);
	_position = mathf::vec3(0.f, 0.f, 3.f);
	_rotation = mathf::quat::identity();
	_camera = new Camera(_game->GetRenderer(), 90.f, 0.1f, 300.f);
	_game->GetRenderer()->SetActiveCamera(_camera);
	MovementProperty _movementPropety;
	ResourceLoader* rs = _game->GetResources();
	_selectedBlock = Block::Dirt;
	_monkey = new RenderModel(_game->GetRenderer(), rs->GetShader("Shadows"), rs->GetMaterial(Block::Stone), rs->GetGeometry("BoxC"));
	_monkey->SetScale(mathf::vec3(5.f));
	_monkey->SetPosition(mathf::vec3(0.f, -2.5f, 0.f));
}

Player::~Player() {
	_game->RemoveEntity(this);
	_game->GetRenderer()->SetActiveCamera(nullptr);
	delete _camera;
}

void Player::GodMovement(Input* input, mathf::vec3& forward, mathf::vec3& right, mathf::vec3& up)
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
	if (_movementPropety.velocity !=  mathf::vec3(0)) {
		_movementPropety.velocity = _movementPropety.velocity.normalize();
	}
	Move(_movementPropety.velocity, FLY_SPEED);

}

void Player::Update(float delta) {
	Input* input = _game->GetInput();
	mathf::vec3 forward;
	mathf::vec3 up;
	mathf::vec3 right;

	_delta = delta;

	if (input->KeyJustPressed(GLFW_KEY_E)) {
		bool state = !_game->GetUI()->GetState();
		_game->GetUI()->SetState(state);
		_game->GetRenderer()->SetCursor(state);
		_rotateCamera = !state;
	}
	if (_rotateCamera) {
		mathf::ivec2 mousePos = input->MousePosDelta();
		_camAngleX += mousePos.x * MOUSE_SENSITIVITY * delta;
		_camAngleY += mousePos.y * MOUSE_SENSITIVITY * delta;
		_camAngleY = mathf::clamp(_camAngleY, -89.5f, 89.5f);
	}

	_rotation = mathf::quat((-mathf::vec3(mathf::radians(_camAngleY), mathf::radians(_camAngleX), 0.f)));
	forward = mathf::vec3(mathf::mat4x4::cast(_rotation) * mathf::vec4(0.f, 0.f, -1.f, 0.f));
	up = mathf::vec3(mathf::mat4x4::cast(_rotation) * mathf::vec4(0.f, 1.f, 0.f, 0.f));
	right = mathf::vec3(mathf::mat4x4::cast(_rotation) * mathf::vec4(1.f, 0.f, 0.f, 0.f));

	_movementPropety.velocity = mathf::vec3(0);
	GodMovement(input, forward, right, up);

	if (_rotateCamera) {
		_camera->SetRotation(_rotation);
	}

	{
		ResourceLoader* r = _game->GetResources();
		if (input->KeyJustPressed(GLFW_KEY_1)) {
			_game->GetHuman()->SetAnimationClip(r->GetAnimationClip("Idle"));
		}
		if (input->KeyJustPressed(GLFW_KEY_2)) {
			_game->GetHuman()->SetAnimationClip(r->GetAnimationClip("Walk"));
		}
		if (input->KeyJustPressed(GLFW_KEY_3)) {
			_game->GetHuman()->SetAnimationClip(r->GetAnimationClip("Jump"));
		}
		if (input->KeyJustPressed(GLFW_KEY_4)) {
			_game->GetHuman()->SetAnimationClip(r->GetAnimationClip("Jumping Jacks"));
		}
		if (input->KeyJustPressed(GLFW_KEY_5)) {
			_game->GetHuman()->SetAnimationClip(r->GetAnimationClip("Boxing1"));
		}
		if (input->KeyJustPressed(GLFW_KEY_6)) {
			_game->GetHuman()->SetAnimationClip(r->GetAnimationClip("Boxing2"));
		}
		if (input->KeyJustPressed(GLFW_KEY_7)) {
			_game->GetHuman()->SetAnimationClip(r->GetAnimationClip("Gun"));
		}
		if (input->KeyJustPressed(GLFW_KEY_8)) {
			_game->GetHuman()->SetAnimationClip(r->GetAnimationClip("Dance1"));
		}
		if (input->KeyJustPressed(GLFW_KEY_9)) {
			_game->GetHuman()->SetAnimationClip(r->GetAnimationClip("Dance2"));
		}
		if (input->KeyJustPressed(GLFW_KEY_0)) {
			_game->GetHuman()->SetAnimationClip(r->GetAnimationClip("Dance3"));
		}
		if (input->KeyJustPressed(GLFW_KEY_MINUS)) {
			_game->GetHuman()->SetAnimationClip(r->GetAnimationClip("Dance4"));
		}
		if (input->KeyJustPressed(GLFW_KEY_EQUAL)) {
			_game->GetHuman()->SetAnimationClip(r->GetAnimationClip("Dance5"));
		}
	}

	_camera->SetPosition(_position);
}

mathf::vec3 Player::GetPosition() {return _position;};
mathf::vec3 Player::GetDirection() {return mathf::vec3(mathf::mat4x4::cast(_rotation) * mathf::vec4(0.f, 0.f, -1.f, 0.f));}
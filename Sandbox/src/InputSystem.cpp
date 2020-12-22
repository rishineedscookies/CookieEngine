#include "InputSystem.h"
#include "Cookie/Input/Input.h"

float InputSystem::CurrMouseX = 0;
float InputSystem::CurrMouseY = 0;
float InputSystem::DeltaMouseX = 0;
float InputSystem::DeltaMouseY = 0;

void InputSystem::OnUpdate(Cookie::World* World, Cookie::Time* Time)
{
	float NewMouseX = Cookie::Input::GetMouseX();
	float NewMouseY = Cookie::Input::GetMouseY();
	InputSystem::DeltaMouseX = NewMouseX - CurrMouseX;
	InputSystem::DeltaMouseY = NewMouseY - CurrMouseY;
	InputSystem::CurrMouseX = NewMouseX;
	InputSystem::CurrMouseY = NewMouseY;
}
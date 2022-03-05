#include <backend/input.h>
#include <lev/input/input.h>

using namespace lev;

namespace
{
	KeyboardState g_kb;
	KeyboardState g_prev_kb;

	MouseState g_mouse;
	MouseState g_prev_mouse;
}

bool Input::init()
{
	return true;
}

void Input::destroy()
{
}

void Input::update()
{
	g_prev_kb = g_kb;
	g_prev_mouse = g_mouse;

	// retrieve input
}

bool Input::down(VirtualKey vkey)
{
	for (auto& k : vkey.keys)
	{
		if (down(k))
			return true;
	}

	for (auto& mb : vkey.mouse_buttons)
	{
		if (down(mb))
			return true;
	}

	return false;
}

bool Input::down(MouseButton mb)
{
	return g_mouse.down[(int)mb];
}

bool Input::down(Key key)
{
	return g_kb.down[(int)key];
}

bool Input::released(VirtualKey vkey)
{
	for (auto& k : vkey.keys)
	{
		if (released(k))
			return true;
	}

	for (auto& mb : vkey.mouse_buttons)
	{
		if (released(mb))
			return true;
	}

	return false;
}

bool Input::released(MouseButton mb)
{
	return g_mouse.released[(int)mb];
}

bool Input::released(Key key)
{
	return g_mouse.released[(int)key];
}

bool Input::pressed(VirtualKey vkey)
{
	for (auto& k : vkey.keys)
	{
		if (pressed(k))
			return true;
	}

	for (auto& mb : vkey.mouse_buttons)
	{
		if (pressed(mb))
			return true;
	}

	return false;
}

bool Input::pressed(MouseButton mb)
{
	return g_mouse.pressed[(int)mb];
}

bool Input::pressed(Key key)
{
	return g_kb.pressed[(int)key];
}

bool Input::ctrl()
{
	return g_kb.down[(int)Key::LEFT_CONTROL] || g_kb.down[(int)Key::RIGHT_CONTROL];
}

bool Input::shift()
{
	return g_kb.down[(int)Key::LEFT_SHIFT] || g_kb.down[(int)Key::RIGHT_SHIFT];
}

bool Input::alt()
{
	return g_kb.down[(int)Key::LEFT_ALT] || g_kb.down[(int)Key::RIGHT_ALT];
}

const char* Input::text()
{
	return g_kb.text;
}

Vec2 Input::mouse_screen_pos()
{
	return g_mouse.screen_position;
}

Vec2 Input::mouse_draw_pos()
{
	return g_mouse.draw_position;
}

Vec2 Input::mouse_position()
{
	return g_mouse.position;
}

Float2 Input::mouse_wheel()
{
	return g_mouse.wheel;
}

Float2 Input::mouse_wheel_change()
{
	return g_prev_mouse.wheel - g_mouse.wheel;
}

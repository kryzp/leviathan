#include <lev/input/input.h>
#include <lev/core/app.h>

using namespace lev;

bool Input::init()
{
	return true;
}

void Input::destroy()
{
}

void Input::update()
{
	m_kb_prev = m_kb;
	m_mouse_prev = m_mouse;

	mem::set(m_kb.text, 0, LEV_MAX_TEXT_INPUT);
	m_mouse.wheel = Float2::zero();
}

bool Input::down_mb(MouseButton mb)
{
	return m_mouse.down[mb];
}

bool Input::down_key(Key key)
{
	return m_kb.down[key];
}

bool Input::down_button(JoystickButton button)
{
	return m_joystick.down[button];
}

bool Input::released_mb(MouseButton mb)
{
	return !m_mouse.down[mb] && m_mouse_prev.down[mb];
}

bool Input::released_key(Key key)
{
	return !m_kb.down[key] && m_kb_prev.down[key];
}

bool Input::released_button(JoystickButton button)
{
	return !m_joystick.down[button] && m_joystick_prev.down[button];
}

bool Input::pressed_mb(MouseButton mb)
{
	return m_mouse.down[mb] && !m_mouse_prev.down[mb];
}

bool Input::pressed_key(Key key)
{
	return m_kb.down[key] && !m_kb_prev.down[key];
}

bool Input::pressed_button(JoystickButton button)
{
	return m_joystick.down[button] && !m_joystick_prev.down[button];
}

Float2 Input::left_stick()
{
	return m_joystick.left_stick;
}

Float2 Input::right_stick()
{
	return m_joystick.right_stick;
}

float Input::left_trigger()
{
	return m_joystick.left_trigger;
}

float Input::right_trigger()
{
	return m_joystick.right_trigger;
}

bool Input::ctrl()
{
	return down_key(KEY_LEFT_CONTROL) || down_key(KEY_RIGHT_CONTROL);
}

bool Input::shift()
{
	return down_key(KEY_LEFT_SHIFT) || down_key(KEY_RIGHT_SHIFT);
}

bool Input::alt()
{
	return down_key(KEY_LEFT_ALT) || down_key(KEY_RIGHT_ALT);
}

const char* Input::text()
{
	return m_kb.text;
}

Vec2F Input::mouse_screen_pos()
{
	return m_mouse.screen_position;
}

Vec2F Input::mouse_draw_pos()
{
	return m_mouse.draw_position;
}

Vec2F Input::mouse_position()
{
	return m_mouse.position;
}

Float2 Input::mouse_wheel()
{
	return m_mouse.wheel;
}

Float2 Input::mouse_wheel_change()
{
	return m_mouse.wheel - m_mouse_prev.wheel;
}

void Input::on_mouse_move(float x, float y)
{
	m_mouse.position = Vec2F(x, y);

	m_mouse.draw_position = Vec2F(
		(x / App::inst()->window_width()) * App::inst()->draw_width(),
		(y / App::inst()->window_height()) * App::inst()->draw_height()
	);
}

void Input::on_mouse_screen_move(float x, float y)
{
	m_mouse.screen_position = Vec2F(x, y);
}

void Input::on_mouse_down(MouseButton button)
{
	m_mouse.down[button] = true;
}

void Input::on_mouse_up(MouseButton button)
{
	m_mouse.down[button] = false;
}

void Input::on_mouse_wheel(float x, float y)
{
	m_mouse.wheel = Float2(x, y);
}

void Input::on_key_down(Key key)
{
	m_kb.down[key] = true;
}

void Input::on_key_up(Key key)
{
	m_kb.down[key] = false;
}

void Input::on_text_utf8(const char* text)
{
	cstr::cncat(m_kb.text, text, LEV_MAX_TEXT_INPUT);
}

void Input::on_joystick_button_down(JoystickButton button)
{
	m_joystick.down[button] = true;
}

void Input::on_joystick_button_up(JoystickButton button)
{
	m_joystick.down[button] = false;
}

void Input::on_joystick_ball(u8 ball)
{
	// todo
}

void Input::on_joystick_hat(u8 hat)
{
	// todo
}

void Input::on_joystick_motion(JoystickAxis axis, float value)
{
	// todo
	// todo
	// todo

	if (axis == JS_AXIS_H)
	{
	}
	else if (axis == JS_AXIS_V)
	{
	}
}

#pragma once

#include <lev/containers/vector.h>
#include <lev/math/vec2.h>

namespace lev
{
	namespace Input
	{
		constexpr int MAX_TEXT_INPUT = 256;
	}
	
	enum class MouseButton
	{
		UNKNOWN = 0,
		LEFT = 1,
		MIDDLE = 2,
		RIGHT = 3,
		SIDE_BOTTOM = 4,
		SIDE_TOP = 5,
		MAX,
	};

	enum class Key
	{
		UNKNOWN = 0,
		A = 4,
		B = 5,
		C = 6,
		D = 7,
		E = 8,
		F = 9,
		G = 10,
		H = 11,
		I = 12,
		J = 13,
		K = 14,
		L = 15,
		M = 16,
		N = 17,
		O = 18,
		P = 19,
		Q = 20,
		R = 21,
		S = 22,
		T = 23,
		U = 24,
		V = 25,
		W = 26,
		X = 27,
		Y = 28,
		Z = 29,
		D1 = 30,
		D2 = 31,
		D3 = 32,
		D4 = 33,
		D5 = 34,
		D6 = 35,
		D7 = 36,
		D8 = 37,
		D9 = 38,
		D0 = 39,
		ENTER = 40,
		ESCAPE = 41,
		BACKSPACE = 42,
		TAB = 43,
		SPACE = 44,
		MINUS = 45,
		EQUALS = 46,
		LEFT_BRACKET = 47,
		RIGHT_BRACKET = 48,
		BACKSLASH = 49,
		NON_US_HASH = 50,
		SEMICOLON = 51,
		APOSTROPHE = 52,
		GRAVE = 53,
		COMMA = 54,
		PERIOD = 55,
		SLASH = 56,
		CAPSLOCK = 57,
		F1 = 58,
		F2 = 59,
		F3 = 60,
		F4 = 61,
		F5 = 62,
		F6 = 63,
		F7 = 64,
		F8 = 65,
		F9 = 66,
		F10 = 67,
		F11 = 68,
		F12 = 69,
		PRINT_SCREEN = 70,
		SCROLL_LOCK = 71,
		PAUSE = 72,
		INSERT = 73,
		HOME = 74,
		PAGE_UP = 75,
		DELETE = 76,
		END = 77,
		PAGE_DOWN = 78,
		RIGHT = 79,
		LEFT = 80,
		DOWN = 81,
		UP = 82,
		NUMLOCK_CLEAR = 83,
		KP_DIVIDE = 84,
		KP_MULTIPLY = 85,
		KP_MINUS = 86,
		KP_PLUS = 87,
		KP_ENTER = 88,
		KP_1 = 89,
		KP_2 = 90,
		KP_3 = 91,
		KP_4 = 92,
		KP_5 = 93,
		KP_6 = 94,
		KP_7 = 95,
		KP_8 = 96,
		KP_9 = 97,
		KP_0 = 98,
		KP_PERIOD = 99,
		NON_US_BACKSLASH = 100,
		APPLICATION = 101,
		POWER = 102,
		KP_EQUALS = 103,
		F13 = 104,
		F14 = 105,
		F15 = 106,
		F16 = 107,
		F17 = 108,
		F18 = 109,
		F19 = 110,
		F20 = 111,
		F21 = 112,
		F22 = 113,
		F23 = 114,
		F24 = 115,
		EXECUTE = 116,
		HELP = 117,
		MENU = 118,
		SELECT = 119,
		STOP = 120,
		AGAIN = 121,
		UNDO = 122,
		CUT = 123,
		COPY = 124,
		PASTE = 125,
		FIND = 126,
		MUTE = 127,
		VOLUME_UP = 128,
		VOLUME_DOWN = 129,
		KP_COMMA = 133,
		KP_EQUALS_AS_400 = 134,
		INTERNATIONAL1 = 135,
		INTERNATIONAL2 = 136,
		INTERNATIONAL3 = 137,
		INTERNATIONAL4 = 138,
		INTERNATIONAL5 = 139,
		INTERNATIONAL6 = 140,
		INTERNATIONAL7 = 141,
		INTERNATIONAL8 = 142,
		INTERNATIONAL9 = 143,
		LANGUAGE1 = 144,
		LANGUAGE2 = 145,
		LANGUAGE3 = 146,
		LANGUAGE4 = 147,
		LANGUAGE5 = 148,
		LANGUAGE6 = 149,
		LANGUAGE7 = 150,
		LANGUAGE8 = 151,
		LANGUAGE9 = 152,
		ALT_ERASE = 153,
		SYS_REQ = 154,
		CANCEL = 155,
		CLEAR = 156,
		PRIOR = 157,
		RETURN2 = 158,
		SEPERATOR = 159,
		OUT = 160,
		OPER = 161,
		CLEAR_AGAIN = 162,
		CRSEL = 163,
		EXSEL = 164,
		KP_00 = 176,
		KP_000 = 177,
		THOUSANDS_SEPERATOR = 178,
		DECIMAL_SEPERATOR = 179,
		CURRENCY_UNIT = 180,
		CURRENCY_SUBUNIT = 181,
		KP_LEFT_PARENT = 182,
		KP_RIGHT_PARENT = 183,
		KP_LEFT_BRACE = 184,
		KP_RIGHT_BRACE = 185,
		KP_TAB = 186,
		KP_BACKSPACE = 187,
		KP_A = 188,
		KP_B = 189,
		KP_C = 190,
		KP_D = 191,
		KP_E = 192,
		KP_F = 193,
		KP_XOR = 194,
		KP_POWER = 195,
		KP_PERCENT = 196,
		KP_LESS = 197,
		KP_GREATER = 198,
		KP_AMPERSAND = 199,
		KP_DOUBLE_AMPERSAND = 200,
		KP_VERTICAL_BAR = 201,
		KP_DOUBLE_VERTICAL_BAR = 202,
		KP_COLON = 203,
		KP_HASH = 204,
		KP_SPACE = 205,
		KP_AT = 206,
		KP_EXCLAM = 207,
		KP_MEM_STORE = 208,
		KP_MEM_RECALL = 209,
		KP_MEM_CLEAR = 210,
		KP_MEM_ADD = 211,
		KP_MEM_SUBTRACT = 212,
		KP_MEM_MULTIPLY = 213,
		KP_MEM_DIVIDE = 214,
		KP_PLUS_MINUS = 215,
		KP_CLEAR = 216,
		KP_CLEAR_ENTRY = 217,
		KP_BINARY = 218,
		KP_OCTAL = 219,
		KP_DECIMAL = 220,
		KP_HEXADECIMAL = 221,
		LEFT_CONTROL = 224,
		LEFT_SHIFT = 225,
		LEFT_ALT = 226,
		LEFT_SUPER = 227,
		RIGHT_CONTROL = 228,
		RIGHT_SHIFT = 229,
		RIGHT_ALT = 230,
		RIGHT_SUPER = 231,
		MAX
	};

	struct KeyboardState
	{
		bool down[(int)Key::MAX];
		bool released[(int)Key::MAX];
		bool pressed[(int)Key::MAX];
		char text[Input::MAX_TEXT_INPUT];
	};

	struct MouseState
	{
		bool down[(int)MouseButton::MAX];
		bool pressed[(int)MouseButton::MAX];
		bool released[(int)MouseButton::MAX];

		Point2 screen_position;
		Point2 draw_position;
		Point2 position;
		
		Float2 wheel;
	};

	namespace Input
	{
		bool init();
		void destroy();
		void update();

		bool down(MouseButton mb);
		bool down(Key key);
		
		bool released(MouseButton mb);
		bool released(Key key);
		
		bool pressed(MouseButton mb);
		bool pressed(Key key);

		bool ctrl();
		bool shift();
		bool alt();

		const char* text();

		Point2 mouse_screen_pos();
		Point2 mouse_draw_pos();
		Point2 mouse_position();
		Float2 mouse_wheel();
		Float2 mouse_wheel_change();

		void on_mouse_move(float x, float y);
		void on_mouse_screen_move(float x, float y);
		
		void on_mouse_down(MouseButton button);
		void on_mouse_up(MouseButton button);
		void on_mouse_wheel(float x, float y);
		
		void on_key_down(Key key);
		void on_key_up(Key key);
		void on_text_utf8(const char* text);
	}
}

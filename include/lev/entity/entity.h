#pragma once

/* ok so heres the thing:

ECS', atleast in my opinion, are pretty garbage.

They solve problems when it comes to generality but when you're making a game you probably
already know what you want.

ECS is constantly branded as THE way to control entities, but honestly? you're moving
ALREADY EXISTING language features (composition) into a dynamically controlled environment.
That literally only slows the code down, and it doesn't help organize code too much either.
You're just spreading out the code and making it hard to understand when something breaks.

So I'm kind of stuck here. I want to make an entity system that's still good and general
but one which isn't annoying to actually use.

*/

namespace lev::ent
{
	struct PositionComponent
	{
		float x;
		float y;
	};

	struct HealthComponent
	{
		float value;
	};

	// e.g:
	class Player
	{
	public:
		PositionComponent position;
		HealthComponent health;

		Player()
		{
		}
	};
}

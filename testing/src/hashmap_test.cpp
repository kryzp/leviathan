#include <lev/core/util.h>
#include <lev/containers/hash_map.h>

using namespace lev;

int main()
{
	HashMap<int, char> hash_map;

	hash_map.insert(Pair(5, 'e'));
	hash_map.insert(Pair(3, 'd'));
	hash_map.insert(Pair(2, 'h'));
	hash_map.insert(Pair(45, 'm'));
	hash_map.erase(5);

	// d h m

	for (auto& p : hash_map)
	{
		log::print("%d : %c", p.first, p.second);
	}

	return 0;
}

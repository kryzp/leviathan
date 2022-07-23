#include <lev/core/util.h>
#include <lev/containers/hash_map.h>

using namespace lev;

int main()
{
	HashMap<char, int> hash_map;

	hash_map.insert(Pair('e', 5));
	hash_map.insert(Pair('d', 3));
	hash_map.insert(Pair('h', 2));
	hash_map.insert(Pair('m', 45));
	hash_map.erase('e');

	// d h m

	log::print("%d", hash_map['h']);

	for (auto& p : hash_map)
	{
		log::print("%c : %d", p.first, p.second);
	}

	return 0;
}

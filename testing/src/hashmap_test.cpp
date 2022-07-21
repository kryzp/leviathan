#include <lev/containers/hash_map.h>
#include <lev/core/util.h>

using namespace lv;

int main()
{
	HashMap<int, char> hash_map;

	hash_map.insert(Pair(5, 'e'));
	hash_map.insert(Pair(3, 'd'));
	hash_map.insert(Pair(2, 'h'));

	log::print("%c", hash_map.get(3));
	log::print("%c", hash_map.get(5));
	log::print("%c", hash_map.get(2));

	hash_map.insert(Pair(45, 'm'));
	hash_map.erase(5);

	log::print("%c", hash_map.get(45));

	return 0;
}

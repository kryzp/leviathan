#include <lev/core/util.h>
#include <lev/containers/linked_list.h>

using namespace lev;

int main()
{
	LinkedList<int> ll;
	ll.add(4);
	ll.add(3);
	ll.add(1);

	for (int i : ll)
		log::print("%d", i);

	return 0;
}

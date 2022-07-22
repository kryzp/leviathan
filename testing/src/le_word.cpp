
/*
 * this has nothing to do with leviathan (apart from some utility classes) i just cba to make another cpp project loll
 * i saw this challenge on leetcode though and wanted to take a crack at it
 * turns out somehow dumber than i thought i was which is a new record
 */

#include <lev/core/util.h>
#include <lev/containers/vector.h>
#include <lev/containers/string.h>
#include <lev/math/vec2.h>
#include <lev/math/complex.h>

using Word = lev::String;
using WordList = lev::Vector<Word>;

#define BOARD_WIDTH 5
#define BOARD_HEIGHT 5
#define BOARD_INDEX(_X, _Y) ((_Y * BOARD_WIDTH) + _X)

struct Node
{
	int x, y;
	char c;
};

bool iterate_dir(Node& node, const lev::Vec2I& dir, int index, const Word& word, const char* board)
{
	node.x += dir.x;
	node.y += dir.y;
	node.c = board[BOARD_INDEX(node.x, node.y)];

	bool result = false;

	if (node.c == word[index])
	{
		if (index >= word.length() - 1)
			result = true;
		else
			result = iterate_dir(node, dir, index + 1, word, board);
	}

	return result;
}

bool check_node(const Node& node, const Word& word, const char* board)
{
	for (int yo = -1; yo <= 1; yo++)
	{
		for (int xo = -1; xo <= 1; xo++)
		{
			if (node.x + xo >= 0 && node.x + xo < BOARD_WIDTH &&
				node.y + yo >= 0 && node.y + yo < BOARD_HEIGHT &&
				(xo != 0 || yo != 0))
			{
				Node n = node;

				if (iterate_dir(n, lev::Vec2I(xo, yo), 1, word, board))
					return true;
			}
		}
	}

	return false;
}

WordList find_words(const char* board, const WordList& words)
{
	WordList result;

	for (int y = 0; y < BOARD_HEIGHT; y++) {
		for (int x = 0; x < BOARD_WIDTH; x++) {
			for (auto& word : words) {

				Node n = { x, y, board[BOARD_INDEX(x, y)] };

				if (n.c == word[0] && check_node(n, word, board)) {
					result.push_back(word);
				}
			}
		}
	}

	return result;
}

int main()
{
	auto number = lev::Complex::pow(lev::Complex(0.0, 1.0), lev::Complex(0.0, 1.0));
	lev::log::print("i^i = %f + %fi", number.real, number.imag);

	return 0;

	WordList words;
	words.push_back("lap");
	words.push_back("bae");
	words.push_back("bedd");
	words.push_back("goo");

	char board[] = {
		'b', 'l', 'o', 'l', 'o',
		'g', 'a', 'o', 'g', 'a',
		'b', 'p', 'e', 'a', 'a',
		'b', 'e', 'd', 'd', 'o',
		'g', 'a', 'o', 'g', 'a',
	};

	for (auto& word : find_words(board, words))
		lev::log::print("%s", word.c_str());

	// output:
	// bae
	// lap
	// bedd

	return 0;
}

#pragma once

#include <lev/core/util.h>

namespace lev
{
	class Node;

	enum SignalType
	{
		NODE_SIGNAL_NONE = 0,
		NODE_SIGNAL_TEST,
		NODE_SIGNAL_MAX
	};

	struct Signal
	{
		u64 sender;
		u32 type;
	};
}

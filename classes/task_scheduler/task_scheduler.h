#pragma once
#include "classes/datamodel/datamodel.h"
#include "classes/visual_engine/visual_engine.h"

namespace rbx {
	struct task_scheduler : instance {
		datamodel get_datamodel();
		visual_engine get_visual_engine();
	};
}
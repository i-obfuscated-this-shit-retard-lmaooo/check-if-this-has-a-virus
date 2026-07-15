#include "task_scheduler.h"

rbx::datamodel rbx::task_scheduler::get_datamodel() {
	if (!this->addr) return datamodel();
	auto fdm = read<uintptr_t>(this->addr + Offsets::FakeDataModel::Pointer);
	if (!fdm) return datamodel();
	auto dm = read<uintptr_t>(fdm + Offsets::FakeDataModel::RealDataModel);
	return datamodel(dm);
}

rbx::visual_engine rbx::task_scheduler::get_visual_engine() {
	if (!this->addr) return visual_engine();
	auto ve = read<uintptr_t>(this->addr + Offsets::VisualEngine::Pointer);
	if (!ve) return visual_engine();
	return visual_engine(ve);
}
#include "entity.h"

#include "misc/globals.h"

rbx::part rbx::entity::get_part(std::string part_name) {
	auto character = this->get_character();
	if (!character.addr) return part();
	auto part_obj = character.find_first_child(part_name);
	return part(part_obj.addr);
}

rbx::entity rbx::entity::get_character() {
	auto modelinstance = read<uintptr_t>(this->addr + Offsets::Player::ModelInstance);
	return entity(modelinstance);
}

rbx::part rbx::entity::get_humanoid() {
	auto humanoid = this->get_part("Humanoid");
	return humanoid;
}

rbx::part rbx::entity::get_humanoid_root_part() {
	auto humanoid_root_part = this->get_part("HumanoidRootPart");
	return humanoid_root_part;
}

rbx::part rbx::entity::get_head() {
	auto head = this->get_part("Head");
	return head;
}

std::string rbx::entity::get_display_name() {
	return read_string(this->addr + Offsets::Player::DisplayName);
}

float rbx::entity::get_health() {
	auto humanoid = this->get_humanoid();
	if (!humanoid.addr) return 0.f;
	auto h = read<float>(humanoid.addr + Offsets::Humanoid::Health);
	return h;
}

float rbx::entity::get_max_health() {
	auto humanoid = this->get_humanoid();
	if (!humanoid.addr) return 0.f;
	auto mh = read<float>(humanoid.addr + Offsets::Humanoid::MaxHealth);
	return mh;
}

rbx::instance rbx::entity::get_team() {
	auto team = read<uintptr_t>(this->addr + Offsets::Player::Team);
	return instance(team);
}

bool rbx::entity::is_teammate() {
	auto our_team = rbx::globals::instances::g_local_player.get_team();
	auto enemy_team = this->get_team();
	return our_team.addr && enemy_team.addr && our_team.addr == enemy_team.addr;
}

rbx::rig_type rbx::entity::get_rig_type() {
	auto humanoid = this->get_humanoid();
	if (!humanoid.addr) return rig_type::r6; // default assumption ig
	auto rig = read<int>(humanoid.addr + Offsets::Humanoid::RigType);
	return (rig_type)(rig);
}

uint64_t rbx::entity::get_user_id() {
	auto user_id = read<uint64_t>(this->addr + Offsets::Player::UserId);
	return user_id;
}

rbx::vector3_t rbx::entity::get_move_direction() {
	vector3_t move_dir;
	auto humanoid = this->get_humanoid();
	if (!humanoid.addr) return move_dir;
	move_dir = read<vector3_t>(humanoid.addr + Offsets::Humanoid::MoveDirection);
	return move_dir;
}

int rbx::entity::get_humanoid_state_id() {
	int state_id = 0;
	auto humanoid = this->get_humanoid();
	if (!humanoid.addr) return state_id;
	auto state = read<uintptr_t>(humanoid.addr + Offsets::Humanoid::HumanoidState);
	state_id = read<int>(state + Offsets::Humanoid::HumanoidStateID);
	return state_id;
}
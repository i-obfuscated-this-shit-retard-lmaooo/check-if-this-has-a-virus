#pragma once
#include <windows.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <mutex>
#include <cstdint>
#include <atomic>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <memory>
#include <ctime>
#include <random>
#include <shellapi.h>
#include <array>
#include <filesystem>
#include <numeric>

#include "math.h"

#include "classes/instance/instance.h"
#include "classes/datamodel/datamodel.h"
#include "classes/visual_engine/visual_engine.h"
#include "classes/task_scheduler/task_scheduler.h"
#include "classes/workspace/workspace.h"
#include "classes/players/players.h"
#include "classes/camera/camera.h"
#include "classes/entity/entity.h"
#include "classes/mouse_service/mouse_service.h"
#include "classes/world/world.h"
#include "classes/lighting/lighting.h"
#include "classes/render_view/render_view.h"

#include "cache/cache.h"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_win32.h"
#include "imgui/backends/imgui_impl_dx11.h"
#include "imgui/imgui_internal.h"

#include "renderer/framework/settings/functions.h"

#define sleep_ms(ms) std::this_thread::sleep_for(std::chrono::milliseconds(ms))

namespace fs = std::filesystem;
using niga_clock = std::chrono::high_resolution_clock;

namespace rbx {
	enum supported_game {
		none,
		universal,
		arsenal,
		rivals,
		bad_business,
		da_hood,
		hood_customs,
		phantom_forces,
	};

	enum easing_function : int {
		linear,

		in_sine,
		in_quad,
		in_cubic,

		out_sine,
		out_quad,
		out_cubic,

		inout_sine,
		inout_quad,
		inout_cubic,

		custom,
	};

	inline const char* easing_names[] = {
		"Linear",

		"In Sine",
		"In Quad",
		"In Cubic",

		"Out Sine",
		"Out Quad",
		"Out Cubic",

		"InOut Sine",
		"InOut Quad",
		"InOut Cubic",

		"Custom",
	};

	inline float easing_values[][5] = {
		{ 0.f, 0.f, 1.f, 1.f }, // linear

		{ 0.47f, 0.f, 0.745f, 0.715f }, // in_sine
		{ 0.55f, 0.085f, 0.68f, 0.53f }, // in_quad
		{ 0.55f, 0.055f, 0.675f, 0.19f }, // in_cubic

		{ 0.39f, 0.575f, 0.565f, 1.f }, // out_sine
		{ 0.25f, 0.46f, 0.45f, 0.94f }, // out_quad
		{ 0.215f, 0.61f, 0.355f, 1.f }, // out_cubic

		{ 0.445f, 0.05f, 0.55f, 0.95f }, // inout_sine
		{ 0.455f, 0.03f, 0.515f, 0.955f }, // inout_quad
		{ 0.645f, 0.045f, 0.355f, 1.f }, // inout_cubic

		{ 0.f, 0.f, 1.f, 1.f }, // custom
	};

#pragma region animation stuff

	enum animation : int {
		run = 0, walk, jump,
		idle1, idle2, fall,
		swim, swim_idle, climb
	};

	enum animation_pack : int {
		original = 0, astronaut, bubbly, cartoony, elder, knight, levitation,
		mage, ninja, pirate, robot, rthro, stylish,
		superhero, toy, vampire, werewolf, zombie
	};

	inline const char* animation_pack_names[] = {
		"Default", "Astronaut", "Bubbly", "Cartoony", "Elder", "Knight",
		"Levitation", "Mage", "Ninja", "Pirate", "Robot", "Rthro", "Stylish",
		"Superhero", "Toy", "Vampire", "Werewolf", "Zombie"
	};

	struct animations {
		uint64_t run; uint64_t walk; uint64_t jump;
		uint64_t idle1; uint64_t idle2; uint64_t fall;
		uint64_t swim; uint64_t swim_idle; uint64_t climb;
	};

	struct animation_packs {
		inline static animations original = {
			.run = 507767714,
			.walk = 507777826,
			.jump = 507765000,
			.idle1 = 507766666,
			.idle2 = 507766951,
			.fall = 507767968,
			.swim = 507784897,
			.swim_idle = 507785072,
			.climb = 507765644
		};

		inline static animations astronaut = {
			.run = 891636393,
			.walk = 891636393,
			.jump = 891627522,
			.idle1 = 891621366,
			.idle2 = 891633237,
			.fall = 891617961,
			.swim = 891639666,
			.swim_idle = 891663592,
			.climb = 891609353
		};

		inline static animations bubbly = {
			.run = 910025107,
			.walk = 910034870,
			.jump = 910016857,
			.idle1 = 910004836,
			.idle2 = 910009958,
			.fall = 910001910,
			.swim = 910028158,
			.swim_idle = 910030921,
			.climb = 909997997
		};

		inline static animations cartoony = {
			.run = 742638842,
			.walk = 742640026,
			.jump = 742637942,
			.idle1 = 742637544,
			.idle2 = 742638445,
			.fall = 742637151,
			.swim = 742639220,
			.swim_idle = 742639812,
			.climb = 742636889
		};

		inline static animations elder = {
			.run = 845386501,
			.walk = 845403856,
			.jump = 845398858,
			.idle1 = 845397899,
			.idle2 = 845400520,
			.fall = 845396048,
			.swim = 845401742,
			.swim_idle = 845403127,
			.climb = 845392038
		};

		inline static animations knight = {
			.run = 657564596,
			.walk = 657552124,
			.jump = 658409194,
			.idle1 = 657595757,
			.idle2 = 657568135,
			.fall = 657600338,
			.swim = 657560551,
			.swim_idle = 657557095,
			.climb = 658360781
		};

		inline static animations levitation = {
			.run = 616010382,
			.walk = 616013216,
			.jump = 616008936,
			.idle1 = 616006778,
			.idle2 = 616008087,
			.fall = 616005863,
			.swim = 616011509,
			.swim_idle = 616012453,
			.climb = 616003713
		};

		inline static animations mage = {
			.run = 707861613,
			.walk = 707897309,
			.jump = 707853694,
			.idle1 = 707742142,
			.idle2 = 707855907,
			.fall = 707829716,
			.swim = 707876443,
			.swim_idle = 707894699,
			.climb = 707826056
		};

		inline static animations ninja = {
			.run = 656118852,
			.walk = 656121766,
			.jump = 656117878,
			.idle1 = 656117400,
			.idle2 = 656118341,
			.fall = 656115606,
			.swim = 656119721,
			.swim_idle = 656121397,
			.climb = 656114359
		};

		inline static animations pirate = {
			.run = 750783738,
			.walk = 750785693,
			.jump = 750782230,
			.idle1 = 750781874,
			.idle2 = 750782770,
			.fall = 750780242,
			.swim = 750784579,
			.swim_idle = 750785176,
			.climb = 750779899
		};

		inline static animations robot = {
			.run = 616091570,
			.walk = 616095330,
			.jump = 616090535,
			.idle1 = 616088211,
			.idle2 = 616089559,
			.fall = 616087089,
			.swim = 616092998,
			.swim_idle = 616094091,
			.climb = 616086039
		};

		inline static animations rthro = {
			.run = 2510198475,
			.walk = 2510202577,
			.jump = 2510197830,
			.idle1 = 2510197257,
			.idle2 = 2510196951,
			.fall = 2510195892,
			.swim = 2510199791,
			.swim_idle = 2510201162,
			.climb = 2510192778
		};

		inline static animations stylish = {
			.run = 616140816,
			.walk = 616146177,
			.jump = 616139451,
			.idle1 = 616136790,
			.idle2 = 616138447,
			.fall = 616134815,
			.swim = 616143378,
			.swim_idle = 616144772,
			.climb = 616133594
		};

		inline static animations superhero = {
			.run = 616117076,
			.walk = 616122287,
			.jump = 616115533,
			.idle1 = 616111295,
			.idle2 = 616113536,
			.fall = 616108001,
			.swim = 616119360,
			.swim_idle = 616120861,
			.climb = 616104706
		};

		inline static animations toy = {
			.run = 782842708,
			.walk = 782843345,
			.jump = 782847020,
			.idle1 = 782841498,
			.idle2 = 782845736,
			.fall = 782846423,
			.swim = 782844582,
			.swim_idle = 782845186,
			.climb = 782843869
		};

		inline static animations vampire = {
			.run = 1083462077,
			.walk = 1083473930,
			.jump = 1083455352,
			.idle1 = 1083445855,
			.idle2 = 1083450166,
			.fall = 1083443587,
			.swim = 1083464683,
			.swim_idle = 1083467779,
			.climb = 1083439238
		};

		inline static animations werewolf = {
			.run = 1083216690,
			.walk = 1083178339,
			.jump = 1083218792,
			.idle1 = 1083195517,
			.idle2 = 1083214717,
			.fall = 1083189019,
			.swim = 1083222527,
			.swim_idle = 1083225406,
			.climb = 1083182000
		};

		inline static animations zombie = {
			.run = 616163682,
			.walk = 616168032,
			.jump = 616161997,
			.idle1 = 616158929,
			.idle2 = 616160636,
			.fall = 616157476,
			.swim = 616165109,
			.swim_idle = 616166655,
			.climb = 616156119
		};
	};

#pragma endregion

	namespace globals {
		inline char title[200] = "";
		inline bool vsync = false;
		inline HWND our_window;
		inline std::atomic<bool> g_running = true;
		inline ImDrawList* draw = nullptr;
		inline std::atomic<float> dt = 0.f;
		inline ImTextureID logo;
		inline float fps_cap = 144.f;
		inline std::atomic<bool> chams_caching = true;

		namespace fonts {
			inline ImFont* visitor_s;
			inline ImFont* visitor_m;
			inline ImFont* visitor_l;
		}

		namespace waifu_images {
			inline ImTextureID waguri;
			inline ImTextureID alya;
			inline ImTextureID hori;
			inline ImTextureID mai;
		}

		namespace instances {
			inline task_scheduler g_task_scheduler;
			inline datamodel g_datamodel;
			inline visual_engine g_visual_engine;
			inline workspace g_workspace;
			inline players g_players;
			inline camera g_camera;
			inline mouse_service g_mouse_service;
			inline world g_world;
			inline lighting g_lighting;
			inline render_view g_render_view;
			inline entity g_local_player;
			inline matrix4_t g_view_matrix;
			inline vector2_t g_dimensions;

			namespace game {
				inline supported_game curr_game = supported_game::none;
				inline uint64_t game_id;
				inline uint64_t place_id;
				inline uint64_t job_id;
				inline uint64_t creator_id;
			}
		}

		namespace cheats {
			namespace visuals {
				inline bool self = true;
				inline const char* checks[] = { "Team", "Alive", "Health", "Distance", "Visible" };
				inline bool checks_enabled[] = { false, false, false, false, false };
				inline float health_check_amount = 20;
				inline float distance_check_amount = 1000;

				namespace box {
					inline bool enabled = false;
					inline bool outline = true;
					inline const char* styles[] = { "Normal", "Corner" };
					inline int style = 0;
					inline const char* types[] = { "Static", "Bounding" };
					inline int type = 1;
					inline auto color = ImColor(174, 82, 255);
					inline float thickness = 1.f;
					inline float rounding = 2.f;
				}

				namespace snaplines {
					inline bool enabled = false;
					inline bool outline = true;
					inline float thickness = 0.5f;
					inline auto color = ImColor(174, 82, 255);
					inline const char* origins[] = { "Mouse", "Center", "Top Center", "Bottom Center", "Local Player Head", "Local Player HRP" };
					inline int origin = 0;
					inline const char* destinations[] = { "Head", "HumanoidRootPart", "Closest" };
					inline int destination = 0;
					inline const char* styles[] = { "Line", "SpiderWeb", "Dashed" };
					inline int style = 0;
				}

				namespace skeleton {
					inline bool enabled = false;
					inline bool outline = true;
					inline auto color = ImColor(174, 82, 255);
					inline float thickness = 0.8f;

					namespace skeleton_dots {
						inline bool enabled = false;
						inline bool outline = true;
						inline auto color = ImColor(174, 82, 255);
						inline float radius = 3.f;
					}
				}

				namespace name {
					inline bool enabled = false;
					inline bool outline = true;
					inline const char* types[] = { "Username", "Display Name" };
					inline int type = 0;
					inline auto color = ImColor(174, 82, 255);
				}

				namespace movement_status {
					inline bool enabled = false;
					inline bool outline = true;
					inline auto color = ImColor(174, 82, 255);

				}

				namespace player_rig {
					inline bool enabled = false;
					inline bool outline = true;
					inline auto color = ImColor(174, 82, 255);

				}

				namespace distance {
					inline bool enabled = false;
					inline bool outline = true;
					inline auto color = ImColor(174, 82, 255);

				}

				namespace tool_name {
					inline bool enabled = false;
					inline bool outline = true;
					inline auto color = ImColor(174, 82, 255);

				}

				namespace health_bar {
					inline bool enabled = false;
					inline bool outline = true;
					inline auto color = ImColor(25, 255, 117);

				}

				namespace straw_hat {
					inline bool enabled = false;
					inline bool outline = true;
					inline auto color = ImColor(174, 82, 255);
					inline int segments = 10;
				}

				namespace waifu_esp {
					inline bool enabled = false;
				}

				namespace box_3d {
					inline bool enabled = false;
					inline bool outline = true;
					inline const char* styles[] = { "Normal", "Corner" };
					inline int style = 0;
					inline const char* types[] = { "Static", "Bounding" };
					inline int type = 1;
					inline auto color = ImColor(174, 82, 255);
					inline float thickness = 1.f;
				}

				namespace chams {
					inline bool enabled = false;
					inline bool outline = true;
					inline const char* styles[] = { "Classic", "Highlight", "Mesh" };
					inline int style = 1;
					inline auto color = ImColor(174, 82, 255,180);
					inline auto outline_color = ImColor(0, 0, 0);

					// for mesh chams 
					inline const char* materials[] = { "Classic", "Highlight", "Mesh" };
					inline int material = 1;
				}

				namespace crosshair {
					inline bool enabled = false;
					inline const char* types[] = { "Static", "Animated" };
					inline int type = 1;
					inline const char* positions[] = { "Mouse", "Center" };
					inline int position = 0;
					inline bool outline = true;
					inline float size = 10.f;
					inline float gap = 10.f;
					inline float thickness = 2.f;
					inline auto color = ImColor(212, 156, 255);
				}
			}

			namespace combat {
				namespace aimbot {
					inline bool enabled = false;
					inline int keybind = 0;
					inline int keybind_type = 0;
					inline const char* types[] = { "Mouse", "Camera" };
					inline int type = 0;
					inline const char* target_modes[] = { "Closest To Mouse", "Closest To Camera", "Closest In-Game" };
					inline int target_mode = 0;
					inline bool sticky_aim = false;
					inline bool smoothing = false;
					inline float smoothing_val = 5.f;
					inline easing_function easing_type = easing_function::linear;
					inline float easing[5] = { 0.f, 0.f, 1.f, 1.f };
					inline float speed = 1.f;
					inline bool prediction = false;
					inline float prediction_val = 5.f;
					inline const char* hitparts[] = { "Head", "HumanoidRootPart", "Random", "Closest" };
					inline int hitpart = 0;
					inline const char* checks[] = { "Team", "Alive", "Health", "Distance", "Visible", "Within FOV" };
					inline bool checks_enabled[] = { false, false, false, false, false, false };
					inline float health_check_amount = 20;
					inline float distance_check_amount = 1000;
					inline bool highlight_target = false;
					inline auto target_color = ImColor(255, 51, 82);

					namespace fov {
						inline bool enabled = false;
						inline const char* types[] = { "Static", "Animated" };
						inline int type = 0;
						inline const char* animations[] = { "Pulsing", "Breathing" };
						inline int animation = 0;
						inline const char* styles[] = { "Normal", "LGBTQ" };
						inline int style = 0;
						inline const char* positions[] = { "Mouse", "Center" };
						inline int position = 0;
						inline bool outline = true;
						inline int segments = 12;
						inline float size = 100.f;
						inline float thickness = 0.5f;
						inline auto color = ImColor(212, 156, 255);

						namespace snaplines {
							inline bool enabled = false;
							inline bool outline = true;
							inline float thickness = 0.5f;
							inline auto color = ImColor(174, 82, 255);
							inline const char* destinations[] = { "Head", "Torso", "Closest" };
							inline int destination = 0;
						}
					}

					namespace target {
						inline cache::cache_entity entity;
						inline rbx::part part;
					}
				}

				namespace silent {
					inline bool enabled = false;
					inline int keybind = 0;
					inline int keybind_type = 0;
					inline const char* types[] = { "Mouse", "Camera" };
					inline int type = 0;
					inline const char* target_modes[] = { "Closest To Mouse", "Closest To Camera", "Closest In-Game" };
					inline int target_mode = 0;
					inline bool sticky_aim = false;
					inline bool smoothing = false;
					inline float smoothing_val = 5.f;
					inline bool prediction = false;
					inline float prediction_val = 5.f;
					inline const char* hitparts[] = { "Head", "HumanoidRootPart", "Random", "Closest" };
					inline int hitpart = 0;
					inline const char* checks[] = { "Team", "Alive", "Health", "Distance", "Visible", "Within FOV" };
					inline bool checks_enabled[] = { false, false, false, false, false, false };
					inline float health_check_amount = 20;
					inline float distance_check_amount = 1000;
					inline bool highlight_target = false;
					inline auto target_color = ImColor(255, 51, 82);

					namespace locked_indicator {
						inline bool enabled = false;
						inline bool outline = true;
						inline float thickness = 0.5f;
						inline auto color = ImColor(174, 82, 255);
					}

					namespace fov {
						inline bool enabled = false;
						inline const char* types[] = { "Static", "Animated" };
						inline int type = 0;
						inline const char* animations[] = { "Pulsing", "Breathing" };
						inline int animation = 0;
						inline const char* styles[] = { "Normal", "LGBTQ" };
						inline int style = 0;
						inline const char* positions[] = { "Mouse", "Center" };
						inline int position = 0;
						inline bool outline = true;
						inline int segments = 12;
						inline float size = 100.f;
						inline float thickness = 0.5f;
						inline auto color = ImColor(212, 156, 255);

						namespace snaplines {
							inline bool enabled = false;
							inline bool outline = true;
							inline float thickness = 0.5f;
							inline auto color = ImColor(174, 82, 255);
							inline const char* destinations[] = { "Head", "Torso", "Closest" };
							inline int destination = 0;
						}
					}

					namespace target {
						inline cache::cache_entity entity;
						inline rbx::part part;
					}
				}

				namespace triggerbot {
					inline bool enabled = false;
					inline int keybind = 0;
					inline int keybind_type = 0;
					inline const char* types[] = { "Click", "Hold" };
					inline int type = 0;
					inline float clicks_per_second = 5.f;
					inline float hold_duration = 5.f; // in s
					inline float range = 10.f;
				}
			}

			namespace misc {
				namespace tickrate {
					inline bool enabled = false;
					inline int keybind = 0;
					inline int keybind_type = 0;
					inline float value = 60.f;
				}

				namespace animation_changer {
					inline bool enabled = false;
					inline int keybind = 0;
					inline int keybind_type = 0;
					inline std::atomic<animation_pack> pack = animation_pack::original;
				}

				namespace lighting {
					inline bool enabled = false;

					namespace clock_time {
						inline bool enabled = false;
						inline int keybind = 0;
						inline int keybind_type = 0;
						inline float current_time = 0.f; // seconds niga
					}
				}
			}
		}
	}
}
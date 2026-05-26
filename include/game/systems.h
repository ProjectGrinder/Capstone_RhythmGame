#pragma once

#include "game/components.h"
#include "scene/scene_decl.h"
// Input Part
#include "game/systems/input_system.h"
// Bullet Hell Part
#include "game/systems/bullethell/bullet_collision.h"
#include "game/systems/bullethell/bullet_system.h"
#include "game/systems/bullethell/bullet_clearer_system.h"
#include "game/systems/bullethell/input_to_velocity.h"
#include "game/systems/bullethell/movement_system.h"
#include "game/systems/bullethell/bullet_collision.h"
#include "game/systems/bullethell/pattern_system.h"
#include "game/systems/bullethell/player_system.h"
#include "game/systems/bullethell/player_anim_system.h"
#include "game/systems/bullethell/bullet_system.h"
#include "game/systems/bullethell/booming_system.h"
#include "game/systems/bullethell/laser_system.h"
#include "game/systems/bullethell/delay_system.h"
#include "game/systems/bullethell/special_pattern_system.h"
#include "game/systems/bullethell/particle_system.h"
#include "game/systems/bullethell/bullet_load_system.h"
#include "game/systems/bullethell/logging_system.h" // Only for demo

// Rhythm Part
#include "game/systems/rhythm/handle_bpm.h"
#include "game/systems/rhythm/handle_holding.h"
#include "game/systems/rhythm/handle_miss_note.h"
#include "game/systems/rhythm/handle_rain_note.h"
#include "game/systems/rhythm/handle_tap_note.h"
#include "game/systems/rhythm/set_holding_time.h"
#include "game/systems/rhythm/update_combo.h"
#include "game/systems/rhythm/update_judge_text.h"
#include "game/systems/rhythm/update_notes.h"
// General Battle Part
#include "game/systems/phase_change.h"
#include "game/systems/phase_change_effect.h"
#include "game/systems/global_clock.h"
#include "game/systems/check_player_state.h"
#include "game/systems/update_score.h"
// Main Menu Part

// Rendering Part
#include "systems/render/animation_system.h"
#include "systems/bullethell/shaking_system.h"
#include "systems/render/flickering_system.h"
#include "systems/render/resize_system.h"
// Render Intent
#include "systems/render/draw_sprite.h"
#include "systems/render/draw_text.h"
#include "systems/render/draw_triangle.h"
#include "systems/render/set_camera.h"


// World Part
#include "systems/world/camera_follow.h"
#include "systems/world/detect_overlap.h"
#include "systems/world/dialogue_system.h"
#include "systems/world/event_system.h"
#include "systems/world/level_page_system.h"
#include "systems/world/logging_system.h"
#include "systems/world/player_move.h"
#include "systems/world/scene_load_system.h"

// Test
#include "systems/test/fps_counter.h"
#include "systems/test/spin_camera.h"
#include "systems/test/spin_sprite.h"
#include "systems/test/stat_text_render.h"
#include "systems/test/draw_collider.h"

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
#include "game/systems/rhythm/update_judge_text.h"
#include "game/systems/rhythm/update_notes.h"
// General Battle Part
#include "game/systems/phase_change.h"
#include "game/systems/global_clock.h"
// Rendering Part
#include "game/systems/animation_system.h"
#include "systems/bullethell/shaking_system.h"
// Render Intent
#include "systems/render/draw_sprite.h"
#include "systems/render/draw_text.h"
#include "systems/render/draw_triangle.h"
#include "systems/render/set_camera.h"
// Test
#include "systems/test/spin_camera.h"
#include "systems/test/spin_sprite.h"
#include "systems/test/fps_counter.h"

#include "systems/audio/play_audio.h"

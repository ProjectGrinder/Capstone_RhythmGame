#pragma once

// Input Part
#include "game/systems/input_system.h"
// Bullet Hell Part
#include "game/systems/bullethell/input_to_velocity.h"
#include "game/systems/bullethell/movement_system.h"
#include "game/systems/bullethell/bullet_collision.h"
#include "game/systems/bullethell/pattern_system.h"
#include "game/systems/bullethell/player_system.h"
#include "game/systems/bullethell/bullet_system.h"
// Rhythm Part
#include "game/systems/rhythm/handle_rhythm.h" // TODO: Pooh implement this system
#include "game/systems/rhythm/handle_miss_note.h"
#include "game/systems/rhythm/handle_bpm.h"
// General Battle Part
#include "game/systems/phase_change.h"
// Rendering Part
#include "game/systems/animation_system.h"
#include "systems/bullethell/shaking_system.h"
// Render Pipeline
#include "game/draw_description.h"

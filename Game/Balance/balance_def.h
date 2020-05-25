#ifndef BALANCE_DEF_H
#define BALANCE_DEF_H

#include <string>

namespace BalanceDef
{

//global objects
const std::string TOWERS_KEY = "towers";
const std::string ENEMIES_KEY = "enemies";
const std::string OTHERS_KEY = "others";
const std::string LIMITS_KEY = "limits";
const std::string ABILITIES_KEY = "abilities";
const std::string MAPS_KEY = "maps";
const std::string ENEMIES_ABILITIES_KEY = "enemies_abilities";
const std::string WAVES_KEY = "waves";

//towers
const std::string BASE_KEY = "base";
const std::string FREEZE_KEY = "freeze";
const std::string POWER_KEY = "power";
const std::string ROCKET_KEY = "rocket";
const std::string LASER_KEY = "laser";
const std::string IMPROVED_KEY = "improved";

//tower stats
const std::string DAMAGE_KEY = "damage";
const std::string ATTACK_SPEED_KEY = "attackSpeed";
const std::string RADIUS_KEY = "radius";
const std::string PROJECTILE_SPEED_KEY = "projectileSpeed";
const std::string COST_KEY = "cost";
const std::string DAMAGE_OFFSET_KEY = "damage_offset";
const std::string ARMOR_DAMAGE_KEY = "armor_damage";
const std::string INFANTRY_LIGHT_ARMOR_KEY = "infantry_light_armor";
const std::string INFANTRY_HEAVY_ARMOR_KEY = "infantry_heavy_armor";
const std::string VEHICLE_LIGHT_ARMOR_KEY = "vehicle_light_armor";
const std::string VEHICLE_HEAVY_ARMOR_KEY = "vehicle_heavy_armor";
const std::string MONSTER_LIGHT_ARMOR_KEY = "monster_light_armor";
const std::string MONSTER_HEAVY_ARMOR_KEY = "monster_heavy_armor";

//enemies
const std::string INFANTRY_KEY = "infantry";
const std::string CAR_KEY = "car";
const std::string TRICYCLE_KEY = "tricycle";
const std::string SMALL_KEY = "small";
const std::string SELFHEAL_KEY = "selfheal";
const std::string TRACTOR_KEY = "tractor";
const std::string ANOTHER_KEY = "another";
const std::string TANK_KEY = "tank";
const std::string SPIDER_KEY = "spider";
const std::string MID_KEY = "mid";
const std::string REPAIR_KEY = "repair";
const std::string SHIELD_KEY = "shield";
const std::string TELEPORT_KEY = "teleport";
const std::string BIG_KEY = "big";
const std::string BIG_TANK_KEY = "big_tank";
const std::string SPAWN_KEY = "spawn";
const std::string JUMPER_KEY = "jumper";
const std::string ROLLER_KEY = "roller";

//enemy stats
const std::string SPEED_KEY = "speed";
const std::string HEALTH_KEY = "health";
const std::string LOSS_KEY = "damage";
const std::string REFLECTION_KEY = "reflection";
const std::string ARMOR_TYPE_KEY = "armor_type";

//limits
const std::string BASE_LIMIT_KEY = "base_max";
const std::string FREEZE_LIMIT_KEY = "freeze_max";
const std::string POWER_LIMIT_KEY = "power_max";
const std::string ROCKET_LIMIT_KEY = "rocket_max";
const std::string LASER_LIMIT_KEY = "laser_max";
const std::string IMPROVED_LIMIT_KEY = "improved_max";

//others
const std::string UPGRADE_GAIN_KEY = "upgrade_gain";
const std::string POWER_TOWER_COST_OFFSET_KEY = "power_tower_cost_offset";
const std::string BLAST_CELLS_KEY = "blast_cells";
const std::string BLAST_COUNT_KEY = "blast_count";
const std::string BASE_TOWER_PENETRATION_KEY = "base_tower_penetration";
const std::string FREEZE_TOWER_VALUE_KEY = "freeze_tower_value";
const std::string FREEZE_TOWER_CELLS_KEY = "freeze_tower_cells";
const std::string FREEZE_TOWER_DURATION_KEY = "freeze_tower_duration";
const std::string LASER_TOWER_MAX_EXTRA_TARGETS_KEY = "laser_tower_max_extra_targets";
const std::string ROCKET_TOWER_CELLS_KEY = "rocker_tower_cells";
const std::string BURN_ATTACK_SPEED_KEY = "burn_attack_speed";
const std::string BURN_DAMAGE_KEY = "burn_damage";
const std::string BURN_DURATION_KEY = "burn_duration";
const std::string DRAIN_VALUE_KEY = "drain_value";
const std::string BLIND_VALUE_KEY = "blind_value";
const std::string REGRESS_VALUE_KEY = "regress_value";

//abilities
const std::string BOMB_DAMAGE_KEY = "bomb_damage";
const std::string BOMB_COOLDOWN_KEY = "bomb_cooldown";
const std::string FREEZE_VALUE_KEY = "freeze_value";
const std::string FREEZE_DURATION_KEY = "freeze_duration";
const std::string FREEZE_COOLDOWN_KEY = "freeze_cooldown";
const std::string ACID_DAMAGE_KEY = "acid_damage";
const std::string ACID_ATTACK_SPEED_KEY = "acid_attack_speed";
const std::string ACID_COUNT_KEY = "acid_count";
const std::string ACID_COOLDOWN_KEY = "acid_cooldown";
const std::string INCREASE_ATTACK_SPEED_DURATION_KEY = "increase_attack_speed_duration";
const std::string INCREASE_ATTACK_SPEED_VALUE_KEY = "increase_attack_speed_value";
const std::string INCREASE_ATTACK_SPEED_COOLDOWN_KEY = "increase_attack_speed_cooldown";
const std::string INCREASE_DAMAGE_DURATION_KEY = "increase_damage_duration";
const std::string INCREASE_DAMAGE_VALUE_KEY = "increase_damage_value";
const std::string INCREASE_DAMAGE_COOLDOWN_KEY = "increase_damage_cooldown";
const std::string STOP_DURATION_KEY = "stop_duration";
const std::string STOP_COOLDOWN_KEY = "stop_cooldown";

//maps
const std::string MAP_KEY = "map#";
const std::string MAP_LIFE_KEY = "map_life";
const std::string MAP_MONEY_KEY = "map_money";
const std::string MAP_DRAIN_KEY = "map_drain";
const std::string MAP_EXPLOSIONS_KEY = "map_explosions";
const std::string MAP_SMOKE_KEY = "map_smoke";
const std::string MAP_REGRESS_KEY = "map_regress";
const std::string MAP_LAVA_KEY = "map_lava";

const std::string MAP_EFFECT_ENABLED_KEY = "map_effect_enabled";
const std::string MAP_EFFECT_DURATION_KEY = "map_effect_duration";
const std::string MAP_EFFECT_TIME_KEY = "map_effect_time";
const std::string MAP_EFFECT_COUNT_KEY = "map_effect_count";

//enemies abilities
const std::string SELFHEAL_INTERVAL_KEY = "selfheal_interval";
const std::string SELFHEAL_VALUE_KEY = "selfheal_value";
const std::string HEAL_INTERVAL_KEY = "heal_interval";
const std::string HEAL_VALUE_KEY = "heal_value";
const std::string SHIELD_INTERVAL_KEY = "shield_interval";
const std::string SHIELD_VALUE_KEY = "shield_value";
const std::string STRONG_INTERVAL_KEY = "strong_interval";
const std::string STRONG_VALUE_KEY = "strong_value";
const std::string TELEPORT_INTERVAL_KEY = "teleport_interval";
const std::string SPAWN_COUNT_KEY = "spawn_count";
const std::string SPAWN_COUNT_OFFSET_KEY = "spawn_count_offset";
const std::string SPAWN_INTERVAL_KEY = "spawn_interval";
const std::string SHUTDOWN_INTERVAL_KEY = "shutdown_interval";
const std::string SHUTDOWN_DURATION_KEY = "shutdown_duration";
const std::string SHUTDOWN_CELLS_KEY = "shutdown_cells";
const std::string DOWNGRADE_VALUE_KEY = "downgrade_value";
const std::string DOWNGRADE_CELLS_KEY = "downgrade_cells";
const std::string DOWNGRADE_DURATION_KEY = "downgrade_durartion";
const std::string DOWNGRADE_INTERVAL_KEY = "downgrade_interval";
const std::string KILL_TOWER_INTERVAL_KEY = "kill_tower_interval";
const std::string DOWNGRADE_TOWER_INTERVAL_KEY = "downgrade_tower_interval";
const std::string KILL_TOWER_CELLS_KEY = "kill_tower_cells";
const std::string DOWNGRADE_TOWER_CELLS_KEY = "downgrade_tower_cells";
const std::string JUMP_INTERVAL_KEY = "jump_interval";
const std::string ROLL_INTERVAL_KEY = "roll_interval";

//waves
const std::string PROTECTION_KEY = "protection";
const std::string RESPAWN_TIME_KEY = "respawn_time";
const std::string SPAWN_ENEMIES_KEY = "spawn_enemies";
const std::string ENEMY_TYPE_KEY = "enemy_type";
const std::string ENEMIES_COUNT_KEY = "enemies_count";
};

#endif // BALANCE_DEF_H

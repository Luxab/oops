#ifndef DEFINITIONS_H
#define DEFINITIONS_H

class Projectile;
class Enemy;
class PowerUp;

typedef std::vector<int> int_vec;
typedef std::unordered_map<int, Projectile*> proj_map;
typedef std::unordered_map<int, Enemy*> enemy_map;
typedef std::unordered_map<int, PowerUp*> pow_map;

#endif

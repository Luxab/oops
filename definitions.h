#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <vector>
#include <map>

class Projectile;
class Enemy;

typedef std::vector<int> int_vec;
typedef std::unordered_map<int, Projectile*> proj_map;
typedef std::unordered_map<int, Enemy*> enemy_map;

#endif

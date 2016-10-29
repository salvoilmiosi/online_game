#ifndef __GAME_MAP_H__
#define __GAME_MAP_H__

#include <vector>
#include <map>

#include <packet_io.h>

enum tile_type {
    TILE_FLOOR,
    TILE_SPAWN,
    TILE_WALL,
    TILE_BREAKABLE,
    TILE_SPECIAL,
};

enum map_zone {
    ZONE_RANDOM,
    ZONE_NORMAL,
    ZONE_WESTERN,
    ZONE_BOMB,
    ZONE_JUMP,
    ZONE_BELT,
    ZONE_DUEL,
    ZONE_POWER,
    ZONE_SPEED,
};

enum special_type {
    SPECIAL_NONE,
    SPECIAL_TRAMPOLINE,
    SPECIAL_BELT,
    SPECIAL_ITEM_SPAWNER,
};

static const int MAP_WIDTH = 17;
static const int MAP_HEIGHT = 13;

static const int TILE_SIZE = 100;

struct tile {
    tile_type type;
    uint16_t data;
};

struct point {
    int x, y;
};

class tile_entity {
private:
    const special_type type;

protected:
    tile *t_tile;
    class game_map *g_map;

public:
    tile_entity(const special_type type, tile *t_tile, game_map *g_map) : type(type), t_tile(t_tile), g_map(g_map) {}

    virtual ~tile_entity() {}

public:
    virtual void tick() = 0;

    const uint8_t getType() {
        return type;
    }

    virtual bool isWalkable() {
        return true;
    }

    virtual bool bombHit() {
        return false;
    }

protected:
    void setData(uint16_t data) {
        t_tile->type = TILE_SPECIAL;
        t_tile->data = ((type & 0x7) << 13) | (data & 0x1fff);
    }
};

class game_map {
private:
    tile *tiles = nullptr;

    int width = 0;
    int height = 0;

    map_zone zone;

    std::vector<point> spawn_pts;

    std::map<tile *, tile_entity *> specials;

    class game_world *world;

public:
    game_map(game_world *world);
    virtual ~game_map();

public:
    void createMap(int w, int h, int num_players, map_zone zone = ZONE_RANDOM);
    void clear();

    void tick();

    uint8_t getZone() const {
        return zone;
    }

    tile *getTile(int x, int y) const;

    int getTileX(tile *t) const;
    int getTileY(tile *t) const;

    tile_entity *getSpecial(tile *t) const;

    int getWidth() const {
        return width;
    }

    int getHeight() const {
        return height;
    }

    game_world *getWorld() {
        return world;
    }

    point getSpawnPt(unsigned int numt);

    void writeToPacket(packet_ext &packet);

private:
    void createDuelMap();
};

#endif // __GAME_MAP_H__

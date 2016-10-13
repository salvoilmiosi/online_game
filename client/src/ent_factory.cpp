#include "packet_io.h"
#include "game_world.h"

#include "player.h"
#include "ent_bomb.h"
#include "ent_broken_wall.h"
#include "ent_item.h"

#include "tile_trampoline.h"

entity *entity::newObjFromPacket(game_world *world, uint16_t id, packet_ext &packet) {
    uint8_t type = packet.readChar();
    byte_array ba = packet.readByteArray();
    switch(type) {
    case TYPE_PLAYER:
        return new player(world, id, ba);
    case TYPE_BOMB:
        return new bomb(world, id, ba);
    case TYPE_EXPLOSION:
        return new explosion(world, id, ba);
    case TYPE_BROKEN_WALL:
        return new broken_wall(world, id, ba);
    case TYPE_ITEM:
        return new game_item(world, id, ba);
    default:
        return nullptr;
    }
}

tile_entity *tile_entity::newTileEntity(tile *t_tile) {
    uint8_t type = (t_tile->data & 0xe0) >> 5;

    switch (type) {
    case SPECIAL_TRAMPOLINE:
        return new tile_trampoline(t_tile);
    default:
        return nullptr;
    }
}
#include "CharacterUtils.h"
#include "MapUtils.h"
#include <Database/Cache/CharacterDatabaseCache.h>
#include "../Connections/WorldConnection.h"

#include "../ECS/Components/Singletons/SingletonComponent.h"
#include "../ECS/Components/PlayerConnectionComponent.h"
#include "../ECS/Components/PlayerPositionComponent.h"

void CharacterUtils::GetDisplayIdFromRace(u8 raceId, u8 genderId, u32& displayId)
{
    switch (raceId)
    {
    // Human
    case 1:
    {
        displayId = 49 + genderId;
        break;
    }
    // Orc
    case 2:
    {
        displayId = 51 + genderId;
        break;
    }
    // Dwarf
    case 3:
    {
        displayId = 53 + genderId;
        break;
    }
    // Night Elf
    case 4:
    {
        displayId = 55 + genderId;
        break;
    }
    // Undead
    case 5:
    {
        displayId = 57 + genderId;
        break;
    }
    // Tauren
    case 6:
    {
        displayId = 59 + genderId;
        break;
    }
    // Gnome
    case 7:
    {
        displayId = 1563 + genderId;
        break;
    }
    // Gnome
    case 8:
    {
        displayId = 1478 + genderId;
        break;
    }
    // Blood Elf
    case 10:
    {
        displayId = 15476 - genderId;
        break;
    }
    // Draenie
    case 11:
    {
        displayId = 16125 + genderId;
        break;
    }
    }
}
u8 CharacterUtils::GetLastMovementTimeIndexFromOpcode(u16 opcode)
{
    u8 opcodeIndex = 0;
    switch (opcode)
    {
    case Opcode::MSG_MOVE_STOP:
    {
        opcodeIndex = 0;
        break;
    }
    case Opcode::MSG_MOVE_STOP_STRAFE:
    {
        opcodeIndex = 1;
        break;
    }
    case Opcode::MSG_MOVE_STOP_TURN:
    {
        opcodeIndex = 2;
        break;
    }
    case Opcode::MSG_MOVE_STOP_PITCH:
    {
        opcodeIndex = 3;
        break;
    }
    case Opcode::MSG_MOVE_START_FORWARD:
    {
        opcodeIndex = 4;
        break;
    }
    case Opcode::MSG_MOVE_START_BACKWARD:
    {
        opcodeIndex = 5;
        break;
    }
    case Opcode::MSG_MOVE_START_STRAFE_LEFT:
    {
        opcodeIndex = 6;
        break;
    }
    case Opcode::MSG_MOVE_START_STRAFE_RIGHT:
    {
        opcodeIndex = 7;
        break;
    }
    case Opcode::MSG_MOVE_START_TURN_LEFT:
    {
        opcodeIndex = 8;
        break;
    }
    case Opcode::MSG_MOVE_START_TURN_RIGHT:
    {
        opcodeIndex = 9;
        break;
    }
    case Opcode::MSG_MOVE_START_PITCH_UP:
    {
        opcodeIndex = 10;
        break;
    }
    case Opcode::MSG_MOVE_START_PITCH_DOWN:
    {
        opcodeIndex = 11;
        break;
    }
    case Opcode::MSG_MOVE_START_ASCEND:
    {
        opcodeIndex = 12;
        break;
    }
    case Opcode::MSG_MOVE_STOP_ASCEND:
    {
        opcodeIndex = 13;
        break;
    }
    case Opcode::MSG_MOVE_START_DESCEND:
    {
        opcodeIndex = 14;
        break;
    }
    case Opcode::MSG_MOVE_START_SWIM:
    {
        opcodeIndex = 15;
        break;
    }
    case Opcode::MSG_MOVE_STOP_SWIM:
    {
        opcodeIndex = 16;
        break;
    }
    case Opcode::MSG_MOVE_FALL_LAND:
    {
        opcodeIndex = 17;
        break;
    }
    case Opcode::CMSG_MOVE_FALL_RESET:
    {
        opcodeIndex = 18;
        break;
    }
    case Opcode::MSG_MOVE_JUMP:
    {
        opcodeIndex = 19;
        break;
    }
    case Opcode::MSG_MOVE_SET_FACING:
    {
        opcodeIndex = 20;
        break;
    }
    case Opcode::MSG_MOVE_SET_PITCH:
    {
        opcodeIndex = 21;
        break;
    }
    case Opcode::MSG_MOVE_SET_RUN_MODE:
    {
        opcodeIndex = 22;
        break;
    }
    case Opcode::MSG_MOVE_SET_WALK_MODE:
    {
        opcodeIndex = 23;
        break;
    }
    case Opcode::CMSG_MOVE_SET_FLY:
    {
        opcodeIndex = 24;
        break;
    }
    case Opcode::CMSG_MOVE_CHNG_TRANSPORT:
    {
        opcodeIndex = 25;
        break;
    }
    case Opcode::MSG_MOVE_HEARTBEAT:
    {
        opcodeIndex = 26;
        break;
    }
    }

    return opcodeIndex;
}

void CharacterUtils::BuildSpeedChangePacket(u64 characterGuid, f32 speed, Opcode opcode, std::shared_ptr<ByteBuffer> buffer)
{
    buffer->PutGuid(characterGuid);
    buffer->PutU32(0);

    /* Convert speed to a multiplicative of base speed */
    if (opcode == Opcode::SMSG_FORCE_WALK_SPEED_CHANGE)
    {
        speed *= 2.5f;
    }
    else if (opcode == Opcode::SMSG_FORCE_RUN_SPEED_CHANGE)
    {
        // Write extra bit added in 2.1.0
        buffer->PutU8(1);
        speed *= 7.1111f;
    }
    else if (opcode == Opcode::SMSG_FORCE_RUN_BACK_SPEED_CHANGE)
    {
        speed *= 4.5f;
    }
    else if (opcode == Opcode::SMSG_FORCE_SWIM_SPEED_CHANGE)
    {
        speed *= 4.722222f;
    }
    else if (opcode == Opcode::SMSG_FORCE_SWIM_BACK_SPEED_CHANGE)
    {
        speed *= 2.5f;
    }
    else if (opcode == Opcode::SMSG_FORCE_FLIGHT_SPEED_CHANGE)
    {
        speed *= 7.1111f;
    }
    else if (opcode == Opcode::SMSG_FORCE_FLIGHT_BACK_SPEED_CHANGE)
    {
        speed *= 4.5f;
    }

    buffer->PutF32(speed);
}
void CharacterUtils::BuildFlyModePacket(u64 characterGuid, std::shared_ptr<ByteBuffer> buffer)
{
    buffer->PutGuid(characterGuid);
    buffer->PutU32(0); // Unk
}

// This function will force the clients position to match what we have in our PlayerPositionComponent, use this after serverside movement
void CharacterUtils::InvalidatePosition(SingletonComponent* singleton, PlayerConnectionComponent* playerConnection, PlayerPositionComponent* playerPositionData)
{
    std::shared_ptr<ByteBuffer> buffer = ByteBuffer::Borrow<101>();
    buffer->PutGuid(playerConnection->characterGuid);
    buffer->PutU32(0); // Teleport Count

    u32 serverTime = static_cast<u32>(singleton->lifeTimeInMS);
    playerPositionData->WriteMovementData(buffer, serverTime);

    u32 clientTime = 0;
    if (playerPositionData->initialServerTime)
    {
        u32 serverTimeDifference = serverTime - playerPositionData->initialServerTime;
        clientTime = playerPositionData->initialClientTime + serverTimeDifference;
    }

    for (i32 i = 0; i < MAX_MOVEMENT_OPCODES; i++)
    {
        playerPositionData->lastMovementOpcodeTime[i] = clientTime;
    }

    playerConnection->socket->SendPacket(buffer.get(), Opcode::MSG_MOVE_TELEPORT_ACK);
}
void CharacterUtils::InvalidatePosition(entt::registry* registry, u32 entityId)
{
    SingletonComponent& singleton = registry->ctx<SingletonComponent>();
    PlayerConnectionComponent& playerConnection = registry->get<PlayerConnectionComponent>(entityId);
    PlayerPositionComponent& playerPositionData = registry->get<PlayerPositionComponent>(entityId);

    InvalidatePosition(&singleton, &playerConnection, &playerPositionData);
}

void CharacterUtils::SendPacketToGridPlayers(entt::registry* registry, u32 entityId, std::shared_ptr<ByteBuffer> buffer, u16 opcode, bool excludeSelf)
{
    PlayerPositionComponent& playerPositionData = registry->get<PlayerPositionComponent>(entityId);
    MapsUtils::SendPacketToGridPlayers(registry, playerPositionData.mapId, playerPositionData.adtId, buffer, opcode, excludeSelf ? entityId : 0);
}
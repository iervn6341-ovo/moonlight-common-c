#pragma once

#include "ByteBuffer.h"
#include "Limelight.h"

#define LI_FILE_TRANSFER_STATUS_PAYLOAD_SIZE \
    (sizeof(uint8_t) + sizeof(uint32_t) + sizeof(uint16_t))

static inline bool LiSerializeFileTransferStatusPayload(char* payload,
                                                        size_t payloadLength,
                                                        uint8_t eventType,
                                                        uint32_t transferId,
                                                        uint16_t status) {
    BYTE_BUFFER bb;

    if (payload == NULL || payloadLength != LI_FILE_TRANSFER_STATUS_PAYLOAD_SIZE ||
            (eventType != LI_FTE_CANCEL && eventType != LI_FTE_ACK)) {
        return false;
    }

    BbInitializeWrappedBuffer(&bb,
                              payload,
                              0,
                              (int)payloadLength,
                              BYTE_ORDER_LITTLE);
    return BbPut8(&bb, eventType) &&
           BbPut32(&bb, transferId) &&
           BbPut16(&bb, status) &&
           bb.position == bb.length;
}

static inline bool LiParseFileTransferStatusPayload(PBYTE_BUFFER bb,
                                                    PLI_FILE_TRANSFER_EVENT event) {
    return BbGet32(bb, &event->transferId) &&
           BbGet16(bb, &event->status) &&
           bb->position == bb->length;
}

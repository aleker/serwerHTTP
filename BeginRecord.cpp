//
// Created by tusia on 16.02.17.
//

#include "BeginRecord.h"

BeginRecord::BeginRecord(int array_size, int type, int request_id) : Record(array_size, type, request_id) {}

void BeginRecord::fillBeginRequestBody(int shift, int role, int flags) {
    message[ROLE_B1 + shift] = (unsigned char) ((role >> 8) & 0xff);
    message[ROLE_B0 + shift] = (unsigned char) ((role) & 0xff);
    message[FLAGS + shift] = (unsigned char) flags;
    for (int i = RESERVED_BEGIN + shift; i < BEGIN_REQUEST_BODY_SIZE + shift; i++) {
        message[i] = 0;
    }
}

void BeginRecord::fill(int role) {
    fillHeader(0, BEGIN_REQUEST_BODY_SIZE);
    fillBeginRequestBody(HEADER_SIZE, role, 0);
}

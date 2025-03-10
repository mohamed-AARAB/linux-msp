/*
 * This file is part of INAV Project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU General Public License Version 3, as described below:
 *
 * This file is free software: you may copy, redistribute and/or modify
 * it under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * This file is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see http://www.gnu.org/licenses/.
 */

#pragma once

#include <stdint.h>

typedef struct __attribute__((packed)) {
    uint8_t quality;    // [0;255]
    int32_t motionX;
    int32_t motionY;
} mspSensorOpflowDataMessage_t;

typedef struct __attribute__((packed)) {
    uint8_t quality;    // [0;255]
    int32_t distanceMm; // Negative value for out of range
} mspSensorRangefinderDataMessage_t;

typedef struct __attribute__((packed)) {
    uint8_t  instance;                  // sensor instance number to support multi-sensor setups
    uint16_t gpsWeek;                   // GPS week, 0xFFFF if not available
    uint32_t msTOW;
    uint8_t  fixType;
    uint8_t  satellitesInView;
    uint16_t horizontalPosAccuracy;     // [cm]
    uint16_t verticalPosAccuracy;       // [cm]
    uint16_t horizontalVelAccuracy;     // [cm/s]
    uint16_t hdop;
    int32_t  longitude;
    int32_t  latitude;
    int32_t  mslAltitude;       // cm
    int32_t  nedVelNorth;       // cm/s
    int32_t  nedVelEast;
    int32_t  nedVelDown;
    uint16_t groundCourse;      // deg * 100, 0..36000
    uint16_t trueYaw;           // deg * 100, values of 0..36000 are valid. 65535 = no data available
    uint16_t year;
    uint8_t  month;
    uint8_t  day;
    uint8_t  hour;
    uint8_t  min;
    uint8_t  sec;
} mspSensorGpsDataMessage_t;

typedef struct __attribute__((packed)) {
    uint8_t  instance;
    uint32_t timeMs;
    float    pressurePa;
    int16_t  temp; // centi-degrees C
} mspSensorBaroDataMessage_t;

typedef struct __attribute__((packed)) {
    uint8_t  instance;
    uint32_t timeMs;
    float    diffPressurePa;
    int16_t  temp;              // centi-degrees C
} mspSensorAirspeedDataMessage_t;

typedef struct __attribute__((packed)) {
    uint8_t  instance;
    uint32_t timeMs;
    int16_t  magX; // mGauss, front
    int16_t  magY; // mGauss, right
    int16_t  magZ; // mGauss, down
} mspSensorCompassDataMessage_t;

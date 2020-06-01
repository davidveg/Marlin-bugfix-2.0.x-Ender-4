/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(SINGLENOZZLE)

#include "../gcode.h"
#include "../../module/tool_change.h"

#if NUM_SERIAL > 1
  #include "../../gcode/queue.h"
#endif

void M217_report(const bool eeprom=false) {
  #if NUM_SERIAL > 1
    const int16_t port = command_queue_port[cmd_queue_index_r];
  #endif
  serialprintPGM_P(port, eeprom ? PSTR("  M217") : PSTR("Singlenozzle:"));
  SERIAL_ECHOPAIR_P(port, " S", LINEAR_UNIT(sn_settings.swap_length));
  SERIAL_ECHOPAIR_P(port, " P", LINEAR_UNIT(sn_settings.prime_speed));
  SERIAL_ECHOPAIR_P(port, " R", LINEAR_UNIT(sn_settings.retract_speed));
  #if ENABLED(SINGLENOZZLE_SWAP_PARK)
    SERIAL_ECHOPAIR_P(port, " X", LINEAR_UNIT(sn_settings.change_point.x));
    SERIAL_ECHOPAIR_P(port, " Y", LINEAR_UNIT(sn_settings.change_point.y));
  #endif
  SERIAL_ECHOPAIR_P(port, " Z", LINEAR_UNIT(sn_settings.z_raise));
  SERIAL_EOL();
}

/**
 * M217 - Set SINGLENOZZLE toolchange parameters
 *
 *  S[linear]   Swap length
 *  P[linear/m] Prime speed
 *  R[linear/m] Retract speed
 *  X[linear]   Park X (Requires SINGLENOZZLE_SWAP_PARK)
 *  Y[linear]   Park Y (Requires SINGLENOZZLE_SWAP_PARK)
 *  Z[linear]   Z Raise
 */
void GcodeSuite::M217() {

  bool report = true;

  if (parser.seenval('S')) { report = false; const float v = parser.value_linear_units(); sn_settings.swap_length = constrain(v, 0, 500); }
  if (parser.seenval('P')) { report = false; const int16_t v = parser.value_linear_units(); sn_settings.prime_speed = constrain(v, 10, 5400); }
  if (parser.seenval('R')) { report = false; const int16_t v = parser.value_linear_units(); sn_settings.retract_speed = constrain(v, 10, 5400); }

  #if ENABLED(SINGLENOZZLE_SWAP_PARK)
    if (parser.seenval('X')) { report = false; sn_settings.change_point.x = parser.value_linear_units(); }
    if (parser.seenval('Y')) { report = false; sn_settings.change_point.y = parser.value_linear_units(); }
  #endif

  if (parser.seenval('Z')) { report = false; sn_settings.z_raise = parser.value_linear_units(); }

  if (report) M217_report();

}

#endif // SINGLENOZZLE

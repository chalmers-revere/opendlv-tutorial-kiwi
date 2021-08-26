/*
 * Copyright (C) 2018 Ola Benderius
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
 */

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "cluon-complete.hpp"
#include "opendlv-standard-message-set.hpp"

#include "behavior.hpp"

TEST_CASE("Test behavior, a short distance to the front should make Kiwi stop.") {
  Behavior b;

  opendlv::proxy::DistanceReading dr;
  dr.distance(0.1f);
  
  b.setFrontUltrasonic(dr);
  b.step();

  auto pp = b.getPedalPositionRequest();

  REQUIRE(pp.position() == Approx(0.0f));
}

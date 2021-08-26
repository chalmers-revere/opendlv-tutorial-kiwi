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

#include "cluon-complete.hpp"
#include "opendlv-standard-message-set.hpp"
#include "behavior.hpp"

int32_t main(int32_t argc, char **argv) {
  int32_t retCode{0};
  auto commandlineArguments = cluon::getCommandlineArguments(argc, argv);
  if (0 == commandlineArguments.count("cid") || 0 == commandlineArguments.count("freq")) {
    std::cerr << argv[0] << " tests the Kiwi platform by sending actuation commands and reacting to sensor input." << std::endl;
    std::cerr << "Usage:   " << argv[0] << " --freq=<Integration frequency> --cid=<OpenDaVINCI session> [--verbose]" << std::endl;
    std::cerr << "Example: " << argv[0] << " --freq=10 --cid=111" << std::endl;
    retCode = 1;
  } else {
    bool const VERBOSE{commandlineArguments.count("verbose") != 0};
    uint16_t const CID = std::stoi(commandlineArguments["cid"]);
    float const FREQ = std::stof(commandlineArguments["freq"]);

    Behavior behavior;

    auto onDistanceReading{[&behavior](cluon::data::Envelope &&envelope)
      {
        auto distanceReading = cluon::extractMessage<opendlv::proxy::DistanceReading>(std::move(envelope));
        uint32_t const senderStamp = envelope.senderStamp();
        if (senderStamp == 0) {
          behavior.setFrontUltrasonic(distanceReading);
        } else if (senderStamp == 1) {
          behavior.setRearUltrasonic(distanceReading);
        }
      }};
    auto onVoltageReading{[&behavior](cluon::data::Envelope &&envelope)
      {
        auto voltageReading = cluon::extractMessage<opendlv::proxy::VoltageReading>(std::move(envelope));
        uint32_t const senderStamp = envelope.senderStamp();
        if (senderStamp == 0) {
          behavior.setLeftIr(voltageReading);
        } else if (senderStamp == 1) {
          behavior.setRightIr(voltageReading);
        }
      }};

    auto onAngleReading{[&behavior](cluon::data::Envelope &&envelope)
      {
        auto angleReading = cluon::extractMessage<opendlv::proxy::AngleReading>(std::move(envelope));
        uint32_t const senderStamp = envelope.senderStamp();
        if (senderStamp == 0) {
          behavior.setAngleReading(angleReading);
        }
      }};

    cluon::OD4Session od4{CID};
    od4.dataTrigger(opendlv::proxy::DistanceReading::ID(), onDistanceReading);
    od4.dataTrigger(opendlv::proxy::VoltageReading::ID(), onVoltageReading);
    od4.dataTrigger(opendlv::proxy::AngleReading::ID(), onAngleReading);

    auto atFrequency{[&VERBOSE, &behavior, &od4]() -> bool
      {
        behavior.step();
        auto groundSteeringAngleRequest = behavior.getGroundSteeringAngle();
        auto pedalPositionRequest = behavior.getPedalPositionRequest();

        cluon::data::TimeStamp sampleTime = cluon::time::now();
        od4.send(groundSteeringAngleRequest, sampleTime, 0);
        od4.send(pedalPositionRequest, sampleTime, 0);
        if (VERBOSE) {
          std::cout << "Ground steering angle is " << groundSteeringAngleRequest.groundSteering()
            << " and pedal position is " << pedalPositionRequest.position() << std::endl;
        }

        return true;
      }};

    od4.timeTrigger(FREQ, atFrequency);
  }
  return retCode;
}

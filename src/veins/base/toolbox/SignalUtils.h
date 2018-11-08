//
// Copyright (C) 2018 Fabian Bronner <fabian.bronner@ccs-labs.org>
//
// Documentation for these modules is at http://veins.car2x.org/
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#ifndef MATHHELPER_H_
#define MATHHELPER_H_

#include <stdint.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>

#include "veins/base/utils/MiXiMDefs.h"
#include "veins/base/phyLayer/DeciderToPhyInterface.h"
#include "veins/base/phyLayer/Decider.h"

#include "veins/base/toolbox/Signal.h"

namespace Veins {
namespace SignalUtils {

using AirFrameVector = DeciderToPhyInterface::AirFrameVector;

double getGlobalMax(simtime_t start, simtime_t end, const AirFrameVector& airFrames);

double getGlobalMin(simtime_t start, simtime_t end, const AirFrameVector& airFrames);
double getMinAtFreqIndex(simtime_t start, simtime_t end, const AirFrameVector& airFrames, size_t freqIndex, AirFrame* exclude);

bool smallerAtFreqIndex(simtime_t start, simtime_t end, AirFrameVector& airFrames, size_t freqIndex, double threshold, AirFrame* exclude = 0);

double getMinSINR(simtime_t start, simtime_t end, AirFrame* signalFrame, AirFrameVector& interfererFrames, double noise);

} // namespace SignalUtils
} // namespace Veins

#endif /* MATHHELPER_H_ */

//
// Copyright (C) 2006-2018 Christoph Sommer <sommer@ccs-labs.org>
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

/**
 * @mainpage %Veins - The open source vehicular network simulation framework.
 *
 * See the %Veins website <a href="http://veins.car2x.org/"> for a tutorial, documentation, and publications </a>.
 */

#pragma once

#include <omnetpp.h>

// Explicitly check OMNeT++ version number
#if OMNETPP_VERSION < 0x500
#error At least OMNeT++/OMNEST version 5.0.0 required
#endif

// Import whole omnetpp namespace
namespace omnetpp {
}
using namespace omnetpp;

// VEINS_API macro. Allows us to use the same .h files for both building a .dll and linking against it
#if defined(VEINS_EXPORT)
#define VEINS_API OPP_DLLEXPORT
#elif defined(VEINS_IMPORT)
#define VEINS_API OPP_DLLIMPORT
#else
#define VEINS_API
#endif

// Macro for marking code as deprecated
#define VEINS_DEPRECATED _OPPDEPRECATED

// Macros for signal names
#define MIXIM_SIGNAL_PACKET_NAME "org.car2x.veins.modules.utility.packet"
#define MIXIM_SIGNAL_UWBIRPACKET_NAME "org.car2x.veins.modules.utility.uwbirpacket"
#define MIXIM_SIGNAL_DROPPEDPACKET_NAME "org.car2x.veins.modules.utility.droppedpacket"
#define MIXIM_SIGNAL_PASSEDMSG_NAME "org.car2x.veins.base.utils.passedmsg"
#define MIXIM_SIGNAL_HOSTSTATE_NAME "org.car2x.veins.base.utils.hoststate"
#define MIXIM_SIGNAL_MOBILITY_CHANGE_NAME "veinsmobilityStateChanged"
#define MIXIM_SIGNAL_BATTERY_CHANGE_NAME "org.car2x.veins.modules.power.battery.batterystate"
#define MIXIM_SIGNAL_MOBANMSG_NAME "org.car2x.veins.modules.utility.bbmobanmessage"

// Convenience macros
#define RNGCONTEXT (cSimulation::getActiveSimulation()->getContext())->

/**
 * %Veins - The open source vehicular network simulation framework.
 */
namespace Veins {
} // namespace Veins

//
// ObstacleControl - models obstacles that block radio transmissions
// Copyright (C) 2010 Christoph Sommer <christoph.sommer@informatik.uni-erlangen.de>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//

#include <set>
#include <limits>
#include "veins/modules/obstacle/VehicleObstacle.h"
#include "veins/modules/mobility/traci/TraCIMobility.h"

using Veins::VehicleObstacle;

namespace {

	bool isPointInObstacle(Coord point, const VehicleObstacle::Coords& shape) {
		bool isInside = false;
		VehicleObstacle::Coords::const_iterator i = shape.begin();
		VehicleObstacle::Coords::const_iterator j = (shape.rbegin()+1).base();
		for (; i != shape.end(); j = i++) {
			bool inYRangeUp = (point.y >= i->y) && (point.y < j->y);
			bool inYRangeDown = (point.y >= j->y) && (point.y < i->y);
			bool inYRange = inYRangeUp || inYRangeDown;
			if (!inYRange) continue;
			bool intersects = point.x < (i->x + ((point.y - i->y) * (j->x - i->x) / (j->y - i->y)));
			if (!intersects) continue;
			isInside = !isInside;
		}
		return isInside;
	}

	double segmentsIntersectAt(Coord p1From, Coord p1To, Coord p2From, Coord p2To) {
		Coord p1Vec = p1To - p1From;
		Coord p2Vec = p2To - p2From;
		Coord p1p2 = p1From - p2From;

		double D = (p1Vec.x * p2Vec.y - p1Vec.y * p2Vec.x);

		double p1Frac = (p2Vec.x * p1p2.y - p2Vec.y * p1p2.x) / D;
		if (p1Frac < 0 || p1Frac > 1) return -1;

		double p2Frac = (p1Vec.x * p1p2.y - p1Vec.y * p1p2.x) / D;
		if (p2Frac < 0 || p2Frac > 1) return -1;

		return p1Frac;
	}

	Coord rotate2d(Coord c, double a) {
		double x = c.x;
		double y = c.y;
		return Coord(x*cos(a) - y*sin(a), x*sin(a) + y*cos(a));
	}
}

VehicleObstacle::Coords VehicleObstacle::getShape() const {
	double l = getLength();
	double o = getAntennaPositionOffset();
	double w = getWidth()/2;
	const TraCIMobility* m = getTraCIMobility();
	Coord p = m->getCurrentPosition();
	double a = m->getAngleRad();

	Coords shape;
	shape.push_back(p + rotate2d(Coord(-(l-o), -w), -a));
	shape.push_back(p + rotate2d(Coord(+o, -w), -a));
	shape.push_back(p + rotate2d(Coord(+o, +w), -a));
	shape.push_back(p + rotate2d(Coord(-(l-o), +w), -a));

	return shape;
}

double VehicleObstacle::getIntersectionPoint(const Coord& senderPos, const Coord& receiverPos) const {
	const double not_a_number = std::numeric_limits<double>::quiet_NaN();

	VehicleObstacle::Coords shape = getShape();

	// shortcut if sender is inside
	bool senderInside = isPointInObstacle(senderPos, shape);
	if (senderInside) return 0;

	// get a list of points (in [0, 1]) along the line between sender and receiver where the beam intersects with this obstacle
	std::multiset<double> intersectAt;
	bool doesIntersect = false;
	VehicleObstacle::Coords::const_iterator i = shape.begin();
	VehicleObstacle::Coords::const_iterator j = (shape.rbegin()+1).base();
	for (; i != shape.end(); j = i++) {
		Coord c1 = *i;
		Coord c2 = *j;

		double inter = segmentsIntersectAt(senderPos, receiverPos, c1, c2);
		if (inter != -1) {
			doesIntersect = true;
			EV << "intersect: " << inter << endl;
			intersectAt.insert(inter);
		}
	}

	// shortcut if no intersections
	if (!doesIntersect) {
		bool receiverInside = isPointInObstacle(receiverPos, shape);
		if (receiverInside) return senderPos.distance(receiverPos);
		return not_a_number;
	}

	return (*intersectAt.begin() * senderPos.distance(receiverPos)) ;
}

//
// Copyright (C) 2018-2018 Max Schettler <max.schettler@ccs-labs.org>
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
#include "veins/modules/utility/TimerManager.h"

using omnetpp::simTime;
using omnetpp::simtime_t;
using Veins::TimerManager;
using Veins::TimerSpecification;

TimerSpecification::TimerSpecification(std::function<void()> callback)
	: start_mode_(StartMode::IMMEDIATE), end_mode_(EndMode::OPEN), period_(-1), callback_(callback) {}

TimerSpecification &TimerSpecification::setInterval(simtime_t interval) {
  ASSERT(interval > 0);
  period_ = interval;
  return *this;
}

TimerSpecification &TimerSpecification::setRelativeStart(simtime_t start) {
  start_mode_ = StartMode::RELATIVE;
  start_ = start;
  return *this;
}

TimerSpecification &TimerSpecification::setAbsoluteStart(simtime_t start) {
  start_mode_ = StartMode::ABSOLUTE;
  start_ = start;
  return *this;
}

TimerSpecification &TimerSpecification::setImmediateStart() {
  start_mode_ = StartMode::IMMEDIATE;
  return *this;
}

TimerSpecification &TimerSpecification::setRelativeEnd(simtime_t end) {
  end_mode_ = EndMode::RELATIVE;
  end_time_ = end;
  return *this;
}

TimerSpecification &TimerSpecification::setAbsoluteEnd(simtime_t end) {
  end_mode_ = EndMode::ABSOLUTE;
  end_time_ = end;
  return *this;
}

TimerSpecification &TimerSpecification::setRepititions(size_t n) {
  end_mode_ = EndMode::REPITITION;
  end_count_ = n;
  return *this;
}

TimerSpecification &TimerSpecification::setOpenEnd() {
  end_mode_ = EndMode::OPEN;
  return *this;
}

TimerSpecification &TimerSpecification::setOneshotIn(omnetpp::simtime_t in) {
	return this->setRelativeStart(in).setInterval(1).setRepititions(1);
}

TimerSpecification &TimerSpecification::setOneshotAt(omnetpp::simtime_t at) {
  return this->setAbsoluteStart(at).setInterval(1).setRepititions(1);
}

void TimerSpecification::finalize() {
  switch (start_mode_) {
  case StartMode::RELATIVE:
    start_ += simTime();
    start_mode_ = StartMode::ABSOLUTE;
    break;
  case StartMode::ABSOLUTE:
    break;
  case StartMode::IMMEDIATE:
    start_ = simTime() + period_;
    break;
  }

  switch (end_mode_) {
  case EndMode::RELATIVE:
    end_time_ += simTime();
    end_mode_ = EndMode::ABSOLUTE;
    break;
  case EndMode::ABSOLUTE:
    break;
  case EndMode::REPITITION:
    end_time_ = start_ + ((end_count_ - 1) * period_);
    end_mode_ = EndMode::ABSOLUTE;
  case EndMode::OPEN:
    break;
  }
}

bool TimerSpecification::validOccurence(simtime_t time) const {
	return time >= start_ && (time <= end_time_ || end_mode_ == EndMode::OPEN) && omnetpp::fmod(time - start_, period_) == 0;
}

TimerManager::TimerManager(omnetpp::cSimpleModule *parent) : parent_(parent) { ASSERT(parent_); }

TimerManager::~TimerManager() {
  for (const auto &timer : timers_) {
    parent_->cancelAndDelete(timer.first);
  }
}

bool TimerManager::handleMessage(omnetpp::cMessage *message) {
  auto *timerMessage = dynamic_cast<TimerManager::TimerMessage *>(message);
  if (!timerMessage) {
    return false;
  }
  ASSERT(timerMessage->isSelfMessage());
  std::string s = timerMessage->getName();

  auto timer = timers_.find(timerMessage);
  if (timer == timers_.end()) {
    return false;
  }
  ASSERT(timer->second.valid() && timer->second.validOccurence(simTime()));

  timer->second.callback_();

  const auto next_event = simTime() + timer->second.period_;
  if (timer->second.validOccurence(next_event)) {
    parent_->scheduleAt(next_event, timer->first);
  } else {
    timers_.erase(timer);
  }

  return true;
}

TimerManager::TimerMessage *TimerManager::create(TimerSpecification timerSpecification, const std::string name) {
  ASSERT(timerSpecification.valid());
  timerSpecification.finalize();

  const auto ret = timers_.insert(std::make_pair(new TimerManager::TimerMessage(name), timerSpecification));
  ASSERT(ret.second);
  parent_->scheduleAt(ret.first->second.start_, ret.first->first);

  return ret.first->first;
}

void TimerManager::cancel(TimerManager::TimerMessage *timerMessage) {
  auto timer = timers_.find(timerMessage);
  if (timer != timers_.end()) {
    parent_->cancelEvent(timer->first);
    timers_.erase(timer);
  }
}

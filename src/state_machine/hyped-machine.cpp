
/*
 * Authors: Yash Mittal, Ragnor Comerford and Calum McMeekin
 * Organisation: HYPED
 * Date:
 * Description:
 *
 *    Copyright 2019 HYPED
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#include "state_machine/hyped-machine.hpp"

namespace hyped {
namespace state_machine {

HypedMachine::HypedMachine(utils::Logger& log)
    : current_state_(State::alloc_)
    , log_(log)
{
  log_.INFO("STATE", "State Machine initialised");
  transition(new(current_state_) Idle());
}

void HypedMachine::handleEvent(Event event)
{
  log_.DBG1("STATE", "Raised event %d", event);
  current_state_->react(*this, event);
}

void HypedMachine::transition(State *state)
{
  // NOTE, no use of argument state, as all react() functions allocate all new
  // states directly to current_state_ variable through common State::alloc_ pointer
  current_state_->entry();
  log_.INFO("STATE", "Transitioned to %s"
    , data::states[current_state_->state_]);
  state_machine_.current_state = current_state_->state_;

  // update shared data structure
  static data::Data& d = data::Data::getInstance();
  d.setStateMachineData(state_machine_);
}

}}   // namespace hyped::state_machine

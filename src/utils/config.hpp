/*
 * Authors: M. Kristien
 * Organisation: HYPED
 * Date: 3. April 2019
 * Description:
 *
 * This a configuration class that should hold all necessary data to configure
 * different submodules in the system.
 * E.g. for telemetry, IP address to be used can be configured
 *
 * Configuration values are read from a configuration file provided.
 *
 *    Copyright 2018 HYPED
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
#ifndef UTILS_CONFIG_HPP_
#define UTILS_CONFIG_HPP_

#include <string>
#include <vector>
#include "data/data.hpp"
#include "utils/interfaces.hpp"

namespace hyped {
// forward declare all known interfaces
#define FORWARD_DECLARE(module, interface) \
  namespace module { class interface; }
INTERFACE_LIST(FORWARD_DECLARE)

namespace utils {

// forward declaration
class System;
class Logger;
struct ModuleEntry;

/**
 * This is an example of automatic core generation using preprocessor macros.
 * First, a list of modules is defined. The list takes and argument V, which will be another macro.
 * As part of the main list definition, relevant information can be passed to the V argument.
 * This can be several values for each entry, e.g. variable name, type, access level,...
 * In this case, it is just the name of the module.
 *
 * Later, we define the macro V to perform the code generation for us.
 * For example, it is used right here to define enums for the modules, see CREATE_ENUM.
 * It is also used in the Config class declaration to declare the parsing method for each
 * module, see DECLARE_PARSE.
 * Later, in .cpp file, it is used to define module map entries, see MAP_ENTRY.
 */
#define MODULE_LIST(V)  \
  V(NoModule)           \
  V(Navigation)         \
  V(StateMachine)       \
  V(Telemetry)          \
  V(Embrakes)           \
  V(Sensors)            \
  V(Factory)

#define CREATE_ENUM(module) \
  k##module,

enum  Submodule {
  MODULE_LIST(CREATE_ENUM)
};

class Config {
  friend System;    // ensure System can access our private parts
  friend ModuleEntry;

 public:
  struct Navigation {
    // define all navigation configuration fields
  } navigation;

  struct StateMachine {
    int timeout;
  } statemachine;

  struct Telemetry {
    std::string IP;
    std::string Port;
  } telemetry;

  struct Embrakes {
    int command[4];
    int button[4];
  } embrakes;

  struct Sensors {
    int chip_select[data::Sensors::kNumImus];
    int KeyenceL;
    int KeyenceR;
    int Thermistor;
    int hp_master;
    int HPSSR[data::Batteries::kNumHPBatteries];
    int IMDOut;
    int embrakes;
  } sensors;

  struct Factory {
#define CREATOR_FUNCTIONS(module, interface) \
  module::interface* (*get##interface)() = 0;
  INTERFACE_LIST(CREATOR_FUNCTIONS)
  } factory;

//  private:
#define DECLARE_PARSE(module) \
  void Parse##module(char* line);

  MODULE_LIST(DECLARE_PARSE)
  // void ParseNavigation(char* line);
  // void ParseStateMachine(char* line);
  // void ParseTelemetry(char* line);
  // void ParseEmbrakes(char* line);
  // void ParseSensors(char* line);
  // void ParseNoModule(char* line);

 private:
  explicit Config(char* config_file);
  Config();
  explicit Config(Config const&) = delete;
  Config& operator=(Config const&) = delete;
  ~Config();

  void readFile(char* config_file);   // recursively called for nested configs

  std::vector<char*> config_files_;
  Logger& log_;
};

}}  // namespace hyped::utils

#endif  // UTILS_CONFIG_HPP_

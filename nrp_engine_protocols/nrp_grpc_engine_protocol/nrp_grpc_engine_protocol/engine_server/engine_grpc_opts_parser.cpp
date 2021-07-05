//
// NRP Core - Backend infrastructure to synchronize simulations
//
// Copyright 2020-2021 NRP Team
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This project has received funding from the European Union’s Horizon 2020
// Framework Programme for Research and Innovation under the Specific Grant
// Agreement No. 945539 (Human Brain Project SGA3).
//

#include "nrp_grpc_engine_protocol/engine_server/engine_grpc_opts_parser.h"
<<<<<<< HEAD
#include "nrp_general_library/utils/nrp_logger.h"

cxxopts::Options EngineGRPCOptsParser::createOptionParser(bool allowUnrecognised)
{
	NRP_LOGGER_TRACE("{} called", __FUNCTION__);

=======

cxxopts::Options EngineGRPCOptsParser::createOptionParser(bool allowUnrecognised)
{
>>>>>>> 0c552da4cd6b3368efa7cf51b04f1c46ad2e2283
	cxxopts::Options options("NRP Gazebo Server");

	if(allowUnrecognised)
		options.allow_unrecognised_options();

	options.add_options()
	        (EngineGRPCConfigConst::EngineServerAddrArg.data(), "NRP URL Address", cxxopts::value<std::string>())
	        (EngineGRPCConfigConst::EngineNameArg.data(), "NRP Engine Name", cxxopts::value<std::string>()->default_value(""));

	return options;
}

cxxopts::ParseResult EngineGRPCOptsParser::parseOpts(int argc, char *argv[], cxxopts::Options parser)
{
<<<<<<< HEAD
	NRP_LOGGER_TRACE("{} called", __FUNCTION__);
	
=======
>>>>>>> 0c552da4cd6b3368efa7cf51b04f1c46ad2e2283
	return parser.parse(argc, argv);
}

#cd nrp-core
cd build

cd nrp_simulation_json_engines/nrp_opensim_json_engine
source test_env.sh # or ". test_env.sh"
echo "Env"
./NRPOpensimJSONEngineTests

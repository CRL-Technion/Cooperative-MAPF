/* Copyright (C) Jiaoyang Li
* Unauthorized copying of this file, via any medium is strictly prohibited
* Confidential
* Written by Jiaoyang Li <jiaoyanl@usc.edu>, March 2021
*/

/*driver.cpp
* Solve a MAPF instance on 2D grids.
*/
#include <boost/program_options.hpp>
#include <boost/tokenizer.hpp>
#include "CoCBS.h"


/* Main function */
int main(int argc, char** argv)
{
	namespace po = boost::program_options;
	// Declare the supported options.
	po::options_description desc("Allowed options");
	desc.add_options()
		("help", "produce help message")

		// params for the input instance and experiment settings
		("map,m", po::value<string>()->required(), "input file for map")
		("agents,a", po::value<string>()->required(), "input file for agents")
		("output,o", po::value<string>(), "output file for schedule")
		("taskNum,k", po::value<int>()->default_value(0), "number of tasks")
		("cutoffTime,t", po::value<double>()->default_value(60), "cutoff time (seconds)")
		("nodeLimit", po::value<int>()->default_value(MAX_NODES), "node limit")
		("screen,s", po::value<int>()->default_value(1), "screen option (0: none; 1: results; 2:all)")
		("seed,d", po::value<int>()->default_value(0), "random seed")
		("stats", po::value<bool>()->default_value(false), "write to files some statistics")
		// ("agentIdx", po::value<string>()->default_value(""), "customize the indices of the agents (e.g., \"0,1\")")

		// params for instance generators
		("rows", po::value<int>()->default_value(0), "number of rows")
		("cols", po::value<int>()->default_value(0), "number of columns")
		("obs", po::value<int>()->default_value(0), "number of obstacles")
		("warehouseWidth", po::value<int>()->default_value(0), "width of working stations on both sides, for generating instances")

		// params for CBS
		// ("heuristics", po::value<string>()->default_value("WDG"), "heuristics for the high-level search (Zero, CG,DG, WDG)")
		("prioritizingConflicts", po::value<bool>()->default_value(false), "conflict priortization. If true, conflictSelection is used as a tie-breaking rule.")
		// ("bypass", po::value<bool>()->default_value(true), "Bypass1")
		// ("disjointSplitting", po::value<bool>()->default_value(false), "disjoint splitting")
		// ("rectangleReasoning", po::value<string>()->default_value("GR"), "rectangle reasoning strategy (None, R, RM, GR, Disjoint)")
		// ("corridorReasoning", po::value<string>()->default_value("GC"), " corridor reasoning strategy (None, C, PC, STC, GC, Disjoint")
		// ("mutexReasoning", po::value<bool>()->default_value(false), "Using mutex reasoning")
		// ("targetReasoning", po::value<bool>()->default_value(true), "Using target reasoning")
		// ("restart", po::value<int>()->default_value(1), "number of restart times (at least 1)")
		// ("sipp", po::value<bool>()->default_value(false), "using sipp as the single agent solver")
		("lazyExpansion", po::value<bool>()->default_value(false), "lazy expansion of root nodes")
		// ("rootTieBreaker", po::value<string>()->default_value("Sum"), "tie breaker for root nodes")
		("taskAssignment", po::value<string>()->default_value("None"), "task assignment type (None, Greedy, Single, Full)")
		("num_assignments,nm", po::value<int>()->default_value(1), "number of assignments to search, when using 'Single' or 'Full' for task assignment type")
		// ("run_baseline,b", po::value<bool>()->default_value(false), "run a greedy baseline algorithm")
		;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);

	if (vm.count("help"))
	{
		cout << desc << endl;
		return 1;
	}

	po::notify(vm);
	/////////////////////////////////////////////////////////////////////////
	/// check the correctness and consistence of params
    //////////////////////////////////////////////////////////////////////
	heuristics_type h = heuristics_type::ZERO; // no heuristics
	// if (vm["heuristics"].as<string>() == "Zero")
	// 	h = heuristics_type::ZERO;
	// else if (vm["heuristics"].as<string>() == "CG")
	// 	h = heuristics_type::CG;
	// else if (vm["heuristics"].as<string>() == "DG")
	// 	h = heuristics_type::DG;
	// else if (vm["heuristics"].as<string>() == "WDG")
	// 	h = heuristics_type::WDG;
	// else
	// {
	// 	cout << "WRONG heuristics strategy!" << endl;
	// 	return -1;
	// }

	rectangle_strategy r = rectangle_strategy::NR; // no rectangle reasoning
	// if (vm["rectangleReasoning"].as<string>() == "None")
	// 	r = rectangle_strategy::NR;  // no rectangle reasoning
	// else if (vm["rectangleReasoning"].as<string>() == "R")
	// 	r = rectangle_strategy::R;  // rectangle reasoning for entire paths
	// else if (vm["rectangleReasoning"].as<string>() == "RM")
	// 	r = rectangle_strategy::RM;  // rectangle reasoning for path segments
    // else if (vm["rectangleReasoning"].as<string>() == "GR")
    //     r = rectangle_strategy::GR;  // generalized rectangle reasoning
	// else if (vm["rectangleReasoning"].as<string>() == "Disjoint")
	// 	r = rectangle_strategy::DR; // disjoint rectangle reasoning
	// else
	// {
	// 	cout << "WRONG rectangle reasoning strategy!" << endl;
	// 	return -1;
	// }

	corridor_strategy c = corridor_strategy::NC;  // no corridor reasoning
	// if (vm["corridorReasoning"].as<string>() == "None")
	// 	c = corridor_strategy::NC;  // no corridor reasoning
	// else if (vm["corridorReasoning"].as<string>() == "C")
	// 	c = corridor_strategy::C;  // corridor reasoning
    // else if (vm["corridorReasoning"].as<string>() == "PC")
    //     c = corridor_strategy::PC;  // corridor + pseudo-corridor reasoning
    // else if (vm["corridorReasoning"].as<string>() == "STC")
    //     c = corridor_strategy::STC;  // corridor with start-target reasoning
    // else if (vm["corridorReasoning"].as<string>() == "GC")
    //     c = corridor_strategy::GC;  // generalized corridor reasoning = corridor with start-target + pseudo-corridor
	// else if (vm["corridorReasoning"].as<string>() == "Disjoint")
	// 	c = corridor_strategy::DC; // disjoint corridor reasoning
	// else
	// {
	// 	cout << "WRONG corridor reasoning strategy!" << endl;
	// 	return -1;
	// }

	root_tie_breaker_type root_tb_type = root_tie_breaker_type::NONE;  // no root tie breaking
	// if (vm["rootTieBreaker"].as<string>() == "None")
	// 	root_tb_type = root_tie_breaker_type::NONE;  // no root tie breaking
	// else if (vm["rootTieBreaker"].as<string>() == "Min")
	// 	root_tb_type = root_tie_breaker_type::MIN;  // maximum minimum clearance
	// else if (vm["rootTieBreaker"].as<string>() == "Sum")
	// 	root_tb_type = root_tie_breaker_type::SUM;  // maximum sum of clearances
	// else
	// {
	// 	cout << "WRONG root tie breaking type!" << endl;
	// 	return -1;
	// }

	search_assignments ta_type;
	if (vm["taskAssignment"].as<string>() == "None")
		ta_type = search_assignments::NOTA;  // no task assignment
	else if (vm["taskAssignment"].as<string>() == "Greedy")
		ta_type = search_assignments::GREEDY;  // greedy task assignment
	else if (vm["taskAssignment"].as<string>() == "Single") {
		ta_type = search_assignments::SINGLE;  // single task assignments
		if (vm["num_assignments"].as<int>() <= 0) {
			cout << "num_assignments must be greater than zero!" << endl;
			return -1;
		}
	}
	else if (vm["taskAssignment"].as<string>() == "Full") {
		ta_type = search_assignments::FULL;  // full task assignments
		if (vm["num_assignments"].as<int>() <= 0) {
			cout << "num_assignments must be greater than zero!" << endl;
			return -1;
		}
	}
	else
	{
		cout << "WRONG task assignment type!" << endl;
		return -1;
	}

	///////////////////////////////////////////////////////////////////////////
	/// load the instance
    //////////////////////////////////////////////////////////////////////
	int agentNum = vm["taskNum"].as<int>() * 2;
	string map_name = vm["map"].as<string>();
	string folder = "benchmarks/" + map_name.erase(map_name.find(".map")) + "/";
	string map = folder + map_name + ".map";
	string agents = folder + vm["agents"].as<string>();
	Instance instance(map, agents,
		agentNum, vm["taskNum"].as<int>(), "", 
		vm["rows"].as<int>(), vm["cols"].as<int>(), vm["obs"].as<int>(), vm["warehouseWidth"].as<int>());

	srand(vm["seed"].as<int>());

	int runs = 1; // vm["restart"].as<int>();


	//////////////////////////////////////////////////////////////////////
	/// initialize the solver
    //////////////////////////////////////////////////////////////////////
	CoCBS cocbs(instance, false, vm["screen"].as<int>());
	cocbs.setPrioritizeConflicts(vm["prioritizingConflicts"].as<bool>());
	cocbs.setDisjointSplitting(false);
	cocbs.setBypass(false);
	cocbs.setRectangleReasoning(r);
	cocbs.setCorridorReasoning(c);
	cocbs.setHeuristicType(h);
	cocbs.setTargetReasoning(false);
	cocbs.setMutexReasoning(false);
	cocbs.setSavingStats(vm["stats"].as<bool>());
	cocbs.setNodeLimit(vm["nodeLimit"].as<int>());
	cocbs.setLazyExpansion(vm["lazyExpansion"].as<bool>());
	cocbs.setRootTbType(root_tb_type);
	cocbs.setTaskAssignment(ta_type);
	cocbs.setNumAssignments(vm["num_assignments"].as<int>());
	cocbs.setRunBaseline(false);


	//////////////////////////////////////////////////////////////////////
	/// run
    //////////////////////////////////////////////////////////////////////
	double runtime = 0;
	int min_f_val = 0;
	for (int i = 0; i < runs; i++)
	{
		cocbs.clear();
		cocbs.solve(vm["cutoffTime"].as<double>(), min_f_val);
		runtime += cocbs.runtime;
		if (cocbs.solution_found)
			break;
		min_f_val = (int) cocbs.min_f_val;
		cocbs.randomRoot = true;
	}
	cocbs.runtime = runtime;

    //////////////////////////////////////////////////////////////////////
    /// write results to files
    //////////////////////////////////////////////////////////////////////
	if (vm.count("output"))
		cocbs.saveResults(vm["output"].as<string>(), vm["agents"].as<string>());
	// cocbs.saveCT(vm["output"].as<string>() + ".tree"); // for debug
	if (vm["stats"].as<bool>())
	{
		cocbs.saveStats(vm["output"].as<string>(), vm["agents"].as<string>());
	}

	// if (vm["run_basaeline"].as<bool>())
	// 	delete cocbs.goal_node;

	cocbs.clearSearchEngines();
	return 0;

}
/* Copyright 2014-2017 Rsyn
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#include "ICCAD15ReaderExtended.h"

#include "rsyn/io/reader/ICCAD15Reader.h"
#include "rsyn/util/StreamStateSaver.h"
#include "rsyn/util/Environment.h"

#include "x/model/DefaultTimingModelWithIdealClock.h"
#include "x/infra/iccad15/Infrastructure.h"
#include "x/infra/iccad15/utilICCAD15.h"

#include "rsyn/util/Json.h"

namespace ICCAD15 {

bool ICCAD15ReaderExtended::load(const Rsyn::Json &params) {
	Rsyn::ICCAD15Reader::load(params);

	const bool globalPlacementOnly = params.value("globalPlacementOnly", false);
	if (globalPlacementOnly) {
		openBenchmarkFromICCAD15ForGlobalPlacementOnly();
	} else {
		openBenchmarkFromICCAD15();
	} // end else

	return true;
} // end method 

// -----------------------------------------------------------------------------

void ICCAD15ReaderExtended::openBenchmarkFromICCAD15() {
	Rsyn::PhysicalDesign physicalDesign = session.getPhysicalDesign();

	Stepwatch watchJezz("Starting Jezz");
	session.startService("rsyn.jezz", {});
	watchJezz.finish();

	Stepwatch watchBlockageControl("Starting blockage control");
	session.startService("ufrgs.blockageControl", {});
	watchBlockageControl.finish();
		
	Stepwatch watchInfrastructure("Initializing contest infrastructure");
	session.startService("ufrgs.ispd16.infra", {
		{"targetUtilization", optionTargetUtilization},
		{"maxDisplacement", optionMaxDisplacement}
	});	
	watchInfrastructure.finish();
		
	// Stats
	int statsSequentialCells = 0;
	int statsCombinationalCells = 0;
	int statsPorts = 0;
	int statsBlocks = 0;
	int statsLCBs = 0;
	int statsNonPortCells = 0; 
		
	for (Rsyn::Instance instance : clsModule.allInstances()) {
		Rsyn::Cell cell = instance.asCell(); // TODO: hack, assuming that the instance is a cell
		if (cell.isPort()) {
			statsPorts++;
		} else {
			statsNonPortCells++;
			Rsyn::PhysicalCell phCell = physicalDesign.getPhysicalCell(cell);
			if (cell.isLCB()) {
				statsLCBs++;
			} // end if
			
			if (cell.isSequential()) {
				statsSequentialCells++;
			} else {
				if (cell.isMacroBlock()) {
					statsBlocks++;
				} else {
					statsCombinationalCells++;
				} // end else
			} // end else
		} // end else
	} // end for
		
	StreamStateSaver sss(std::cout);
	std::cout << std::setprecision(2) << std::fixed;
	std::cout << "Stats\n";
	std::cout << "\t#Cells : " << statsNonPortCells << "\n";
	std::cout << "\t#Combinational : " << statsCombinationalCells 
			<< " (" << (100*statsCombinationalCells/double(statsNonPortCells)) << "%)\n";
	std::cout << "\t#Sequential : " << statsSequentialCells 
			<< " (" << (100*statsSequentialCells/double(statsNonPortCells)) << "%)\n";
	std::cout << "\t#Blocks : " << statsBlocks 
			<< " (" << (100*statsBlocks/double(statsNonPortCells)) << "%)\n";
	std::cout << "\t#LCBs : " << statsLCBs 
			<< " (" << (100*statsLCBs/double(statsNonPortCells)) << "%)\n";
	sss.restore();
} // end method 

// -----------------------------------------------------------------------------

void ICCAD15ReaderExtended::openBenchmarkFromICCAD15ForGlobalPlacementOnly() {
} // end method 

} // end namespace 


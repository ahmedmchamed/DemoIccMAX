#include "IccRoundTrip.h"
#include "IccRoundTripCmpEntry.h"
#include <cstdio>
#include <iostream>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include "IccProfLibVer.h"
#include "CommandLineUtility.h"
#include "ColourData.h"

namespace IccRoundTrip {
    int run(std::uint16_t argCount, std::vector<std::string> args) {


        const std::string fileName{argv[1]};
        ColourData colourData{};
        colourData.isDeviceToPcs = static_cast<bool>(argv[2]);

        if (std::ifstream openFile{fileName}; openFile.is_open()) {
            std::string currentLine{};
            std::string rowItem{};

            while (std::getline(openFile, currentLine)) {
                std::istringstream lineStream{currentLine};
                ColourData::Rows row{};

                while (std::getline(lineStream, rowItem, ',')) {
                    if (rowItem != std::string{"-->"}) {
                        row.push_back(std::stof(rowItem));
                    }
                }

                colourData.csvData.push_back(row);
            }
        } else {
            std::cerr << "\nError opening file: " << fileName << std::endl;
            return -1;
        }

        if (argc <= 1) {
            std::cerr << "Usage: iccRoundTrip profile {rendering_intent=1 {use_mpe=0}}" << std::endl;
            std::cerr << "Built with IccProfLib version " << ICCPROFLIBVER << std::endl;
            std::cerr << "where rendering_intent is (0=perceptual, 1=relative, 2=saturation, 3=absolute)" << std::endl;
            return -1;
        }

        icRenderingIntent nIntent = icRelativeColorimetric;
        int nUseMPE = 0;

        if (argc > 2) {
            nIntent = static_cast<icRenderingIntent>(std::atoi(argv[4]));
            if (argc>3) {
              nUseMPE = atoi(argv[3]);
            }
        }

        CIccMinMaxEval eval;

        icStatusCMM stat = eval.EvaluateProfile(argv[3], 0, nIntent, icInterpLinear, (nUseMPE != 0));

        if (stat != icCmmStatOk) {
            std::cerr << "Unable to perform round trip on " << argv[1] << std::endl;
            return -1;
        }

        CIccPRMG prmg;

        stat = prmg.EvaluateProfile(argv[3], nIntent, icInterpLinear, (nUseMPE != 0));

        if (stat != icCmmStatOk) {
            std::cerr << "Unable to perform PRMG analysis on " << argv[3] << std::endl;
            return -1;
        }

        CIccInfo info;

        return 0;
    }
}

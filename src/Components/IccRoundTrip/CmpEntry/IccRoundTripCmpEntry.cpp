#include "IccRoundTrip.h"
#include "IccRoundTripCmpEntry.h"
#include "ColourData.h"
#include <iostream>
#include "IccProfLibVer.h"
#include "CommandLineUtility.h"

namespace IccRoundTrip {
    int run(std::uint16_t const argCount, std::vector<std::string> const& args) {
        CommandLineUtility utility{};
        ColourData const data{ utility.parseCommandLineArgs(args)};

        if (argCount <= 1) {
            std::cerr << "Usage: iccRoundTrip profile {rendering_intent=1 {use_mpe=0}}" << std::endl;
            std::cerr << "Built with IccProfLib version " << ICCPROFLIBVER << std::endl;
            std::cerr << "where rendering_intent is (0=perceptual, 1=relative, 2=saturation, 3=absolute)" << std::endl;
            return -1;
        }

        icRenderingIntent nIntent = icRelativeColorimetric;
        int nUseMPE = 0;

        if (argCount > 2) {
            nIntent = static_cast<icRenderingIntent>(static_cast<std::uint32_t>(data.getRenderIntent()));
            // if (argc>3) {
            //   nUseMPE = atoi(argv[3]);
            // }
        }

        CIccMinMaxEval eval;

        icStatusCMM stat = eval.EvaluateProfile(data, data.getProfile().c_str(), 0, nIntent, icInterpLinear, (nUseMPE != 0));

        if (stat != icCmmStatOk) {
            std::cerr << "Unable to perform round trip on " << utility.getCommandLineArgValue(std::string{ "profile" }).first << std::endl;
            return -1;
        }

        CIccPRMG prmg;

        stat = prmg.EvaluateProfile(data.getProfile().c_str(), nIntent, icInterpLinear, (nUseMPE != 0));

        if (stat != icCmmStatOk) {
            std::cerr << "Unable to perform PRMG analysis on " << utility.getCommandLineArgValue(std::string{ "profile" }).first << std::endl;
            return -1;
        }

        CIccInfo info;

        return 0;
    }
}

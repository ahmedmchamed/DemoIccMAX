/** @file
File:       IccEval.cpp

Contains:   Implementation of the CIccProfile Evaluation utilites.

Version:    V1

Copyright:  (c) see ICC Software License
*/

/*
* The ICC Software License, Version 0.2
*
*
* Copyright (c) 2003-2012 The International Color Consortium. All rights 
* reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer. 
*
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in
*    the documentation and/or other materials provided with the
*    distribution.
*
* 3. In the absence of prior written permission, the names "ICC" and "The
*    International Color Consortium" must not be used to imply that the
*    ICC organization endorses or promotes products derived from this
*    software.
*
*
* THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED.  IN NO EVENT SHALL THE INTERNATIONAL COLOR CONSORTIUM OR
* ITS CONTRIBUTING MEMBERS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
* USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
* OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
* SUCH DAMAGE.
* ====================================================================
*
* This software consists of voluntary contributions made by many
* individuals on behalf of the The International Color Consortium. 
*
*
* Membership in the ICC is encouraged when this software is used for
* commercial purposes. 
*
*  
* For more information on The International Color Consortium, please
* see <http://www.color.org/>.
*  
* 
*/

////////////////////////////////////////////////////////////////////// 
// HISTORY:
//
// -Initial implementation by Max Derhak 5-15-2003
//
//////////////////////////////////////////////////////////////////////

#include <cmath>
#include <fstream>
#include "CommandLineUtility.h"
#include "IccEval.h"
#include "IccTag.h"

#ifdef USEREFICCMAXNAMESPACE
namespace refIccMAX {
#endif

icStatusCMM CIccEvalCompare::EvaluateProfile(
    IccRoundTrip::ColourData const &colourData,
    CIccProfile *pProfile,
    icUInt8Number nGran/* =0 */,
    icRenderingIntent nIntent/* =icUnknownIntent */,
    icXformInterp nInterp/* =icInterpLinear */,
    bool buseMpeTags/* =true */)
{
    using Rows = IccRoundTrip::ColourData::Rows;

    if (!pProfile) {
        return icCmmStatCantOpenProfile;
    }

    if (pProfile->m_Header.deviceClass != icSigInputClass &&
        pProfile->m_Header.deviceClass != icSigDisplayClass &&
        pProfile->m_Header.deviceClass != icSigOutputClass &&
        pProfile->m_Header.deviceClass != icSigColorSpaceClass) {
        return icCmmStatInvalidProfile;
    }

    CIccCmm profileApplier(
        colourData.isDeviceToPcs() ? icSigUnknownData : icSigLabData,
        colourData.isDeviceToPcs() ? icSigLabData : pProfile->m_Header.colorSpace,
        colourData.isDeviceToPcs()
    );

    CIccCmm Lab2Dev2Lab(icSigLabData, icSigLabData, false);

    icStatusCMM result;

    result = profileApplier.AddXform(*pProfile, nIntent, nInterp, nullptr, icXformLutColorimetric, buseMpeTags);

    if (result!=icCmmStatOk) {
        return result;
    }

    result = profileApplier.Begin();

    if (result != icCmmStatOk) {
        return result;
    }

    icFloatNumber sPixel[15];
    icFloatNumber devPcs[15];

    std::vector<Rows> outputData;
    for (const auto &row: colourData.getCSVData()) {
        for (std::size_t i = 0; i < row.size(); ++i) {
            sPixel[i] = row[i];
        }

        if (!colourData.isDeviceToPcs()) {
            icLabToPcs(sPixel);
        }

        profileApplier.Apply(devPcs, sPixel); //Convert device value to pcs from input table

        if (colourData.isDeviceToPcs()) {
            icLabFromPcs(devPcs);
        }

        std::vector<float> outputValue;
        for (const auto channel: devPcs) {
            outputValue.push_back(channel);
        }

        outputData.push_back(outputValue);
    }

    std::ofstream fileStream{ "output.csv" };
    //
    // FILE *outputFile = fopen("output.csv", "w");
    // for (const auto outputValue: outputData) {
    //     for (const auto outputChannel: outputValue) {
    //         fprintf(outputFile, "%f,", outputChannel);
    //     }
    //     fprintf(outputFile, "\n");
    // }
    // fclose(outputFile);

    for (auto const& outputValue : outputData) {
        for (auto const outputChannel : outputValue) {
            fileStream << outputChannel << ',';
        }
        fileStream << std::endl;
    }

    return icCmmStatOk;
}

icStatusCMM CIccEvalCompare::EvaluateProfile(
    IccRoundTrip::ColourData const &colourData,
    const icChar *szProfilePath,
    icUInt8Number nGrid/* =0 */,
    icRenderingIntent nIntent/* =icUnknownIntent */,
    icXformInterp nInterp/* =icInterpLinear */,
    bool buseMpeTags/* =true */)
{
CIccProfile *pProfile = ReadIccProfile(szProfilePath);

if (!pProfile)
    return icCmmStatCantOpenProfile;

icStatusCMM result = EvaluateProfile(colourData, pProfile, nGrid, nIntent, nInterp, buseMpeTags);

delete pProfile;

    return result;
}

#ifdef USEREFICCMAXNAMESPACE
} //namespace refIccMAX
#endif

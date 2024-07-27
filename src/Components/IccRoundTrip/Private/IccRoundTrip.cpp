#include <cstdio>
#include <cmath>
#include "IccRoundTrip.h"
#include "IccUtil.h"
#include "IccPrmg.h"

namespace IccRoundTrip {
    CIccMinMaxEval::CIccMinMaxEval() :
        minDE1{ 10000 },
        minDE2{ 10000 },
        maxDE1{ -1 },
        maxDE2{ -1 },
        num3{ 0 },
        m_nTotal{ 0 },
        maxLab1{},
        maxLab2{},
        sum1{ 0 },
        sum2{ 0 },
        num1{ 0.0 },
        num2{ 0.0 }
    {
        memset(&maxLab1[0], 0, sizeof(maxLab1));
        memset(&maxLab2[0], 0, sizeof(maxLab2));
    }

    void CIccMinMaxEval::Compare(icFloatNumber *pixel, icFloatNumber *deviceLab, icFloatNumber *lab1, icFloatNumber *lab2)
    {
        icFloatNumber DE1 = icDeltaE(deviceLab, lab1);
        icFloatNumber DE2 = icDeltaE(lab1, lab2);

        if (DE1<minDE1) {
          minDE1 = DE1;
        }

        if (DE1>maxDE1) {
          maxDE1 = DE1;
          memcpy(&maxLab1[0], deviceLab, sizeof(maxLab1));
        }

        if (DE2<minDE2) {
          minDE2 = DE2;
        }

        if (DE2>maxDE2) {
          maxDE2 = DE2;
          memcpy(&maxLab2[0], deviceLab, sizeof(maxLab2));
        }

        if(DE2 <= 1.0)
          num3 += 1;

        sum1 += DE1;
        num1 += 1.0;

        sum2 += DE2;
        num2 += 1.0;

        m_nTotal += 1;
    }
}


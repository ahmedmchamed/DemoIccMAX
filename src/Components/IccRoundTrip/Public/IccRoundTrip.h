#ifndef ICC_ROUND_TRIP_H
#define ICC_ROUND_TRIP_H

#include "IccUtil.h"
#include "IccEval.h"
#include "IccPrmg.h"

class CIccMinMaxEval : public CIccEvalCompare
{
  public:
    CIccMinMaxEval();

    void Compare(icFloatNumber *pixel, icFloatNumber *deviceLab, icFloatNumber *lab1, icFloatNumber *lab2) override;

    icFloatNumber GetMean1() { return sum1 / num1; }
    icFloatNumber GetMean2() { return sum2 / num2; }

    icFloatNumber minDE1, minDE2;
    icFloatNumber maxDE1, maxDE2;
    icUInt32Number num3, m_nTotal;

    icFloatNumber maxLab1[3], maxLab2[3];

  protected:

    icFloatNumber sum1, sum2;
    icFloatNumber num1, num2;
};

#endif
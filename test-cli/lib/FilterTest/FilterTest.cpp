#include "FilterTest.h"
#include <arm_math.h>
#include "IIRFilter.h"
#include "AccelerometerFilter.h"
#include "GyroscopeFilter.h"

#define TEST_FILTER_OUTPUT 1

FilterTest::FilterTest() {
  m_message = String("Test Filter Output");
}

const String runFilterMessage = "Run filter on data";

void FilterTest::setup() {
  addExit(this);
  addOption(this, &FilterTest::runFilter, runFilterMessage);
} 

void FilterTest::printTitle() {
  DEBUG_SERIAL.println("Filter Test");
}

void FilterTest::runFilter() {
  static int dataSize = 512;
  float32_t data[dataSize] = {
    0.08543,
-0.0041,
-0.13107,
-0.1444,
-0.0578,
-0.08871,
-0.10256,
0.05079,
0.19268,
0.16137,
0.06185,
0.08076,
0.15071,
0.01521,
-0.16531,
-0.15918,
-0.06326,
-0.08777,
-0.11895,
0.00309,
0.14671,
0.11527,
-0.01569,
-0.00184,
0.06957,
-0.06579,
-0.19129,
-0.08164,
0.03413,
-0.01649,
-0.055,
0.08676,
0.22306,
0.10368,
-0.04634,
-0.05073,
-0.03621,
-0.11668,
-0.15905,
-0.07605,
0.01601,
0.04612,
0.1182,
0.22226,
0.21373,
0.06171,
-0.02995,
-0.02062,
-0.10589,
-0.22434,
-0.22953,
-0.06246,
0.07424,
0.05372,
0.09249,
0.23465,
0.21466,
-0.01343,
-0.15772,
-0.12348,
-0.17757,
-0.24445,
-0.14413,
0.08849,
0.14245,
0.0809,
0.14751,
0.21426,
0.10968,
-0.07525,
-0.15972,
-0.07112,
-0.07578,
-0.11082,
0.02268,
0.11807,
0.06944,
-0.00397,
0.02121,
0.06584,
0.00456,
-0.07139,
-0.02555,
0.06331,
0.00389,
-0.06472,
0.01482,
0.05812,
-0.02915,
-0.11482,
-0.08191,
-0.02542,
-0.01037,
0.04666,
0.13885,
0.14605,
0.0312,
-0.01636,
0.04772,
0.02294,
-0.09936,
-0.15799,
-0.06779,
0.00549,
0.01855,
0.09036,
0.13366,
0.09982,
0.00922,
-0.01383,
0.04093,
-0.01663,
-0.10016,
-0.06166,
0.04119,
0.01015,
-0.03981,
-0.00197,
0.03666,
0.02441,
-0.02435,
0.00469,
0.05305,
0.00735,
-0.04967,
-0.01236,
0.02987,
0.01255,
-0.00197,
0.05279,
0.0749,
0.0404,
-0.04234,
-0.04287,
-0.02902,
-0.05793,
-0.047,
0.06611,
0.1623,
0.07104,
-0.02102,
0.01601,
0.05625,
-0.01716,
-0.13001,
-0.12241,
-0.07898,
-0.07525,
-0.02968,
0.02614,
0.04013,
0.02028,
0.00615,
0.0396,
0.00869,
-0.0061,
-0.00224,
0.00016,
0.02148,
0.01521,
0.03253,
0.04612,
0.00189,
-0.09523,
-0.10523,
-0.02515,
-0.00504,
-0.02036,
0.05572,
0.1274,
0.10661,
0.00296,
-0.02449,
0.00242,
-0.0105,
-0.04514,
-0.01689,
-0.0029,
-0.03421,
-0.04807,
0.00416,
0.04892,
0.02614,
-0.02715,
-0.0077,
-0.00344,
-0.01236,
0.05572,
0.06797,
0.04572,
0.02761,
-0.03328,
-0.03728,
-0.04101,
-0.06925,
-0.11708,
-0.11282,
-0.03381,
0.00829,
0.04519,
0.12167,
0.14538,
0.11261,
0.06278,
-0.00584,
-0.06006,
-0.14919,
-0.18543,
-0.11135,
-0.01929,
0.03493,
0.11061,
0.18602,
0.15058,
0.06504,
0.0408,
-0.00544,
-0.10496,
-0.1448,
-0.11162,
-0.06113,
-0.02022,
0.01335,
0.08396,
0.10368,
0.12313,
0.11434,
0.06931,
0.0288,
0.00069,
-0.04314,
-0.05513,
-0.03928,
-0.0045,
-0.00064,
0.05332,
0.10115,
0.09622,
0.02734,
-0.03035,
-0.06606,
-0.07632,
-0.06805,
-0.01263,
0.01615,
0.0288,
0.04612,
0.05905,
0.09022,
0.05332,
-0.06352,
-0.09004,
-0.05153,
-0.05593,
-0.07099,
-0.03688,
0.01188,
0.01148,
0.00376,
0.01388,
0.07677,
0.07184,
-0.0041,
-0.0554,
-0.06006,
-0.06712,
-0.06899,
-0.02729,
0.02294,
0.01108,
0.02454,
0.00722,
-0.0105,
-0.02769,
-0.06512,
-0.06486,
-0.05167,
-0.06805,
-0.03661,
0.01815,
0.06171,
0.07943,
0.05812,
0.0356,
0.01868,
-0.03914,
-0.08884,
-0.07645,
-0.06179,
-0.07139,
-0.05713,
0.01788,
0.08023,
0.0384,
0.02108,
0.01082,
-0.05073,
-0.13027,
-0.11655,
0.00109,
0.0817,
0.05931,
0.04879,
0.07304,
0.06837,
-0.00797,
-0.06153,
-0.05753,
-0.03954,
-0.03901,
0.03067,
0.14525,
0.16124,
0.08303,
0.00855,
-0.01396,
-0.05979,
-0.10536,
-0.08418,
-0.03648,
-0.03275,
-0.01356,
0.09049,
0.19001,
0.14685,
0.02654,
0.0288,
0.06438,
0.00722,
-0.08111,
-0.06992,
-0.01183,
0.01974,
0.036,
0.06251,
0.0741,
0.01974,
-0.03555,
-0.02249,
-0.00517,
-0.01636,
-0.02862,
0.05119,
0.13326,
0.08569,
0.04159,
0.09009,
0.09649,
-0.02888,
-0.11549,
-0.06219,
-0.00837,
-0.01689,
0.00695,
0.11274,
0.16177,
0.14565,
0.06198,
0.05385,
0.04133,
-0.03328,
-0.05793,
-0.04314,
-0.0574,
-0.12361,
-0.10989,
0.00429,
0.00909,
-0.06579,
-0.00797,
0.13246,
0.12713,
-0.01823,
-0.04034,
0.02654,
0.02414,
-0.07218,
-0.07258,
0.01748,
0.03533,
-0.0538,
-0.03421,
0.01921,
-0.02396,
-0.11695,
-0.10363,
-0.00064,
0.06851,
0.01828,
0.01295,
0.04839,
0.01029,
-0.07805,
-0.15199,
-0.11748,
-0.0947,
-0.08484,
-0.01796,
0.06051,
0.10634,
0.05279,
0.09236,
0.13006,
-0.01783,
-0.14,
-0.15426,
-0.0967,
-0.13201,
-0.17237,
-0.03222,
0.127,
0.10421,
-0.01863,
-0.06019,
-0.00211,
-0.05606,
-0.12561,
-0.053,
0.02481,
0.01668,
0.04266,
0.12393,
0.15378,
0.02534,
-0.10669,
-0.07498,
-0.03834,
-0.12321,
-0.13041,
-0.01916,
0.04732,
0.02601,
0.05812,
0.14232,
0.10408,
-0.02316,
-0.02449,
0.02467,
-0.01543,
-0.12228,
-0.1384,
-0.02156,
0.03107,
-0.01516,
0.03333,
0.17136,
0.17842,
0.08996,
0.06904,
0.10261,
0.00069,
-0.13267,
-0.17517,
-0.08298,
-0.01156,
-0.00584,
0.01988,
0.06531,
0.08236,
0.08849,
0.1234,
0.13885,
0.00842,
-0.07325,
0.04386,
0.11407,
0.01029,
-0.08724,
-0.01023,
0.07224,
-0.02302,
-0.13414,
-0.03088,
0.05558,
-0.01636,
-0.06059,
0.0761,
0.15218,
0.10088,
0.03054,
0.04053,
0.01828,
-0.08218,
-0.1396,
-0.06153,
0.03134,
-0.01929,
-0.07538,
-0.00837,
0.06558,
-0.01983,
-0.10403,
-0.01543,
0.02054,
-0.06406,
-0.09417,
0.00775,
0.03413,
-0.02888,
-0.01863,
0.03706,
0.00149,
-0.07378,
-0.07125,
0.00402,
0.00176,
-0.06286,
-0.01423,
0.06518,
0.05225,
-0.05313,
-0.06019,
  };
  GyroscopeFilter filter;

  float32_t dataOut[dataSize];

  for (int i = 0; i < dataSize; ++i) {
    filter.update(data[i]);
    dataOut[i] = filter.get();
  }

  DEBUG_SERIAL.println("input output");
  for (int i = 0; i < dataSize; ++i) {
    DEBUG_SERIAL.print(data[i]); DEBUG_SERIAL.print(" "); DEBUG_SERIAL.println(dataOut[i]);
  }

}
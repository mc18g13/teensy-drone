#ifndef ESC_CALIBRATION_HANDLER
#define ESC_CALIBRATION_HANDLER

#include <arm_math.h>
#include <map>
#include "CalibrationHandler.h"

class ESCCalibrationHandler : public CalibrationHandler {
public:


  ESCCalibrationHandler();

  virtual void setup() override;
  virtual void printTitle() override;
  
  void calibrate();

};

#endif 
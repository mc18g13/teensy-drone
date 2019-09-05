#include "MARG.h"

#include <SPI.h>
#include <MPU9250.h>
#include "MemoryLocations.h"

#define SPI_CLOCK 16000000  // 8MHz clock works.

#define SS_PIN   10 

#define WAITFORINPUT(){         \
	while(!Serial.available()){}; \
	while(Serial.available()){    \
		Serial.read();              \
	};                            \
}                               \



MARG::MARG() : mpu(SPI_CLOCK, SS_PIN) {

  delay(5000);

  SPI.begin();

	mpu.init(true);

	uint8_t wai = mpu.whoami();
	if (wai == 0x71) {
		Serial.println("Successful connection");
	}
	else{
		Serial.print("Failed connection: ");
		Serial.println(wai, HEX);
	}


	uint8_t wai_AK8963 = mpu.AK8963_whoami();
	if (wai_AK8963 == 0x48){
		Serial.println("Successful connection to mag");
	}
	else{
		Serial.print("Failed connection to mag: ");
		Serial.println(wai_AK8963, HEX);
	}

	mpu.calib_acc();
	mpu.calib_mag();

  double xSum = 0;
  double ySum = 0;
  double zSum = 0;
  const int GYRO_CALIBRATION_COUNT = 1000;

  for (int i = 0; i < GYRO_CALIBRATION_COUNT; ++i)
  {
    mpu.read_all();
    xSum += mpu.gyro_data[0];
    ySum += mpu.gyro_data[1];
    zSum += mpu.gyro_data[2];
  }

  gyroscopeError.v0 = xSum / GYRO_CALIBRATION_COUNT;
  gyroscopeError.v1 = ySum / GYRO_CALIBRATION_COUNT;
  gyroscopeError.v2 = zSum / GYRO_CALIBRATION_COUNT;

  delay(100);

  // IMU.begin();
  // IMU.setSrd(1);
  // IMU.setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_184HZ);

  this->readValuesForCalibration();
  // this->calculateCalibrations();

  // IMU.calibrateGyro();
}

void MARG::read() {

static const double TO_RADIANS = 0.01745329251;
	mpu.read_all();

  rotationalRates.v0 = (mpu.gyro_data[0] - gyroscopeError.v0) * TO_RADIANS;
  rotationalRates.v1 = (mpu.gyro_data[1] - gyroscopeError.v1) * TO_RADIANS;
  rotationalRates.v2 = (mpu.gyro_data[2] - gyroscopeError.v2) * TO_RADIANS;

  acceleration.v0 = map(mpu.accel_data[0], accelerationCalbrationMin.v0, accelerationCalbrationMax.v0, -g, g);
  acceleration.v1 = map(mpu.accel_data[1], accelerationCalbrationMin.v1, accelerationCalbrationMax.v1, -g, g);
  acceleration.v2 = map(mpu.accel_data[2], accelerationCalbrationMin.v2, accelerationCalbrationMax.v2, -g, g);

  // acceleration.v0 = mpu.accel_data[0];
  // acceleration.v1 = mpu.accel_data[1];
  // acceleration.v2 = mpu.accel_data[2];
  
  magnetics.v0 = map(mpu.mag_data[0], magneticsCalibrationMin.v0, magneticsCalibrationMax.v0, -1, 1);
  magnetics.v1 = map(mpu.mag_data[1], magneticsCalibrationMin.v1, magneticsCalibrationMax.v1, -1, 1);
  magnetics.v2 = map(mpu.mag_data[2], magneticsCalibrationMin.v2, magneticsCalibrationMax.v2, -1, 1);

  // magnetics.v0 = mpu.mag_data[0];
  // magnetics.v1 = mpu.mag_data[1];
  // magnetics.v2 = mpu.mag_data[2];

}

void MARG::calculateCalibrations() {

//   Vector magneticCalibrations   = magneticsCalibrationMax.minus(magneticsCalibrationMin);
//   Vector accelerationCalibrations = accelerationCalbrationMax.minus(accelerationCalbrationMin);

//   Vector magneticsScale =
//     Vector( 1 / magneticCalibrations.v0, 
//             1 / magneticCalibrations.v1, 
//             1 / magneticCalibrations.v2 );

//   Vector accelerationScale =
//     Vector( 1 / accelerationCalibrations.v0, 
//             1 / accelerationCalibrations.v1, 
//             1 / accelerationCalibrations.v2 );

//   Vector accelerationOffset = accelerationCalibrations.divide(2);
//   Vector magneticOffset   = magneticCalibrations.divide(2);

//   IMU.setAccelCalX(accelerationOffset.v0, accelerationScale.v0);
//   IMU.setAccelCalY(accelerationOffset.v1, accelerationScale.v1);
//   IMU.setAccelCalZ(accelerationOffset.v2, accelerationScale.v2);

//   IMU.setMagCalX(magneticOffset.v0, magneticsScale.v0);
//   IMU.setMagCalY(magneticOffset.v1, magneticsScale.v1);
//   IMU.setMagCalZ(magneticOffset.v2, magneticsScale.v2);

}


void MARG::readValuesForCalibration() {
  int eeAddress = ACCEL_START;

  EEPROM.get(eeAddress, accelerationCalbrationMin.v0);

  eeAddress += sizeof(float32_t);
  EEPROM.get(eeAddress, accelerationCalbrationMax.v0);

  eeAddress += sizeof(float32_t);
  EEPROM.get(eeAddress, accelerationCalbrationMin.v1);

  eeAddress += sizeof(float32_t);
  EEPROM.get(eeAddress, accelerationCalbrationMax.v1);

  eeAddress += sizeof(float32_t);
  EEPROM.get(eeAddress, accelerationCalbrationMin.v2);

  eeAddress += sizeof(float32_t);
  EEPROM.get(eeAddress, accelerationCalbrationMax.v2);


  
  eeAddress = MAG_START;
  EEPROM.get(eeAddress, magneticsCalibrationMin.v0);

  eeAddress += sizeof(float32_t);
  EEPROM.get(eeAddress, magneticsCalibrationMax.v0);

  eeAddress += sizeof(float32_t);
  EEPROM.get(eeAddress, magneticsCalibrationMin.v1);

  eeAddress += sizeof(float32_t);
  EEPROM.get(eeAddress, magneticsCalibrationMax.v1);

  eeAddress += sizeof(float32_t);
  EEPROM.get(eeAddress, magneticsCalibrationMin.v2);

  eeAddress += sizeof(float32_t);
  EEPROM.get(eeAddress, magneticsCalibrationMax.v2);

  // Serial.print("accelX max ");Serial.println(accelerationCalbrationMax.v0);
  // Serial.print("accelY max ");Serial.println(accelerationCalbrationMax.v1);
  // Serial.print("accelZ max ");Serial.println(accelerationCalbrationMax.v2);

  // Serial.print("accelX min ");Serial.println(accelerationCalbrationMin.v0);
  // Serial.print("accelY min ");Serial.println(accelerationCalbrationMin.v1);
  // Serial.print("accelZ min ");Serial.println(accelerationCalbrationMin.v2);


  // Serial.print("magX max ");Serial.println(magneticsCalibrationMax.v0);
  // Serial.print("magY max ");Serial.println(magneticsCalibrationMax.v1);
  // Serial.print("magZ max ");Serial.println(magneticsCalibrationMax.v2);

  // Serial.print("magX min ");Serial.println(magneticsCalibrationMin.v0);
  // Serial.print("magY min ");Serial.println(magneticsCalibrationMin.v1);
  // Serial.print("magZ min ");Serial.println(magneticsCalibrationMin.v2);

}
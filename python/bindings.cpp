#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <cstdint>

#include "../src/SparkFun_BNO08x_Raspi_Library.h"

namespace py = pybind11;

PYBIND11_MODULE(bno08x, m) {
  m.doc() = "Pybind11 bindings for SparkFun BNO08x Raspberry Pi Library";

  py::class_<BNO08x>(m, "BNO08x")
      .def(py::init<>())

      // Connection / setup
      .def("begin",
           [](BNO08x &self, uint8_t addr, int8_t int_pin, int8_t rst_pin) {
             return self.begin(addr, int_pin, rst_pin);
           },
           py::arg("deviceAddress") = BNO08x_DEFAULT_ADDRESS,
           py::arg("user_INTPin") = -1,
           py::arg("user_RSTPin") = -1)

      .def("beginSPI",
           [](BNO08x &self, int int_pin, int rst_pin, int cs_pin,
              uint32_t spi_speed, const std::string &dev, const std::string &gpio_chip) {
             return self.beginSPI(int_pin, rst_pin, cs_pin, spi_speed, dev.c_str(), gpio_chip.c_str());
           },
           py::arg("user_INTPin"),
           py::arg("user_RSTPin"),
           py::arg("user_CSPin") = -1,
           py::arg("spiPortSpeed") = 1000000U,
           py::arg("dev") = BNO08x_DEFAULT_SPI_DEV,
           py::arg("gpio_chip") = BNO08x_DEFAULT_GPIO_CHIP)

      .def("close", &BNO08x::close)

      .def("isConnected", &BNO08x::isConnected)

      // Device control
      .def("hardwareReset", &BNO08x::hardwareReset)
      .def("wasReset", &BNO08x::wasReset)
      .def("getResetReason", &BNO08x::getResetReason)
      .def("enableDebugging", &BNO08x::enableDebugging, py::arg("val"))
      .def("softReset", &BNO08x::softReset)
      .def("serviceBus", &BNO08x::serviceBus)
      // Note: resetReason() is declared in the header but not defined in the
      // current source. Do not bind it to avoid undefined symbol at import.
      .def("modeOn", &BNO08x::modeOn)
      .def("modeSleep", &BNO08x::modeSleep)

      // Reports / events
      .def("enableReport",
           [](BNO08x &self, int sensor, uint32_t interval_us, uint32_t sensorSpecific) {
             return self.enableReport(static_cast<sh2_SensorId_t>(sensor), interval_us, sensorSpecific);
           },
           py::arg("sensor"), py::arg("interval_us") = 10000U, py::arg("sensorSpecific") = 0U)
      .def("getSensorEvent", &BNO08x::getSensorEvent)
      .def("getSensorEventID", &BNO08x::getSensorEventID)

      // Utilities
      .def("qToFloat", &BNO08x::qToFloat, py::arg("fixedPointValue"), py::arg("qPoint"))

      // Enable convenience wrappers (ms -> us conversion happens inside C++)
      .def("enableRotationVector", &BNO08x::enableRotationVector, py::arg("timeBetweenReports") = 10U)
      .def("enableGeomagneticRotationVector", &BNO08x::enableGeomagneticRotationVector, py::arg("timeBetweenReports") = 10U)
      .def("enableGameRotationVector", &BNO08x::enableGameRotationVector, py::arg("timeBetweenReports") = 10U)
      .def("enableARVRStabilizedRotationVector", &BNO08x::enableARVRStabilizedRotationVector, py::arg("timeBetweenReports"))
      .def("enableARVRStabilizedGameRotationVector", &BNO08x::enableARVRStabilizedGameRotationVector, py::arg("timeBetweenReports"))
      .def("enableAccelerometer", &BNO08x::enableAccelerometer, py::arg("timeBetweenReports") = 10U)
      .def("enableLinearAccelerometer", &BNO08x::enableLinearAccelerometer, py::arg("timeBetweenReports") = 10U)
      .def("enableGravity", &BNO08x::enableGravity, py::arg("timeBetweenReports") = 10U)
      .def("enableGyro", &BNO08x::enableGyro, py::arg("timeBetweenReports") = 10U)
      .def("enableUncalibratedGyro", &BNO08x::enableUncalibratedGyro, py::arg("timeBetweenReports") = 10U)
      .def("enableMagnetometer", &BNO08x::enableMagnetometer, py::arg("timeBetweenReports") = 10U)
      .def("enableTapDetector", &BNO08x::enableTapDetector, py::arg("timeBetweenReports"))
      .def("enableStepCounter", &BNO08x::enableStepCounter, py::arg("timeBetweenReports") = 10U)
      .def("enableStabilityClassifier", &BNO08x::enableStabilityClassifier, py::arg("timeBetweenReports") = 10U)
      .def("enableActivityClassifier", &BNO08x::enableActivityClassifier, py::arg("timeBetweenReports"), py::arg("activitiesToEnable"))
      .def("enableRawAccelerometer", &BNO08x::enableRawAccelerometer, py::arg("timeBetweenReports") = 10U)
      .def("enableRawGyro", &BNO08x::enableRawGyro, py::arg("timeBetweenReports") = 10U)
      .def("enableRawMagnetometer", &BNO08x::enableRawMagnetometer, py::arg("timeBetweenReports") = 10U)
      .def("enableGyroIntegratedRotationVector", &BNO08x::enableGyroIntegratedRotationVector, py::arg("timeBetweenReports") = 10U)

      // Quaternion getters
      .def("getQuat",
           [](BNO08x &self) {
             float i, j, k, real, rad; uint8_t acc;
             self.getQuat(i, j, k, real, rad, acc);
             return py::make_tuple(i, j, k, real, rad, acc);
           })
      .def("getQuatI", &BNO08x::getQuatI)
      .def("getQuatJ", &BNO08x::getQuatJ)
      .def("getQuatK", &BNO08x::getQuatK)
      .def("getQuatReal", &BNO08x::getQuatReal)
      .def("getQuatRadianAccuracy", &BNO08x::getQuatRadianAccuracy)
      .def("getQuatAccuracy", &BNO08x::getQuatAccuracy)

      // Game quaternion
      .def("getGameQuatI", &BNO08x::getGameQuatI)
      .def("getGameQuatJ", &BNO08x::getGameQuatJ)
      .def("getGameQuatK", &BNO08x::getGameQuatK)
      .def("getGameQuatReal", &BNO08x::getGameQuatReal)

      // Acceleration
      .def("getAccel",
           [](BNO08x &self) {
             float x,y,z; uint8_t acc;
             self.getAccel(x,y,z,acc);
             return py::make_tuple(x,y,z,acc);
           })
      .def("getAccelX", &BNO08x::getAccelX)
      .def("getAccelY", &BNO08x::getAccelY)
      .def("getAccelZ", &BNO08x::getAccelZ)
      .def("getAccelAccuracy", &BNO08x::getAccelAccuracy)

      // Linear acceleration
      .def("getLinAccel",
           [](BNO08x &self) {
             float x,y,z; uint8_t acc;
             self.getLinAccel(x,y,z,acc);
             return py::make_tuple(x,y,z,acc);
           })
      .def("getLinAccelX", &BNO08x::getLinAccelX)
      .def("getLinAccelY", &BNO08x::getLinAccelY)
      .def("getLinAccelZ", &BNO08x::getLinAccelZ)
      .def("getLinAccelAccuracy", &BNO08x::getLinAccelAccuracy)

      // Gyroscope
      .def("getGyro",
           [](BNO08x &self) {
             float x,y,z; uint8_t acc;
             self.getGyro(x,y,z,acc);
             return py::make_tuple(x,y,z,acc);
           })
      .def("getGyroX", &BNO08x::getGyroX)
      .def("getGyroY", &BNO08x::getGyroY)
      .def("getGyroZ", &BNO08x::getGyroZ)
      .def("getGyroAccuracy", &BNO08x::getGyroAccuracy)

      // Uncalibrated Gyro
      .def("getUncalibratedGyro",
           [](BNO08x &self) {
             float x,y,z,bx,by,bz; uint8_t acc;
             self.getUncalibratedGyro(x,y,z,bx,by,bz,acc);
             return py::make_tuple(x,y,z,bx,by,bz,acc);
           })
      .def("getUncalibratedGyroX", &BNO08x::getUncalibratedGyroX)
      .def("getUncalibratedGyroY", &BNO08x::getUncalibratedGyroY)
      .def("getUncalibratedGyroZ", &BNO08x::getUncalibratedGyroZ)
      .def("getUncalibratedGyroBiasX", &BNO08x::getUncalibratedGyroBiasX)
      .def("getUncalibratedGyroBiasY", &BNO08x::getUncalibratedGyroBiasY)
      .def("getUncalibratedGyroBiasZ", &BNO08x::getUncalibratedGyroBiasZ)
      .def("getUncalibratedGyroAccuracy", &BNO08x::getUncalibratedGyroAccuracy)

      // Gyro Integrated RV
      .def("getGyroIntegratedRV",
           [](BNO08x &self) {
             return py::make_tuple(
               self.getGyroIntegratedRVI(),
               self.getGyroIntegratedRVJ(),
               self.getGyroIntegratedRVK(),
               self.getGyroIntegratedRVReal(),
               self.getGyroIntegratedRVangVelX(),
               self.getGyroIntegratedRVangVelY(),
               self.getGyroIntegratedRVangVelZ()
             );
           })
      .def("getGyroIntegratedRVI", &BNO08x::getGyroIntegratedRVI)
      .def("getGyroIntegratedRVJ", &BNO08x::getGyroIntegratedRVJ)
      .def("getGyroIntegratedRVK", &BNO08x::getGyroIntegratedRVK)
      .def("getGyroIntegratedRVReal", &BNO08x::getGyroIntegratedRVReal)
      .def("getGyroIntegratedRVangVelX", &BNO08x::getGyroIntegratedRVangVelX)
      .def("getGyroIntegratedRVangVelY", &BNO08x::getGyroIntegratedRVangVelY)
      .def("getGyroIntegratedRVangVelZ", &BNO08x::getGyroIntegratedRVangVelZ)

      // Magnetometer
      .def("getMag",
           [](BNO08x &self) {
             float x,y,z; uint8_t acc;
             self.getMag(x,y,z,acc);
             return py::make_tuple(x,y,z,acc);
           })
      .def("getMagX", &BNO08x::getMagX)
      .def("getMagY", &BNO08x::getMagY)
      .def("getMagZ", &BNO08x::getMagZ)
      .def("getMagAccuracy", &BNO08x::getMagAccuracy)

      // Gravity
      .def("getGravity",
           [](BNO08x &self) {
             float x,y,z; uint8_t acc;
             self.getGravity(x,y,z,acc);
             return py::make_tuple(x,y,z,acc);
           })
      .def("getGravityX", &BNO08x::getGravityX)
      .def("getGravityY", &BNO08x::getGravityY)
      .def("getGravityZ", &BNO08x::getGravityZ)
      .def("getGravityAccuracy", &BNO08x::getGravityAccuracy)

      // Calibration & tare
      .def("setCalibrationConfig", &BNO08x::setCalibrationConfig, py::arg("sensors"))
      .def("saveCalibration", &BNO08x::saveCalibration)
      .def("tareNow",
           [](BNO08x &self, bool zAxis, int basis) {
             return self.tareNow(zAxis, static_cast<sh2_TareBasis_t>(basis));
           },
           py::arg("zAxis") = false, py::arg("basis") = int(SH2_TARE_BASIS_ROTATION_VECTOR))
      .def("saveTare", &BNO08x::saveTare)
      .def("clearTare", &BNO08x::clearTare)

      // Misc
      .def("getTapDetector", &BNO08x::getTapDetector)
      .def("getTimeStamp", &BNO08x::getTimeStamp)
      .def("getStepCount", &BNO08x::getStepCount)
      .def("getStabilityClassifier", &BNO08x::getStabilityClassifier)
      .def("getActivityClassifier", &BNO08x::getActivityClassifier)
      .def("getActivityConfidence", &BNO08x::getActivityConfidence, py::arg("activity"))

      // Raw sensors
      .def("getRawAccelX", &BNO08x::getRawAccelX)
      .def("getRawAccelY", &BNO08x::getRawAccelY)
      .def("getRawAccelZ", &BNO08x::getRawAccelZ)
      .def("getRawGyroX", &BNO08x::getRawGyroX)
      .def("getRawGyroY", &BNO08x::getRawGyroY)
      .def("getRawGyroZ", &BNO08x::getRawGyroZ)
      .def("getRawMagX", &BNO08x::getRawMagX)
      .def("getRawMagY", &BNO08x::getRawMagY)
      .def("getRawMagZ", &BNO08x::getRawMagZ)

      // Euler convenience
      .def("getRoll", &BNO08x::getRoll)
      .def("getPitch", &BNO08x::getPitch)
      .def("getYaw", &BNO08x::getYaw);
}

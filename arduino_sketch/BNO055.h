#ifndef BNO055_H
#define BNO055_H

#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Wire.h>

// Clase para manejar el sensor BNO055
class Bno055 {
public:
    // Método para inicializar el sensor
    void doInit() {
    if (!bno.begin()) {
        Serial.println("No se pudo encontrar el BNO055");
        return;
    }
    bno.setExtCrystalUse(true);
    bnoReady = true;
    Serial.println("BNO055 inicializado");
}

    // Método para leer los datos del sensor
    void read() {
        if (bnoReady) {
            sensors_event_t orientationData;
            bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
            yaw = orientationData.orientation.x;
            roll = orientationData.orientation.y;
            pitch = orientationData.orientation.z;
        }
    }

    // Métodos para obtener los valores de pitch, roll y yaw
    double getPitch() { return pitch; }
    double getRoll() { return roll; }
    double getYaw() { return yaw; }

    // Método para obtener el estado de calibración
    void getCalibration(uint8_t* system, uint8_t* gyro, uint8_t* accel, uint8_t* mag) {
        bno.getCalibration(system, gyro, accel, mag);
    }

    // Objeto del sensor BNO055 (hacerlo público si necesitas acceder a él desde el sketch)
    // Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

private:
    // Objeto del sensor BNO055
    Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

    // Variables para almacenar los valores de orientación
    double pitch = 0.0;
    double roll = 0.0;
    double yaw = 0.0;

    // Indicador de si el sensor está listo
    bool bnoReady = false;
};

#endif

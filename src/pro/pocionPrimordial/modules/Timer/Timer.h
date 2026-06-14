#pragma once

class Timer {
public:
    // Constructor: inicializa el temporizador con un tiempo en segundos
    Timer(float seconds);

    // Actualiza el temporizador, reduciendo el tiempo restante
    void update(float dt);

    // Reinicia el temporizador al tiempo inicial
    void reset(float seconds);

    // Devuelve si el temporizador ha terminado
    bool isFinished() const;

    // Devuelve el tiempo restante
    float getTimeRemaining() const;

    void reduceBaseTime(float amount);
    float getBaseTime() const;

private:
    float timeRemaining; // Tiempo restante en segundos
    float baseTime; // Tiempo base inicial
};
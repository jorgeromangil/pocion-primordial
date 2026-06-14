#include "Timer.h"

// Constructor: inicializa el temporizador con un tiempo en segundos
Timer::Timer(float seconds) : timeRemaining(seconds), baseTime(seconds) {}

// Actualiza el temporizador, reduciendo el tiempo restante
void Timer::update(float dt) {
    if (timeRemaining > 0) {
        timeRemaining -= dt;
        if (timeRemaining < 0) {
            timeRemaining = 0; // Evitar valores negativos
        }
    }
}

// Reinicia el temporizador al tiempo inicial
void Timer::reset(float seconds) {
    timeRemaining = seconds;
}

// Devuelve si el temporizador ha terminado
bool Timer::isFinished() const {
    return timeRemaining <= 0;
}

// Devuelve el tiempo restante
float Timer::getTimeRemaining() const {
    return timeRemaining;
}

void Timer::reduceBaseTime(float amount) {
    baseTime -= amount;
    if (baseTime < 0.5f) { // Limitar el tiempo mínimo
        baseTime = 0.5f;
    }
}

float Timer::getBaseTime() const {
    return baseTime;
}
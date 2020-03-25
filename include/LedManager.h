#pragma once

#include <Arduino.h>

struct LedSettings
{
    gpio_num_t pin;
    int timeHigh;
    int timeLow;
};


class LedManagerClass
{
private:
    /* data */
    bool isinitialized = false;    
public:
    LedManagerClass();
    /**
     * @brief Inicializa a Class LedManager
     * 
     * @return true Inicializou com sucesso
     * @return false Erro ao inicializar Class 
     */
    bool begin(gpio_num_t pin);
    /**
     * @brief 
     * 
     * @return true 
     * @return false 
     */
    bool start(LedSettings ledSettings);
    /**
     * @brief 
     * 
     * @return true 
     * @return false 
     */
    bool stop();
};

extern LedManagerClass LedManager;


#include "LedManager.h"

///
/// VARIAVEIS
///
LedSettings g_ledSettings;
volatile bool g_started = false;
volatile bool g_stopped = false;
SemaphoreHandle_t xSemaphore = NULL;

///
/// PROTOTIPOS
///
void vTaskLed(void *pvParameters);
bool stopRequested();

///
/// FUNCOES ESTATICAS
///

/**
 * @brief 
 * 
 * @param pvParameters 
 */
void vTaskLed(void *pvParameters)
{
    LedSettings m_ledSettings;
    xSemaphoreTake(xSemaphore, portMAX_DELAY);
    g_started = true;
    m_ledSettings = g_ledSettings;
    xSemaphoreGive(xSemaphore);

    ESP_LOGD("vTaskLed", "Led comecou a piscar.");

    while (true)
    {
        if(stopRequested())
            break;
        
        digitalWrite(m_ledSettings.pin, HIGH);
        delay(m_ledSettings.timeHigh);
        digitalWrite(m_ledSettings.pin, LOW);
        delay(m_ledSettings.timeLow);    

        vTaskDelay(100 / portMAX_DELAY);     
    }

    ESP_LOGD("vTaskLed", "Led parou de piscar.");

    vTaskDelete(NULL);
}

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool stopRequested()
{
    bool isStopped = false;
    xSemaphoreTake(xSemaphore, portMAX_DELAY);
    isStopped = g_stopped;

    if(isStopped)
        g_started = false;

    xSemaphoreGive(xSemaphore);

    return isStopped;
}

///
/// CLASSE
///

/**
 * @brief Construct a new Led Manager Class:: Led Manager Class object
 * 
 */
LedManagerClass::LedManagerClass()
{
}

/**
 * @brief 
 * 
 * @param pin 
 * @return true 
 * @return false 
 */
bool LedManagerClass::begin(gpio_num_t pin)
{
    xSemaphore = xSemaphoreCreateMutex();
    if (xSemaphore == NULL)
        return false;
    
    //Define o pino com saida.
    pinMode(pin, OUTPUT);

    //Atribui o mesmo valor do pino a variavel da Class.
    xSemaphoreTake(xSemaphore, portMAX_DELAY);
    g_ledSettings.pin = pin;
    xSemaphoreGive(xSemaphore);    

    //Inicializa LedManager.
    isinitialized = true;

    return true;
}

/**
 * @brief 
 * 
 * @param ledSettings 
 * @return true 
 * @return false 
 */
bool LedManagerClass::start(LedSettings ledSettings)
{
    //Verifica se o LedManager foi incializado.
    if(!isinitialized)
        return false;
    
    bool isStarted = false;
    xSemaphoreTake(xSemaphore, portMAX_DELAY);
    isStarted = g_started;
    g_ledSettings.timeHigh = ledSettings.timeHigh;
    g_ledSettings.timeLow = ledSettings.timeLow;
    xSemaphoreGive(xSemaphore);

    if(isStarted)
        return false;

    if(ledSettings.timeHigh <= 0 || ledSettings.timeLow <= 0)
        return false;   
    
    xTaskCreate(vTaskLed, "led_task", 10240, NULL, 1, NULL);

    return true;
}

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool LedManagerClass::stop()
{
    if(!isinitialized)
        return false;
    
    bool isStarted = false;
    xSemaphoreTake(xSemaphore, portMAX_DELAY);
    isStarted = g_started;
    xSemaphoreGive(xSemaphore);

    if(!isStarted)
        return false;
    
    xSemaphoreTake(xSemaphore, portMAX_DELAY);
    g_stopped = true;
    xSemaphoreGive(xSemaphore);

    return true;
}

LedManagerClass LedManager;
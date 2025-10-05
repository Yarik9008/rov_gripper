# ROV Gripper Project

Проект для управления захватом ROV (Remotely Operated Vehicle).

## Описание

Этот проект предназначен для разработки системы управления захватом подводного робота. Проект использует PlatformIO для разработки на микроконтроллере STM8.

## Текущая функциональность

- **Мигание светодиодом** - базовая программа для тестирования работы микроконтроллера
- Светодиод подключен к пину PC13 (встроенный светодиод на BluePill)
- Интервал мигания: 500 мс включено / 500 мс выключено
- **Поддержка Serial монитора** через UART (PA9/PA10)

## Технические характеристики

- **Микроконтроллер**: STM32F103C8T6 (72MHz, 20KB RAM, 64KB Flash)
- **Платформа**: STM32 BluePill F103C8
- **Фреймворк**: Arduino (STM32 Core)
- **Инструмент разработки**: PlatformIO
- **Программатор**: ST-Link V2

## Структура проекта

```
rov_gripper/
├── src/
│   └── main.c          # Основной код программы
├── include/            # Заголовочные файлы
├── lib/               # Библиотеки
├── test/              # Тесты
├── platformio.ini    # Конфигурация PlatformIO
└── README.md          # Этот файл
```

## Установка и запуск

1. Установите PlatformIO
2. Клонируйте репозиторий
3. Откройте проект в PlatformIO IDE
4. Подключите ST-Link V2 к микроконтроллеру STM32F103:
   ```
   STM32F103    ST-Link V2
   ----------   -----------
   VCC     →    3.3V
   GND     →    GND  
   SWDIO   →    SWDIO
   SWCLK   →    SWCLK
   NRST    →    NRST
   ```
5. Скомпилируйте и загрузите на микроконтроллер:
   ```bash
   pio run -e stm32f103 -t upload
   ```

## Команды PlatformIO

```bash
# Компиляция проекта
pio run -e stm32f103

# Прошивка микроконтроллера
pio run -e stm32f103 -t upload

# Очистка проекта
pio run -e stm32f103 -t clean

# Мониторинг последовательного порта
pio device monitor
```

## Настройка COM-порта для STM32F103

### Метод 1: USB Virtual COM Port (VCP) - Рекомендуется

Для того чтобы STM32F103 определялся как COM-порт, нужно настроить USB Virtual COM Port:

#### Шаг 1: Обновите код для поддержки Serial
```cpp
#include <Arduino.h>

#define LED_PIN PC13

void setup() {
  pinMode(LED_PIN, OUTPUT);
  
  // Инициализация Serial порта
  Serial.begin(115200);
  
  Serial.println("ROV Gripper STM32F103 - Started");
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  Serial.println("LED ON");
  delay(500);
  
  digitalWrite(LED_PIN, LOW);
  Serial.println("LED OFF");
  delay(500);
}
```

#### Шаг 2: Обновите platformio.ini
```ini
[env:stm32f103]
platform = ststm32
board = bluepill_f103c8
framework = arduino
upload_protocol = stlink
debug_tool = stlink
debug_init_break = tbreak setup
build_flags = 
    -DHAL_PCD_MODULE_ENABLED
    -DUSE_USB_FS
    -DUSBD_VID=0x0483
    -DUSBD_PID=0x5740
```

#### Шаг 3: Установите драйверы USB VCP
- **STM32CubeIDE** с драйверами USB VCP
- **Или скачайте драйверы** с сайта ST

#### Шаг 4: Проверьте COM-порт
```bash
# После прошивки проверьте устройства
pio device list

# Должно появиться:
# COM9
# Hardware ID: USB\VID_0483&PID_5740
# Description: STM32 Virtual COM Port
```

### Метод 2: UART через USB-UART адаптер

Если USB VCP не работает, используйте USB-UART адаптер:

#### Подключение:
```
STM32F103    USB-UART адаптер
---------    -----------------
PA9 (TX)  →  RX
PA10 (RX) →  TX
GND       →  GND
3.3V      →  3.3V (опционально)
```

#### Настройка Serial монитора:
```bash
# Мониторинг с указанием порта
pio device monitor --port COM3 --baud 115200
```

### Метод 3: Через ST-Link Virtual COM Port

Некоторые ST-Link имеют встроенный Virtual COM Port:

#### Проверьте ST-Link:
```bash
pio device list

# Ищите:
# ST-Link Virtual COM Port
# Hardware ID: USB\VID_0483&PID_3748
```

### Решение проблем с COM-портом

**Проблема**: STM32 не определяется как COM-порт
- Установите драйверы USB VCP
- Проверьте подключение USB кабеля
- Попробуйте другой USB порт

**Проблема**: COM-порт появляется, но нет данных
- Проверьте скорость передачи (115200 baud)
- Убедитесь, что Serial.begin() вызван в setup()
- Проверьте подключение TX/RX

**Проблема**: "Access denied" при открытии COM-порта
- Закройте другие программы, использующие порт
- Перезагрузите STM32F103
- Попробуйте другой COM-порт

## Подробная инструкция по подключению STM32 к ST-Link V2

### Схема подключения

```
STM32F103 BluePill    ST-Link V2
------------------    -----------
Pin 1  (3.3V)    →    3.3V
Pin 2  (GND)     →    GND
Pin 3  (PB3)     →    SWDIO
Pin 4  (PB4)     →    SWCLK  
Pin 5  (PB5)     →    NRST
```

### Альтернативная схема подключения

Если у вас ST-Link V2 с отдельными контактами:
```
STM32F103    ST-Link V2
---------    -----------
VCC     →    3.3V
GND     →    GND
PA13    →    SWDIO
PA14    →    SWCLK
NRST    →    NRST
```

### Пошаговая инструкция

1. **Отключите питание** микроконтроллера перед подключением
2. **Подключите провода** согласно схеме выше
3. **Проверьте подключение** - все контакты должны быть надежно соединены
4. **Подключите ST-Link** к компьютеру через USB
5. **Установите драйверы** ST-Link (если не установлены)
6. **Проверьте обнаружение** устройства:
   ```bash
   pio device list
   ```
7. **Скомпилируйте и загрузите**:
   ```bash
   pio run -e stm32f103 -t upload
   ```

### Решение проблем

**Проблема**: "Maple DFU" в диспетчере устройств
- Микроконтроллер в режиме загрузчика
- Отключите USB питание, используйте только ST-Link
- Выполните: `pio run -e stm32f103 -t upload`
- Или используйте STM32CubeProgrammer для принудительной загрузки

**Проблема**: "No ST-Link detected" / Некорректное определение
- **Установите драйверы ST-Link** с сайта ST
- **Проверьте физическое подключение** всех 5 проводов
- **Используйте качественный USB кабель**
- **Попробуйте другой USB порт**
- **Проверьте перемычки BOOT0=GND, BOOT1=GND**

**Проблема**: "Target not responding"
- Убедитесь, что STM32 получает питание 3.3V
- Проверьте подключение NRST
- Попробуйте подключение под сбросом

**Проблема**: "Connection failed"
- Проверьте подключение SWDIO и SWCLK
- Убедитесь, что нет коротких замыканий
- Попробуйте другой ST-Link

**Проблема**: ST-Link не обнаружен системой
- **Скачайте ST-Link Utility** с https://www.st.com/en/development-tools/stsw-link004.html
- **Или установите STM32CubeIDE** с драйверами
- **Проверьте в Диспетчере устройств** наличие "STMicroelectronics STM32 STLink"
- **Используйте STM32CubeProgrammer** как альтернативу

### Режим Maple DFU - подробное решение

Если STM32F103 отображается как "Maple DFU" в диспетчере устройств:

#### Причина:
- Микроконтроллер находится в режиме загрузчика (bootloader mode)
- Плата загружена с Maple bootloader
- Нужно переключить в нормальный режим работы

#### Решение 1: Через ST-Link (рекомендуется)
1. **Отключите USB** от STM32F103
2. **Подключите ST-Link** согласно схеме
3. **Питание только от ST-Link** (3.3V)
4. **Выполните команду**:
   ```bash
   pio run -e stm32f103 -t upload
   ```

#### Решение 2: STM32CubeProgrammer
1. **Скачайте** STM32CubeProgrammer с сайта ST
2. **Подключите ST-Link** к STM32F103
3. **Откройте программу** и выберите ST-Link
4. **Нажмите "Connect"**
5. **Загрузите файл** `.pio/build/stm32f103/firmware.bin`

#### Решение 3: Проверка перемычек
На плате BluePill проверьте:
- **BOOT0** должен быть подключен к **GND**
- **BOOT1** должен быть подключен к **GND**

#### Решение 4: Полная очистка
```bash
# Очистка и перекомпиляция
pio run -e stm32f103 -t clean
pio run -e stm32f103
pio run -e stm32f103 -t upload
```

### Альтернативные методы прошивки

Если ST-Link не работает:
- **STM32CubeProgrammer** - официальная утилита ST
- **OpenOCD** - открытый отладчик
- **Arduino IDE** с поддержкой STM32

## Лицензия

MIT License - см. файл LICENSE для подробностей.

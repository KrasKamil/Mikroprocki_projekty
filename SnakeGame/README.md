# Snake Game na Kinetis KL05 (FRDM-KL05Z) 🐍

Prosta gra w węża napisana w języku C na mikrokontroler NXP Kinetis KL05 (Cortex-M0+), wykorzystująca wyświetlacz LCD 16x2 po I2C oraz analogowy joystick.

## 📷 Opis projektu
Gra obsługuje:
- **Ruch węża** w 4 kierunkach sterowany joystickiem.
- **Logikę ogona** (wąż rośnie po zjedzeniu).
- **Wykrywanie kolizji** ze ścianami i samym sobą (Game Over).
- **Ekran wygranej** po osiągnięciu określonej długości.
- **Sterowanie przerwaniami** (SysTick) dla płynności rozgrywki.

## 🛠 Wymagania sprzętowe
1. Płytka **FRDM-KL05Z** (lub sam chip MKL05Z4).
2. Wyświetlacz **LCD 16x2** ze sterownikiem HD44780 + **Konwerter I2C (PCF8574)**.
3. Moduł **Joysticka analogowego** (XY).
4. Przewody połączeniowe.

## 🔌 Podłączenie (Wiring)

| Moduł      | Pin Modułu | Pin KL05 (FRDM) | Uwagi |
|------------|------------|-----------------|-------|
| **LCD I2C**| GND        | GND             |       |
|            | VCC        | 5V              | Ważne: 5V dla dobrego kontrastu! |
|            | SDA        | PTB4            | I2C0_SDA |
|            | SCL        | PTB3            | I2C0_SCL |
|||||
| **Joystick**| GND       | GND             |       |
|            | VCC        | 3.3V            |       |
|            | VRx        | PTB8            | ADC Channel 11 |
|            | VRy        | PTB9            | ADC Channel 10 |
|            | SW         | Nie podłączony  |       |

## 💻 Struktura kodu
Projekt został podzielony na moduły dla czytelności:
- `main.c` - Główna pętla gry, logika węża, obsługa stanów (Menu, Game, GameOver).
- `lcd_i2c.c` / `.h` - Biblioteka obsługi ekranu przez I2C (Bit-banging).
- `adc_joy.c` / `.h` - Obsługa przetwornika ADC do odczytu pozycji joysticka.

## ⚙️ Kompilacja i Wgrywanie
Środowisko: **Keil uVision 5**
1. Otwórz plik `.uvprojx`.
2. Upewnij się, że masz zainstalowany pakiet `Keil::Kinetis_KLxx_DFP`.
3. Skompiluj (F7).
4. Wgraj na płytkę (F8).

---
Autor: Kamil Kras
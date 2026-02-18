# ⚡ Projekty Systemów Wbudowanych (Embedded)

Witaj w moim repozytorium! Znajdujesz tu zbiór projektów opartych na mikrokontrolerach z rdzeniem **ARM Cortex-M (NXP Kinetis / STM32)**.

Projekty pisane są głównie w języku **C** w środowisku **Keil uVision**, z naciskiem na programowanie "bare-metal" (bezpośrednio na rejestrach), co pozwala na głębokie zrozumienie działania sprzętu.



## 📂 Spis Projektów

Poniżej znajduje się lista dostępnych projektów. Kliknij w nazwę, aby przejść do kodu i szczegółowej dokumentacji.

| Projekt | Opis | Hardware | Kluczowe peryferia |
| :--- | :--- | :--- | :--- |
| 🐍 **[Snake Game](./SnakeGame)** | Klasyczna gra w węża z obsługą kolizji i naliczaniem punktów. | **FRDM-KL05Z** (Cortex-M0+) | `I2C` (Bit-banging), `ADC` (Joystick), `SysTick`, `GPIO` |
| 🚧 **Wkrótce...** | Miejsce na kolejne projekty (np. obsługa UART, PWM, Timery). | - | - |

---

## 🛠️ Wykorzystywane Technologie

### Sprzęt (Hardware)
* **Mikrokontrolery:** NXP Kinetis KL05 (MKL05Z4).
* **Moduły:** Wyświetlacze LCD 16x2 (HD44780 + PCF8574), Joysticki analogowe, Diody LED.
* **Programatory:** OpenSDA / J-Link.

### Oprogramowanie (Software)
* **Język:** C (Standard C99).
* **IDE:** Keil uVision 5.
* **Biblioteki:** CMSIS-CORE (rejestry), własne biblioteki obsługi peryferiów (ADC, I2C, LCD).

---

## 🔌 Pinout Płytki (Hardware Reference)
To jest główny schemat wyprowadzeń, z którego korzystam we wszystkich projektach.
Piny zgodne ze standardem **Arduino R3** są zaznaczone na niebiesko.

![FRDM-KL05Z Pinout](./frdm-kl05z.png)

> **Szybka ściąga:**
> * **I2C:** PTB3 (SCL), PTB4 (SDA)
> * **UART:** PTB1 (TX), PTB2 (RX)
> * **PWM (LED RGB):** PTB8 (Red), PTB9 (Green), PTB10 (Blue)

---

## 🚀 Jak uruchomić te projekty?

1.  Sklonuj repozytorium:
    ```bash
    git clone [https://github.com/KrasKamil/Mikroprocki_projekty.git](https://github.com/KrasKamil/Mikroprocki_projekty.git)
    ```
2.  Wejdź do folderu interesującego Cię projektu (np. `SnakeGame`).
3.  Otwórz plik projektu `.uvprojx` w środowisku Keil uVision.
4.  Upewnij się, że masz zainstalowane odpowiednie **Device Packs** (np. `Keil::Kinetis_KLxx_DFP`).
5.  Skompiluj (`F7`) i wgraj na płytkę (`F8`).

---
📫 **Kontakt:** [Mój Profil](https://github.com/KrasKamil)
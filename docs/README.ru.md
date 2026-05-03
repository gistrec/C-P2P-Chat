# Чат на Си

[English](../README.md) | **Русский**

[![Build](https://github.com/gistrec/C-P2P-Chat/actions/workflows/build.yml/badge.svg)](https://github.com/gistrec/C-P2P-Chat/actions/workflows/build.yml)

Децентрализованный P2P-чат на UDP с неблокирующим вводом/выводом.

Возможности:
- Подключаешься к одному клиенту — автоматически соединяешься с остальными участниками сети.
- Отслеживание активности: клиент, переставший отвечать на пинги, считается отключившимся.
- Псевдографический интерфейс в стиле Midnight Commander.
- Поддержка имён пользователей.

![Скриншот](../screenshot.png)

---

## Зависимости
- [ncurses](https://www.gnu.org/software/ncurses/) — библиотека для управления вводом/выводом в терминале (нужна именно `ncursesw` с поддержкой широких символов).
- [CMake](https://cmake.org/) ≥ 3.10.

## Сборка

```bash
git clone --depth=1 https://github.com/gistrec/C-P2P-Chat.git
cd C-P2P-Chat
cmake -S . -B build
cmake --build build -- -j 2
```

### Установка зависимостей

**Ubuntu / Debian:**
```bash
sudo apt-get install cmake libncursesw5-dev
```

**macOS (Homebrew):**
```bash
brew install cmake ncurses
```

Подробнее: [установка ncurses](ncurses.ru.md), [установка CMake](cmake.ru.md).

## Запуск

| Флаг | Описание |
| ---- | -------- |
| `-n`, `--name <ник>` | Никнейм (**обязательно**) |
| `-l`, `--local-port <port>` | Локальный порт (по умолчанию `8888`) |
| `-r`, `--remote-host <ip>` | IP узла, к которому подключаемся |
| `-p`, `--remote-port <port>` | Порт узла (по умолчанию `8888`) |
| `-h`, `--help` | Показать справку |

Пример:
```bash
./build/C_P2P_Chat --name Сашка --remote-host 46.180.227.50 --remote-port 8888 --local-port 8080
```

## Планы
- Команды чата.
- Поддержка цветов.

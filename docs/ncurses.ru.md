# Установка ncurses

[English](ncurses.md) | **Русский**

Проект использует **ncursesw** — сборку ncurses с поддержкой широких символов — для псевдографического интерфейса.

## Ubuntu / Debian

```bash
sudo apt-get install libncurses5-dev libncursesw5-dev
```

## macOS

В системе предустановлен только legacy curses, поэтому ставим пакет из Homebrew:
```bash
brew install ncurses
```

Сборка автоматически найдёт `ncursesw` в `/opt/homebrew/opt/ncurses` (Apple Silicon) или `/usr/local/opt/ncurses` (Intel) — дополнительных флагов не требуется.

## Из исходников

Если пакета в системе нет — собираем из [официальных исходников](https://invisible-island.net/ncurses/) с поддержкой широких символов:
```bash
./configure --enable-widec
make
sudo make install
```

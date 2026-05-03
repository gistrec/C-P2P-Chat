# Installing ncurses

**English** | [Русский](ncurses.ru.md)

The project uses **ncursesw** — the wide-character build of ncurses — for its terminal UI.

## Ubuntu / Debian

```bash
sudo apt-get install libncurses5-dev libncursesw5-dev
```

## macOS

The system ships only the legacy curses library, so install the Homebrew package:
```bash
brew install ncurses
```

The build picks up `ncursesw` from `/opt/homebrew/opt/ncurses` (Apple Silicon) or `/usr/local/opt/ncurses` (Intel) automatically — no extra flags required.

## From source

If a package is unavailable, build from the [official sources](https://invisible-island.net/ncurses/) with wide-character support enabled:
```bash
./configure --enable-widec
make
sudo make install
```

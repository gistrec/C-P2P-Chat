# Установка CMake

[English](cmake.md) | **Русский**

Проекту требуется **CMake ≥ 3.10**.

## Ubuntu / Debian

В большинстве современных дистрибутивов CMake достаточно свежий:
```bash
sudo apt-get update
sudo apt-get install cmake
```

Если в репозитории слишком старая версия — удалите её и подключите официальный APT-репозиторий Kitware:
```bash
sudo apt-get remove --purge --auto-remove cmake
sudo apt-get install -y software-properties-common lsb-release
wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null \
  | gpg --dearmor - | sudo tee /usr/share/keyrings/kitware-archive-keyring.gpg >/dev/null
echo "deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ $(lsb_release -cs) main" \
  | sudo tee /etc/apt/sources.list.d/kitware.list
sudo apt-get update
sudo apt-get install cmake
```

## macOS

Через [Homebrew](https://brew.sh):
```bash
brew install cmake
```

## Проверка

```bash
cmake --version
```

Должна отображаться версия `3.10` или выше.

# Чат на си

[![CircleCI](https://img.shields.io/circleci/project/github/gistrec/C-P2P-Chat.svg?style=for-the-badge)](https://circleci.com/gh/gistrec/C-P2P-Chat)

Децентрализованный P2P чат с использованием UDP пакетов и неблокирующего ввода/вывода!

Основной функционал:
-  При подключении к одному клиенту, к остальным чат подключится сам!
- Присутствует функция отслеживания активности клиентов. 
Если кто-то долго не отвечает на пакеты - считаем его отключившимся
- Псевдографический интерфейс в стиле Midnight Commander
- Возможность использовать имена

![Скриншот](https://raw.githubusercontent.com/gistrec/C-P2P-Chat/master/screenshot.png)

___

### Зависимости
* [ncurses] - библиотека предназначенная для управления вводом-выводом на терминал
* [cmake] - кроссплатформенная система автоматизации сборки  

___

### Сборка
* [Установка ncurses]
* [Установка CMake v3.9]  
Используйте команду [git clone](https://git-scm.com/docs/git-clone) чтобы скачать этот проект.
```
git clone --depth=1 https://github.com/gistrec/C-P2P-Chat.git Test-chat
cd Test-chat
```

Соберите проект с помощью CMake:
```
cmake . 
cmake --build . -- -j 2
```

### Запуск
Доступные флаги для запуска:

| Флаг   | Описание |
| ------ | -------- |
| -name `<ник>` | Установка ника |
| -connect `<ip>` `<port>` | Подключиться к ip port |
| -port `<port>` | Установить локальный порт |

**Флаг -name является обязательным!**
Пример запуска чата:
```
./С_P2P_Chat -name Сашка -connect 46.180.227.50 8888 -port 8080
```

___

### Todos
 - Добавить команды
 - Добавить поддержку разных цветов

[docs]: <https://github.com/gistrec/C-P2P-Chat/tree/master/docs>
[ncurses]: <https://www.gnu.org/software/ncurses/>
[cmake]: <https://cmake.org/>
[Установка ncurses]: <https://github.com/gistrec/C-P2P-Chat/blob/master/docs/ncurses.md>
[Установка CMake v3.9]: <https://github.com/gistrec/C-P2P-Chat/blob/master/docs/cmake.md>

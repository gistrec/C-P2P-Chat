## Сборка и установка Cmake
Удалите предыдущую версию
```
sudo apt remove cmake
sudo apt purge --auto-remove cmake
```

Скачиваем с официальной страницы CMake последнюю версию исходников.  
Это можно сделать следующими комкандами:
```
version=3.12
build=0
mkdir ~/temp
cd ~/temp
wget https://cmake.org/files/v$version/cmake-$version.$build.tar.gz
tar -xzvf cmake-$version.$build.tar.gz
cd cmake-$version.$build/
```

Установите извлеченные файлы командами:
```
./bootstrap
make -j4
sudo make install
```

Проверьте версию CMake

$ cmake --version  

Результат команды `cmake --version`:  
`cmake version 3.9.X`

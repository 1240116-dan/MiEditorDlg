# Editor de imagenes con MFC y OpenCV 5.1

Editor de imagenes en C++ con interfaz MFC que usa OpenCV 5.1 compilado desde codigo fuente.

Funciones: abrir y guardar, brillo, contraste, desenfoque, grises, bordes (Canny), volteo y restaurar.

## Requisitos
- Visual Studio 2026 con C++ y MFC
- OpenCV 5.1 compilado en C:\cv5\install (Release, x64)

## Configuracion (Release | x64)
- Incluir: C:\cv5\install\include
- Bibliotecas: C:\cv5\install\x64\vc18\lib
- Dependencia: opencv_world510.lib
- Copiar opencv_world510.dll junto al .exe

Script de compilacion de OpenCV: scripts/build-opencv5.ps1

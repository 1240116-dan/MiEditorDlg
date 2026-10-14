$ErrorActionPreference = "Continue"

$Root       = "C:/cv5"
$OpenCVDir  = "$Root/opencv"
$ContribDir = "$Root/opencv_contrib"
$BuildDir   = "$Root/build"
$InstallDir = "$Root/install"
$Generator  = "Visual Studio 18 2026"

Write-Host "1. Descargando OpenCV..." -ForegroundColor Cyan
if (-not (Test-Path "$OpenCVDir/.git")) {
    git clone --branch 5.x --depth 1 https://github.com/opencv/opencv.git $OpenCVDir
}

Write-Host "2. Descargando opencv_contrib..." -ForegroundColor Cyan
if (-not (Test-Path "$ContribDir/.git")) {
    git clone --branch 5.x --depth 1 https://github.com/opencv/opencv_contrib.git $ContribDir
}

Write-Host "3. Preparando carpeta build..." -ForegroundColor Cyan
if (Test-Path $BuildDir) { Remove-Item $BuildDir -Recurse -Force }
New-Item -ItemType Directory -Force -Path $BuildDir | Out-Null
Set-Location $BuildDir

Write-Host "4. Configurando CMake..." -ForegroundColor Cyan
$CMakeArgs = @(
    "-G", $Generator, "-A", "x64",
    "-DCMAKE_CXX_STANDARD=17",
    "-DCMAKE_CXX_STANDARD_REQUIRED=ON",
    "-DBUILD_WITH_DEBUG_INFO=ON",
    "-DOPENCV_EXTRA_MODULES_PATH=$ContribDir/modules",
    "-DBUILD_TESTS=OFF",
    "-DBUILD_PERF_TESTS=OFF",
    "-DBUILD_EXAMPLES=OFF",
    "-DBUILD_DOCS=OFF",
    "-DBUILD_opencv_apps=OFF",
    "-DBUILD_opencv_world=ON",
    "-DBUILD_opencv_python3=OFF",
    "-DBUILD_opencv_java=OFF",
    "-DWITH_VTK=OFF",
    "-DWITH_OPENCL=OFF",
    "-DWITH_TBB=OFF",
    "-DWITH_OPENMP=OFF",
    "-DWITH_FFMPEG=ON",
    "-DCMAKE_INSTALL_PREFIX=$InstallDir",
    $OpenCVDir
)
cmake @CMakeArgs
if ($LASTEXITCODE -ne 0) { throw "La configuracion de CMake fallo." }

$Jobs = [Environment]::ProcessorCount

Write-Host "5. Compilando Release (esto tarda)..." -ForegroundColor Cyan
cmake --build . --config Release --parallel $Jobs
if ($LASTEXITCODE -ne 0) { throw "La compilacion fallo." }

Write-Host "6. Instalando..." -ForegroundColor Cyan
cmake --build . --config Release --target INSTALL --parallel $Jobs
if ($LASTEXITCODE -ne 0) { throw "La instalacion fallo." }

Write-Host "7. Verificando..." -ForegroundColor Cyan
Get-ChildItem $InstallDir -Recurse -Filter "opencv_world*.lib" | Select-Object FullName
Get-ChildItem $InstallDir -Recurse -Filter "opencv_world*.dll" | Select-Object FullName
Write-Host "TERMINADO" -ForegroundColor Green

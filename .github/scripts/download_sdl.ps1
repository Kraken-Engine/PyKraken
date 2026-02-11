$ErrorActionPreference = "Stop"
$DestDir = "C:\sdl"
if (!(Test-Path $DestDir)) {
    New-Item -ItemType Directory -Path $DestDir | Out-Null
}

$Downloads = @(
    @{
        Url = "https://github.com/libsdl-org/SDL/releases/download/release-3.4.0/SDL3-devel-3.4.0-VC.zip"
        Name = "SDL3-3.4.0"
    },
    @{
        Url = "https://github.com/libsdl-org/SDL_image/releases/download/release-3.4.0/SDL3_image-devel-3.4.0-VC.zip"
        Name = "SDL3_image-3.4.0"
    },
    @{
        Url = "https://github.com/libsdl-org/SDL_ttf/releases/download/release-3.2.2/SDL3_ttf-devel-3.2.2-VC.zip"
        Name = "SDL3_ttf-3.2.2"
    },
    @{
        Url = "https://github.com/libsdl-org/SDL_mixer/releases/download/prerelease-3.1.2/SDL3_mixer-devel-3.1.2-VC.zip"
        Name = "SDL3_mixer-3.1.2"
    }
)

foreach ($Item in $Downloads) {
    $ExpectedDir = Join-Path $DestDir $Item.Name
    if (Test-Path $ExpectedDir) {
        Write-Host "Found cached $($Item.Name)"
        continue
    }
    
    Write-Host "Downloading $($Item.Name)..."
    $ZipPath = Join-Path $env:TEMP "$($Item.Name).zip"
    try {
        Invoke-WebRequest -Uri $Item.Url -OutFile $ZipPath
    } catch {
        Write-Error "Failed to download $($Item.Url): $_"
    }
    
    Write-Host "Extracting $($Item.Name)..."
    Expand-Archive -Path $ZipPath -DestinationPath $DestDir -Force
    
    Remove-Item $ZipPath
}

Write-Host "SDL dependencies installed to $DestDir"
Get-ChildItem $DestDir

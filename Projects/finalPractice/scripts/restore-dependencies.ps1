param(
    [Parameter(Mandatory = $true)]
    [string]$ProjectRoot,

    [string]$Triplet = "x64-windows-static-md"
)

$ErrorActionPreference = "Stop"
$ProgressPreference = "SilentlyContinue"

$projectRootPath = [IO.Path]::GetFullPath($ProjectRoot)
$manifestPath = Join-Path $projectRootPath "vcpkg.json"
$toolsRoot = Join-Path $projectRootPath ".tools"
$vcpkgRoot = Join-Path $toolsRoot "vcpkg"
$vcpkgExe = Join-Path $vcpkgRoot "vcpkg.exe"
$stampPath = Join-Path $vcpkgRoot ".final-practice-baseline"

if (-not (Test-Path -LiteralPath $manifestPath)) {
    throw "vcpkg manifest not found: $manifestPath"
}

$manifest = Get-Content -Raw -LiteralPath $manifestPath | ConvertFrom-Json
$baseline = $manifest.'builtin-baseline'
if ([string]::IsNullOrWhiteSpace($baseline) -or $baseline -notmatch '^[0-9a-f]{40}$') {
    throw "vcpkg.json must contain a 40-character builtin-baseline."
}

$installedBaseline = if (Test-Path -LiteralPath $stampPath) {
    (Get-Content -Raw -LiteralPath $stampPath).Trim()
} else {
    ""
}

if (-not (Test-Path -LiteralPath $vcpkgExe) -or $installedBaseline -ne $baseline) {
    New-Item -ItemType Directory -Path $toolsRoot -Force | Out-Null

    $expectedPrefix = $toolsRoot + [IO.Path]::DirectorySeparatorChar

    foreach ($path in @($vcpkgRoot)) {
        if (Test-Path -LiteralPath $path) {
            $fullPath = [IO.Path]::GetFullPath($path)
            if (-not $fullPath.StartsWith($expectedPrefix, [StringComparison]::OrdinalIgnoreCase)) {
                throw "Refusing to remove a path outside the project tools directory: $fullPath"
            }
            Remove-Item -LiteralPath $fullPath -Recurse -Force
        }
    }

    $gitCommand = Get-Command git.exe -ErrorAction SilentlyContinue
    $gitExe = if ($null -ne $gitCommand) { $gitCommand.Source } else { "" }
    if ([string]::IsNullOrWhiteSpace($gitExe)) {
        $gitCandidates = @(
            (Join-Path $env:ProgramFiles "Git\cmd\git.exe"),
            (Join-Path $env:LOCALAPPDATA "Git\cmd\git.exe"),
            (Join-Path $env:LOCALAPPDATA "Programs\Git\cmd\git.exe")
        )
        $gitExe = $gitCandidates | Where-Object { Test-Path -LiteralPath $_ } | Select-Object -First 1
    }
    if ([string]::IsNullOrWhiteSpace($gitExe)) {
        $desktopGit = Get-ChildItem -Path (Join-Path $env:LOCALAPPDATA "GitHubDesktop\app-*\resources\app\git\cmd\git.exe") -ErrorAction SilentlyContinue |
            Sort-Object FullName -Descending |
            Select-Object -First 1
        if ($null -ne $desktopGit) {
            $gitExe = $desktopGit.FullName
        }
    }
    if ([string]::IsNullOrWhiteSpace($gitExe)) {
        throw "Git for Windows was not found. Install Git or GitHub Desktop and retry the build."
    }

    Write-Host "Cloning pinned vcpkg revision $baseline..."
    & $gitExe init $vcpkgRoot
    if ($LASTEXITCODE -ne 0) { throw "Unable to initialize the local vcpkg repository." }
    & $gitExe -C $vcpkgRoot remote add origin "https://github.com/microsoft/vcpkg.git"
    if ($LASTEXITCODE -ne 0) { throw "Unable to configure the vcpkg remote." }
    & $gitExe -C $vcpkgRoot fetch --depth 1 origin $baseline
    if ($LASTEXITCODE -ne 0) { throw "Unable to download vcpkg baseline $baseline." }
    & $gitExe -C $vcpkgRoot checkout --detach FETCH_HEAD
    if ($LASTEXITCODE -ne 0) { throw "Unable to check out vcpkg baseline $baseline." }

    Write-Host "Bootstrapping vcpkg..."
    & (Join-Path $vcpkgRoot "bootstrap-vcpkg.bat") -disableMetrics
    if ($LASTEXITCODE -ne 0 -or -not (Test-Path -LiteralPath $vcpkgExe)) {
        throw "vcpkg bootstrap failed with exit code $LASTEXITCODE."
    }

    Set-Content -LiteralPath $stampPath -Value $baseline -NoNewline
}

Write-Host "Restoring Final Practice dependencies ($Triplet)..."
& $vcpkgExe install `
    --triplet $Triplet `
    --x-wait-for-lock `
    "--x-manifest-root=$projectRootPath" `
    "--x-install-root=$(Join-Path $projectRootPath 'vcpkg_installed')"

if ($LASTEXITCODE -ne 0) {
    throw "vcpkg dependency restore failed with exit code $LASTEXITCODE."
}

$restoreStamp = Join-Path (Join-Path (Join-Path $projectRootPath "vcpkg_installed") $Triplet) ".final-practice-restored"
Set-Content -LiteralPath $restoreStamp -Value $baseline -NoNewline

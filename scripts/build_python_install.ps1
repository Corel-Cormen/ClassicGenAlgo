param(
    [Parameter(Mandatory = $true)]
    [string]$PyPath,
    [Parameter(Mandatory = $true)]
    [string]$PyVersion,
    [Parameter(Mandatory = $true)]
    [string]$PyPlatform
)

if (-not (Test-Path -Path $PyPath)) {
    New-Item -ItemType Directory -Path $PyPath | Out-Null
    Write-Host "Created Python directory: $PyPath"

    $Url = "https://www.python.org/ftp/python/$PyVersion/python-$PyVersion-amd$PyPlatform.zip"

    $PyFile = "$PyPath/python-tmp.zip"
    Write-Host "Downloading: $Url"
    Invoke-RestMethod -Uri $Url -OutFile "$PyFile"
    Write-Host "Download complete: $PyFile"

    Write-Host "Unzip Python: $PyPath"
    Expand-Archive -Path "$PyFile" -DestinationPath "$PyPath" -Force
    Remove-Item $PyFile

    $PyExe = "$PyPath/python.exe"
    & $PyExe --version

    $GetPip = "$PyPath/get-pip.py"
    Invoke-WebRequest -Uri "https://bootstrap.pypa.io/get-pip.py" -OutFile $GetPip
    & $PyExe $GetPip
    Remove-Item $GetPip

    & $PyExe -m ensurepip
    & $PyExe -m pip install numpy --only-binary=:all:
    & $PyExe -m pip install --no-deps --no-cache-dir `
        --disable-pip-version-check --no-warn-script-location `
        scipy benchmark_functions matplotlib Pillow pyparsing opfunu setuptools `
        cycler kiwisolver packaging python-dateutil requests

} else {
    Write-Host "Python directory already exists"
}

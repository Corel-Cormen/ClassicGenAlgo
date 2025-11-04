param(
    [Parameter(Mandatory = $true)]
    [string]$PyPath
)

$CurrentDir = Get-Location
$PatchRoot = Join-Path $CurrentDir "patches\python"
$PatchFiles = Get-ChildItem -Path $PatchRoot -Recurse -Filter *.patch

foreach ($File in $PatchFiles) {
    Write-Host "Apply patch: $($File.Name)"

    $FileDir = Split-Path $File.FullName -Parent
    $RelativePath = $FileDir.Substring($PatchRoot.Length).TrimStart('\')
    $RepoDir = Join-Path $PyPath $RelativePath

    Write-Host "Repo dir: $RepoDir"
    Push-Location $RepoDir

    try {
        git init | Out-Null

        git apply $File.FullName
        if ($LASTEXITCODE -ne 0) {
            Write-Warning "Failed to apply patch"
        } else {
            Write-Host "Patch applied successfully"
        }
    }
    finally {
        if (Test-Path ".git") {
            Remove-Item -Path ".git" -Recurse -Force
        }
        Pop-Location
    }
}

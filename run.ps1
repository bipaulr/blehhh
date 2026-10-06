param(
    [Parameter(Mandatory=$true)]
    [ValidateSet('lex','ec','enfa','nfa','dfa','name','var','calc','ast','for','vc','while')]
    [string]$Program,
    [string]$InputFile
)
$ErrorActionPreference = 'Stop'
if (-not $InputFile) {
    $defaultInput = if ($Program -eq 'lex') { 'lexsource.txt' } else { $Program + 'input.txt' }
    $InputFile = Join-Path $PSScriptRoot $defaultInput
}
$inputPath = (Resolve-Path -LiteralPath $InputFile).Path
# Only whole lines beginning with # (optionally indented) are explanatory comments.
$lines = @(Get-Content -LiteralPath $inputPath | Where-Object { $_ -notmatch '^\s*#' })
$buildDir = Join-Path ([IO.Path]::GetTempPath()) ('vofr-lab-' + [Guid]::NewGuid().ToString('N'))
[void](New-Item -ItemType Directory -Path $buildDir)
$exe = Join-Path $buildDir ($Program + '.exe')

function Invoke-Tool([string]$Tool, [string[]]$ToolArgs) {
    & $Tool @ToolArgs
    if ($LASTEXITCODE -ne 0) { throw "$Tool failed with exit code $LASTEXITCODE" }
}
function Send-Input([string]$Text) {
    $info = New-Object Diagnostics.ProcessStartInfo
    $info.FileName = $exe
    $info.UseShellExecute = $false
    $info.RedirectStandardInput = $true
    $info.CreateNoWindow = $true
    $process = [Diagnostics.Process]::Start($info)
    $process.StandardInput.Write($Text)
    $process.StandardInput.Close()
    $process.WaitForExit()
    $status = $process.ExitCode
    $process.Dispose()
    # Invalid var/for examples intentionally return yyparse's nonzero status.
    if ($status -ne 0 -and $Program -notin @('var','for','while')) {
        throw "Program failed with exit code $status"
    }
}

Push-Location $buildDir
try {
    $cFile = Join-Path $PSScriptRoot ($Program + '.c')
    if (Test-Path -LiteralPath $cFile) {
        Invoke-Tool 'gcc' @('-std=c11','-Wall','-Wextra',$cFile,'-o',$exe)
    } else {
        $flex = if (Get-Command flex -ErrorAction SilentlyContinue) { 'flex' }
                elseif (Get-Command win_flex -ErrorAction SilentlyContinue) { 'win_flex' }
                else { throw 'Install Flex (or win_flex) and add it to PATH.' }
        $yFile = Join-Path $PSScriptRoot ($Program + '.y')
        if (Test-Path -LiteralPath $yFile) {
            if (Get-Command bison -ErrorAction SilentlyContinue) {
                Invoke-Tool 'bison' @('-y','-d',$yFile)
            } elseif (Get-Command win_bison -ErrorAction SilentlyContinue) {
                Invoke-Tool 'win_bison' @('-y','-d',$yFile)
            } elseif (Get-Command yacc -ErrorAction SilentlyContinue) {
                Invoke-Tool 'yacc' @('-d',$yFile)
            } else { throw 'Install Bison (or win_bison/yacc) and add it to PATH.' }
        }
        Invoke-Tool $flex @('--nounistd','--never-interactive', (Join-Path $PSScriptRoot ($Program + '.l')))
        $sources = @(if (Test-Path -LiteralPath $yFile) { 'y.tab.c'; 'lex.yy.c' } else { 'lex.yy.c' })
        Invoke-Tool 'gcc' ($sources + @('-o',$exe))
    }
    if ($Program -eq 'lex') {
        Invoke-Tool $exe @($inputPath)
    } elseif ($Program -in @('var','for','while')) {
        # The PDF's YYACCEPT stops after one statement, so restart for each case.
        foreach ($line in $lines) { Send-Input ($line + "`n") }
    } else { Send-Input (($lines -join "`n") + "`n") }
} finally {
    Pop-Location
    # The generated directory is unique and checked before recursive cleanup.
    $resolvedBuild = [IO.Path]::GetFullPath($buildDir)
    $tempRoot = [IO.Path]::GetFullPath([IO.Path]::GetTempPath())
    if ($resolvedBuild.StartsWith($tempRoot, [StringComparison]::OrdinalIgnoreCase) -and
        ([IO.Path]::GetFileName($resolvedBuild) -like 'vofr-lab-*')) {
        Remove-Item -LiteralPath $resolvedBuild -Recurse -Force
    }
}

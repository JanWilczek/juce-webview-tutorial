$packageSourceName = "nugetRepository"

# Ignore errors: they are meaningless
$ErrorActionPreference = "SilentlyContinue"

Register-PackageSource -Provider NuGet -Name $packageSourceName -Location https://api.nuget.org/v3/index.json -Force
Install-Package Microsoft.Web.WebView2 -Scope CurrentUser -RequiredVersion 1.0.1901.177 -Source $packageSourceName -Force


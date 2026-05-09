@echo off
REM ============================================================================
REM SQL Server Native Client minimal header stub for CI. The real header
REM ships with the SQL Server Native Client SDK and is licensed for
REM redistribution, but installing it on every CI run would be
REM expensive. The stubs here are enough to surface our own SQLP /
REM Database glue errors without dragging in the full SDK.
REM ============================================================================
set "OUT=ThirdParty\sqlncli\Include"
mkdir "%OUT%" 2>nul
> "%OUT%\sqlncli11.h" (
  echo #ifndef SQLNCLI11_H
  echo #define SQLNCLI11_H
  echo #include ^<sql.h^>
  echo #include ^<sqlext.h^>
  echo #endif
)
echo Stubbed sqlncli headers in %OUT%

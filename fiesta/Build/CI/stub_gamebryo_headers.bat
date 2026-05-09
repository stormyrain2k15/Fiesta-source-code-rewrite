@echo off
REM ============================================================================
REM Gamebryo SDK header stubs for CI. The real Gamebryo SDK ships in
REM ThirdParty/Gamebryo/ but it's read-only vendor IP that a public CI
REM image shouldn't redistribute through caching layers. These stubs
REM let Client.vcxproj compile far enough to surface our own bugs.
REM ============================================================================
set "OUT=ThirdParty\Gamebryo\Include"
mkdir "%OUT%" 2>nul
> "%OUT%\NiMain.h" (
  echo #ifndef NIMAIN_H
  echo #define NIMAIN_H
  echo class NiObject { public: virtual ~NiObject^(^) {} };
  echo class NiNode   : public NiObject {};
  echo class NiCamera : public NiObject {};
  echo #endif
)
echo Stubbed Gamebryo headers in %OUT%

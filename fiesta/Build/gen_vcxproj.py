#!/usr/bin/env python3
# Visual Studio 2010 project file generator for the Fiesta tree.
# One-shot run from /app/fiesta/Build/. Re-run after adding files to keep
# .vcxproj / .filters in sync. Outputs go next to the source dirs:
#   Build/Login.vcxproj, Build/Zone.vcxproj, etc.
# Single solution: Build/Fiesta.sln.
#
# Toolset: v100 (VS2010), Win32, MBCS, /MT (static CRT).
import os, uuid, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
BUILD = os.path.join(ROOT, "Build")
os.makedirs(BUILD, exist_ok=True)

# Deterministic GUIDs so re-running doesn't churn the .sln.
def stable_guid(name):
    return "{" + str(uuid.uuid5(uuid.NAMESPACE_DNS, "fiesta." + name)).upper() + "}"

# (project_name, source_dirs[list of dirs relative to root], type, deps[list of project names])
# type: "StaticLibrary" or "Application"
PROJECTS = [
    ("Common",            ["Server/Common"],                                  "StaticLibrary", []),
    ("Shared",            ["Server/Shared"],                                  "StaticLibrary", []),
    ("DataReader",        ["Server/DataReader"],                              "StaticLibrary", ["Shared"]),
    ("LuaRuntime",        ["Lua"],                                            "StaticLibrary", ["Shared"]),
    ("DataServerCommon",  ["Server/DataServer/Common"],                       "StaticLibrary", ["Shared"]),
    ("Login",             ["Server/Login"],                                   "Application",   ["Common", "Shared", "DataReader"]),
    ("DataServer.Account",    ["Server/DataServer/Account"],                  "Application",   ["Common", "Shared", "DataServerCommon"]),
    ("DataServer.AccountLog", ["Server/DataServer/AccountLog"],               "Application",   ["Common", "Shared", "DataServerCommon"]),
    ("DataServer.Character",  ["Server/DataServer/Character"],                "Application",   ["Common", "Shared", "DataServerCommon"]),
    ("DataServer.GameLog",    ["Server/DataServer/GameLog"],                  "Application",   ["Common", "Shared", "DataServerCommon"]),
    ("WorldManager",      ["Server/WorldManager"],                            "Application",   ["Common", "Shared", "DataReader", "DataServerCommon"]),
    ("Zone",              ["Server/Zone"],                                    "Application",   ["Common", "Shared", "DataReader", "LuaRuntime"]),
    ("ZoneTests",         ["Server/Zone/tests"],                              "Application",   ["Common", "Shared", "DataReader", "LuaRuntime"]),
    # NOTE: Client project is intentionally excluded from the active solution
    # build. The client rewrite is out of scope for the server-only audit
    # rounds. Keep the source under fiesta/Client/ so it doesn't drift, but
    # don't generate a vcxproj for it until a separate client-build pass
    # starts. To re-enable, add the following entry:
    #   ("Client", ["Client"], "Application", ["Common", "Shared", "DataReader", "LuaRuntime"]),
]

# Source dirs to exclude from each project's gather() walk. Used so Zone
# doesn't pull in tests/ (which has its own ZoneTests project + main()).
EXCLUDES = {
    "Zone": ["Server/Zone/tests"],
}

# Per-project extra preprocessor definitions. ZoneTests needs
# FIESTA_TEST_MAIN to enable the main() in TestBase.cpp.
EXTRA_DEFINES = {
    "ZoneTests": ["FIESTA_TEST_MAIN"],
}

def gather(src_dirs, exts, excludes=None):
    out = []
    excludes = excludes or []
    for d in src_dirs:
        full = os.path.join(ROOT, d)
        for root, _, files in os.walk(full):
            # Skip any subtree that matches an excluded source dir.
            rel_root = os.path.relpath(root, ROOT).replace("\\", "/")
            if any(rel_root == ex or rel_root.startswith(ex + "/") for ex in excludes):
                continue
            for f in files:
                if not any(f.endswith(e) for e in exts): continue
                rel = os.path.relpath(os.path.join(root, f), BUILD)
                out.append(rel.replace("/", "\\"))
    return sorted(out)

def filter_for(rel, base_dirs):
    # Group by leaf directory under the source root.
    norm = rel.replace("\\", "/")
    for base in base_dirs:
        marker = "/" + base.split("/")[-1] + "/"
        i = norm.find(marker)
        if i < 0: continue
        sub = norm[i + len(marker):]
        parts = sub.split("/")
        if len(parts) <= 1: return base.split("/")[-1]
        return base.split("/")[-1] + "\\" + "\\".join(parts[:-1])
    return ""

def cfg_block(is_app):
    if is_app:
        link = """      <SubSystem>Console</SubSystem>
      <GenerateDebugInformation>true</GenerateDebugInformation>
      <AdditionalDependencies>ws2_32.lib;mswsock.lib;winmm.lib;sqlncli11.lib;%(AdditionalDependencies)</AdditionalDependencies>"""
    else:
        link = ""
    return link

def write_vcxproj(name, src_dirs, kind, deps):
    excludes = EXCLUDES.get(name, [])
    cpps = gather(src_dirs, [".cpp", ".cc"], excludes)
    hdrs = gather(src_dirs, [".h", ".hpp", ".inl"], excludes)
    guid = stable_guid(name)
    is_app = (kind == "Application")
    out_ext = "exe" if is_app else "lib"

    # Include paths: every static-lib dep contributes its source dir to /I
    # so the consuming project can `#include "Foo.h"`.
    inc_dirs = ["..\\Server\\Common", "..\\Server\\Shared", "..\\Server\\DataReader",
                "..\\Server\\DataServer\\Common", "..\\Lua", "..\\ThirdParty\\Gamebryo\\Include"]
    inc_str = ";".join(inc_dirs) + ";%(AdditionalIncludeDirectories)"

    proj = []
    proj.append('<?xml version="1.0" encoding="utf-8"?>')
    proj.append('<Project DefaultTargets="Build" ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">')
    proj.append('  <ItemGroup Label="ProjectConfigurations">')
    for cfg in ("Debug", "Release"):
        proj.append(f'    <ProjectConfiguration Include="{cfg}|Win32"><Configuration>{cfg}</Configuration><Platform>Win32</Platform></ProjectConfiguration>')
    proj.append('  </ItemGroup>')
    proj.append('  <PropertyGroup Label="Globals">')
    proj.append(f'    <ProjectGuid>{guid}</ProjectGuid>')
    proj.append(f'    <RootNamespace>{name.replace(".", "_")}</RootNamespace>')
    proj.append('    <Keyword>Win32Proj</Keyword>')
    proj.append('  </PropertyGroup>')
    proj.append('  <Import Project="$(VCTargetsPath)\\Microsoft.Cpp.Default.props" />')
    for cfg in ("Debug", "Release"):
        proj.append(f'  <PropertyGroup Condition="\'$(Configuration)|$(Platform)\'==\'{cfg}|Win32\'" Label="Configuration">')
        proj.append(f'    <ConfigurationType>{kind}</ConfigurationType>')
        proj.append('    <PlatformToolset>v100</PlatformToolset>')
        proj.append('    <CharacterSet>MultiByte</CharacterSet>')
        proj.append(f'    <UseDebugLibraries>{"true" if cfg == "Debug" else "false"}</UseDebugLibraries>')
        if cfg == "Release":
            proj.append('    <WholeProgramOptimization>true</WholeProgramOptimization>')
        proj.append('  </PropertyGroup>')
    proj.append('  <Import Project="$(VCTargetsPath)\\Microsoft.Cpp.props" />')
    # Pull in the Gamebryo property sheet so SDK paths are user-overridable.
    proj.append('  <ImportGroup Label="PropertySheets">')
    proj.append('    <Import Project="Gamebryo.props" Condition="exists(\'Gamebryo.props\')" />')
    # Every project that uses Lua (LuaRuntime itself + everything that
    # links against it) gets the Lua property sheet so <lua.h> resolves.
    proj.append('    <Import Project="Lua.props" Condition="exists(\'Lua.props\')" />')
    proj.append('  </ImportGroup>')
    for cfg in ("Debug", "Release"):
        proj.append(f'  <PropertyGroup Condition="\'$(Configuration)|$(Platform)\'==\'{cfg}|Win32\'">')
        proj.append(f'    <OutDir>$(SolutionDir)Bin\\{cfg}\\</OutDir>')
        proj.append(f'    <IntDir>$(SolutionDir)Obj\\{cfg}\\$(ProjectName)\\</IntDir>')
        proj.append(f'    <TargetName>{name}</TargetName>')
        proj.append(f'    <TargetExt>.{out_ext}</TargetExt>')
        proj.append('  </PropertyGroup>')
    for cfg in ("Debug", "Release"):
        opt = "Disabled" if cfg == "Debug" else "MaxSpeed"
        rt  = "MultiThreadedDebug" if cfg == "Debug" else "MultiThreaded"
        defs = "WIN32;_WIN32_WINNT=0x0501;_CRT_SECURE_NO_WARNINGS;NOMINMAX;"
        defs += "_DEBUG" if cfg == "Debug" else "NDEBUG"
        if is_app:
            defs += ";_CONSOLE"
        else:
            defs += ";_LIB"
        for extra in EXTRA_DEFINES.get(name, []):
            defs += ";" + extra
        proj.append(f'  <ItemDefinitionGroup Condition="\'$(Configuration)|$(Platform)\'==\'{cfg}|Win32\'">')
        proj.append('    <ClCompile>')
        proj.append('      <WarningLevel>Level3</WarningLevel>')
        proj.append('      <PrecompiledHeader>NotUsing</PrecompiledHeader>')
        proj.append(f'      <Optimization>{opt}</Optimization>')
        proj.append(f'      <RuntimeLibrary>{rt}</RuntimeLibrary>')
        proj.append(f'      <PreprocessorDefinitions>{defs};%(PreprocessorDefinitions)</PreprocessorDefinitions>')
        proj.append(f'      <AdditionalIncludeDirectories>{inc_str}</AdditionalIncludeDirectories>')
        proj.append('      <MultiProcessorCompilation>true</MultiProcessorCompilation>')
        proj.append('      <MinimalRebuild>false</MinimalRebuild>')
        if cfg == "Debug":
            proj.append('      <DebugInformationFormat>ProgramDatabase</DebugInformationFormat>')
        proj.append('    </ClCompile>')
        if is_app:
            proj.append('    <Link>')
            proj.append('      <SubSystem>Console</SubSystem>')
            proj.append('      <GenerateDebugInformation>true</GenerateDebugInformation>')
            proj.append('      <AdditionalDependencies>ws2_32.lib;mswsock.lib;winmm.lib;%(AdditionalDependencies)</AdditionalDependencies>')
            if cfg == "Release":
                proj.append('      <EnableCOMDATFolding>true</EnableCOMDATFolding>')
                proj.append('      <OptimizeReferences>true</OptimizeReferences>')
            proj.append('    </Link>')
        else:
            proj.append('    <Lib>')
            proj.append('      <AdditionalOptions>/ignore:4221 %(AdditionalOptions)</AdditionalOptions>')
            proj.append('    </Lib>')
        proj.append('  </ItemDefinitionGroup>')
    if cpps:
        proj.append('  <ItemGroup>')
        for c in cpps: proj.append(f'    <ClCompile Include="{c}" />')
        proj.append('  </ItemGroup>')
    if hdrs:
        proj.append('  <ItemGroup>')
        for h in hdrs: proj.append(f'    <ClInclude Include="{h}" />')
        proj.append('  </ItemGroup>')
    if deps:
        proj.append('  <ItemGroup>')
        for d in deps:
            proj.append(f'    <ProjectReference Include="{d}.vcxproj">')
            proj.append(f'      <Project>{stable_guid(d)}</Project>')
            proj.append('    </ProjectReference>')
        proj.append('  </ItemGroup>')
    proj.append('  <Import Project="$(VCTargetsPath)\\Microsoft.Cpp.targets" />')
    proj.append('</Project>')

    path = os.path.join(BUILD, name + ".vcxproj")
    open(path, "w", newline="\r\n").write("\n".join(proj))

    # .filters file -- group source files into folders mirroring the
    # source layout so Solution Explorer is navigable instead of a 400-
    # entry flat list.
    filters_seen = set()
    f = []
    f.append('<?xml version="1.0" encoding="utf-8"?>')
    f.append('<Project ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">')
    # Pre-compute filter folders.
    folders = set()
    for c in cpps + hdrs:
        ff = filter_for(c, src_dirs)
        if ff:
            parts = ff.split("\\")
            for i in range(1, len(parts) + 1):
                folders.add("\\".join(parts[:i]))
    f.append('  <ItemGroup>')
    for fld in sorted(folders):
        gid = stable_guid(name + "." + fld)
        f.append(f'    <Filter Include="{fld}"><UniqueIdentifier>{gid}</UniqueIdentifier></Filter>')
    f.append('  </ItemGroup>')
    if cpps:
        f.append('  <ItemGroup>')
        for c in cpps:
            ff = filter_for(c, src_dirs)
            if ff:
                f.append(f'    <ClCompile Include="{c}"><Filter>{ff}</Filter></ClCompile>')
            else:
                f.append(f'    <ClCompile Include="{c}" />')
        f.append('  </ItemGroup>')
    if hdrs:
        f.append('  <ItemGroup>')
        for h in hdrs:
            ff = filter_for(h, src_dirs)
            if ff:
                f.append(f'    <ClInclude Include="{h}"><Filter>{ff}</Filter></ClInclude>')
            else:
                f.append(f'    <ClInclude Include="{h}" />')
        f.append('  </ItemGroup>')
    f.append('</Project>')
    open(path + ".filters", "w", newline="\r\n").write("\n".join(f))

    # .user file -- empty (per-developer settings, kept out of git in
    # most workflows; emitted blank so VS doesn't synthesize one).
    u = []
    u.append('<?xml version="1.0" encoding="utf-8"?>')
    u.append('<Project ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">')
    u.append('</Project>')
    open(path + ".user", "w", newline="\r\n").write("\n".join(u))

    return guid, len(cpps), len(hdrs)

def write_solution(infos):
    sln = []
    sln.append("Microsoft Visual Studio Solution File, Format Version 11.00")
    sln.append("# Visual Studio 2010")
    cpp_guid = "{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}"
    for name, guid, _, _ in infos:
        sln.append(f'Project("{cpp_guid}") = "{name}", "{name}.vcxproj", "{guid}"')
        # Project dependencies block.
        deps = next((d for n, _, _, d in [(p[0], p[2], p[3], p[3]) for p in PROJECTS] if n == name), [])
        # Find original deps tuple
        for pn, _, _, pd in PROJECTS:
            if pn == name:
                deps = pd; break
        if deps:
            sln.append("\tProjectSection(ProjectDependencies) = postProject")
            for d in deps:
                dg = stable_guid(d)
                sln.append(f"\t\t{dg} = {dg}")
            sln.append("\tEndProjectSection")
        sln.append("EndProject")
    sln.append("Global")
    sln.append("\tGlobalSection(SolutionConfigurationPlatforms) = preSolution")
    sln.append("\t\tDebug|Win32 = Debug|Win32")
    sln.append("\t\tRelease|Win32 = Release|Win32")
    sln.append("\tEndGlobalSection")
    sln.append("\tGlobalSection(ProjectConfigurationPlatforms) = postSolution")
    for name, guid, _, _ in infos:
        for cfg in ("Debug", "Release"):
            sln.append(f"\t\t{guid}.{cfg}|Win32.ActiveCfg = {cfg}|Win32")
            sln.append(f"\t\t{guid}.{cfg}|Win32.Build.0 = {cfg}|Win32")
    sln.append("\tEndGlobalSection")
    sln.append("\tGlobalSection(SolutionProperties) = preSolution")
    sln.append("\t\tHideSolutionNode = FALSE")
    sln.append("\tEndGlobalSection")
    sln.append("EndGlobal")
    open(os.path.join(BUILD, "Fiesta.sln"), "w", newline="\r\n").write("\n".join(sln))

def write_props():
    # Property sheet for the Gamebryo SDK location. Users edit the
    # macro to point at their local install. Default to the in-tree
    # ThirdParty/Gamebryo/ which ships read-only.
    p = []
    p.append('<?xml version="1.0" encoding="utf-8"?>')
    p.append('<Project ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">')
    p.append('  <ImportGroup Label="PropertySheets" />')
    p.append('  <PropertyGroup Label="UserMacros">')
    p.append('    <GamebryoRoot>$(SolutionDir)..\\ThirdParty\\Gamebryo</GamebryoRoot>')
    p.append('  </PropertyGroup>')
    p.append('  <PropertyGroup />')
    p.append('  <ItemDefinitionGroup>')
    p.append('    <ClCompile>')
    p.append('      <AdditionalIncludeDirectories>$(GamebryoRoot)\\Include;$(GamebryoRoot)\\NiMain;$(GamebryoRoot)\\NiDX9Renderer;$(GamebryoRoot)\\NiAnimation;$(GamebryoRoot)\\NiMesh;%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>')
    p.append('    </ClCompile>')
    p.append('    <Link>')
    p.append('      <AdditionalLibraryDirectories>$(GamebryoRoot)\\Lib\\$(Configuration);%(AdditionalLibraryDirectories)</AdditionalLibraryDirectories>')
    p.append('    </Link>')
    p.append('  </ItemDefinitionGroup>')
    p.append('  <ItemGroup>')
    p.append('    <BuildMacro Include="GamebryoRoot"><Value>$(GamebryoRoot)</Value></BuildMacro>')
    p.append('  </ItemGroup>')
    p.append('</Project>')
    open(os.path.join(BUILD, "Gamebryo.props"), "w", newline="\r\n").write("\n".join(p))

def main():
    write_props()
    infos = []
    for name, dirs, kind, deps in PROJECTS:
        guid, ncpp, nhdr = write_vcxproj(name, dirs, kind, deps)
        print(f"{name:30s}  guid={guid}  cpp={ncpp:4d}  h={nhdr:4d}")
        infos.append((name, guid, ncpp, nhdr))
    write_solution(infos)
    print(f"\nWrote {len(infos)} projects + Fiesta.sln + Gamebryo.props to {BUILD}/")

if __name__ == "__main__":
    main()

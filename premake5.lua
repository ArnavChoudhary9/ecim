-- premake5.lua generated for ecim project
-- Sets up a workspace and a single C++ console project that uses Eigen from vendor/eigen

workspace "ecim"
   configurations { "Debug", "Release" }
   platforms { "x64", "x86" }
   location ("build")
   startproject "ecim"

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"

   filter {}

project "ecim"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++17"
   targetdir "bin/%{cfg.buildcfg}"
   objdir "obj/%{cfg.buildcfg}"

   -- Limit files to project source and headers (exclude vendor)
   files { "main.cpp", "ecim/**.cpp", "ecim/**.hpp", "ecim/**.h" }

   -- Resolve Eigen include path at load time to avoid passing a function into Premake fields
   local function resolve_eigen_include()
      local env = os.getenv("EIGEN_INCLUDE_DIR")
      if env and env ~= "" then
         print("Using Eigen includes from EIGEN_INCLUDE_DIR: " .. env)
         return env
      end
      local vendorPath = path.getabsolute("vendor/eigen")
      if os.isdir(vendorPath) then
         -- Eigen typically exposes includes in the root; some packages use Eigen/Eigen
         print("Using Eigen includes from vendor: " .. vendorPath)
         return vendorPath
      end
      -- Fallback: still add vendor/eigen (Premake will not validate), but warn the user
      print("Warning: Eigen include directory not found at '" .. vendorPath .. "'. Set EIGEN_INCLUDE_DIR env var if Eigen is elsewhere or initialize the submodule.")
      return "vendor/eigen"
   end

   includedirs {
      "ecim",
      resolve_eigen_include(),
   }

   -- Add system-specific settings
   filter "system:windows"
      systemversion "latest"
      defines { "_CRT_SECURE_NO_WARNINGS" }

   filter "system:linux"
      pic "On"

   filter "configurations:Debug"
      symbols "On"

   filter "configurations:Release"
      optimize "Full"

   filter {}

-- Test project
project "ecim_tests"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++17"
   targetdir "bin/%{cfg.buildcfg}"
   objdir "obj/%{cfg.buildcfg}/tests"

   -- Include test files and all ecim source files (excluding main.cpp)
   files { 
      "tests/test_runner.cpp",
      "tests/test_components.cpp",
      "tests/test_circuits.cpp",
      "tests/test_transient.cpp",
      "ecim/**.cpp", 
      "ecim/**.hpp", 
      "ecim/**.h" 
   }

   includedirs {
      "ecim",
      resolve_eigen_include(),
   }

   -- Add system-specific settings
   filter "system:windows"
      systemversion "latest"
      defines { "_CRT_SECURE_NO_WARNINGS" }

   filter "system:linux"
      pic "On"

   filter "configurations:Debug"
      symbols "On"

   filter "configurations:Release"
      optimize "Full"

   filter {}

-- Helper action to print recommended commands
newaction {
   trigger = "eigen-status",
   description = "Checks for the presence of vendor/eigen and reports recommended steps",
   execute = function()
      local vendorPath = path.getabsolute("vendor/eigen")
      if os.isdir(vendorPath) then
         print("vendor/eigen found at: " .. vendorPath)
      else
         print("vendor/eigen not found. If you use git, run: git submodule update --init --recursive")
         print("Or set EIGEN_INCLUDE_DIR to the path where Eigen is installed.")
      end
   end
}

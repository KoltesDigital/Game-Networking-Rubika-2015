configurations { "Debug", "Release" }
platforms { "Win32", "Win64" }

defines { "GUID_WINDOWS" }

flags {
	"FatalWarnings",
	"MultiProcessorCompile",
	"NoPCH",
	"Unicode",
}

filter "configurations:Debug"
	defines { "DEBUG" }
	flags { "Symbols" }
	targetsuffix "-d"

filter "configurations:Release"
	defines { "NDEBUG" }
	optimize "On"

filter { "platforms:Win32" }
	architecture "x32"
	libdirs { "dep/lib" }
	system "Windows"
	targetdir "bin"

filter { "platforms:Win64" }
	architecture "x64"
	libdirs { "dep/lib64" }
	system "Windows"
	targetdir "bin64"

workspace "DutyCalls"
	language "C++"
	location "build"

project "Engine"
	files {
		"code/engine/**"
	}
	includedirs {
		"code",
		"dep/include",
	}
	location "build/Engine"
	kind "StaticLib"
	rtti "Off"
	vpaths {
		-- engine = "*"
	}

project "Client"
	files {
		"code/client/**",
		"code/mod/**",
	}
	includedirs {
		"code",
		"dep/include",
	}
	links {
		"Engine",
		"winmm",
		"ws2_32",
	}
	location "build/Client"
	kind "ConsoleApp"
	rtti "Off"
	
	filter "configurations:Debug"
		links {
			"enet-d",
			"sfml-graphics-d",
			"sfml-network-d",
			"sfml-system-d",
			"sfml-window-d",
		}

	filter "configurations:Release"
		links {
			"enet",
			"sfml-graphics",
			"sfml-network",
			"sfml-system",
			"sfml-window",
		}

project "Server"
	files {
		"code/server/**",
		"code/mod/**",
	}
	includedirs {
		"code",
		"dep/include",
	}
	links {
		"Engine",
		"winmm",
		"ws2_32",
	}
	location "build/Server"
	kind "ConsoleApp"
	rtti "Off"
	
	filter "configurations:Debug"
		links {
			"enet-d",
			"sfml-graphics-d",
			"sfml-network-d",
			"sfml-system-d",
			"sfml-window-d",
		}

	filter "configurations:Release"
		links {
			"enet",
			"sfml-graphics",
			"sfml-network",
			"sfml-system",
			"sfml-window",
		}

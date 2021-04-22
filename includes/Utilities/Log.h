#pragma once

#include <string>
#include <iomanip>
#include <iostream>
#include <glm/glm.hpp>
#include "Mathf/Mathf.h"
#include <assimp/Importer.hpp>

using string = std::string;

#define TEXT_RED string("\e[91m")
#define TEXT_GREEN string("\e[92m")
#define TEXT_YELLOW string("\e[93m")
#define TEXT_MAGENTA string("\e[95m")
#define TEXT_CYAN string("\e[96m")
#define TEXT_BLACK string("\e[30m")
#define TEXT_DEFAULT string("\e[39m")
#define BG_RED string("\e[41m")
#define BG_YELLOW string("\e[43m")
#define BG_GREEN string("\e[42m")
#define BG_GRAY string("\e[47m")
#define BG_CYAN string("\e[46m")
#define BG_DEFAULT string("\e[49m")

namespace {
	template<typename T>
	void Print(string textColor, string bgColor, T val) {
		std::cout << textColor + bgColor;
		std::cout << val;
		std::cout << TEXT_DEFAULT + BG_DEFAULT;
	};
	template<>
	void Print<glm::vec4>(string textColor, string bgColor, glm::vec4 val) {
		std::cout << textColor + bgColor;
		std::cout << "[vec4 x: " << val.x << ", y: " << val.y << ", z: " << val.z << ", w: " << val.w << "]";
		std::cout << TEXT_DEFAULT + BG_DEFAULT;
	}
	template<>
	void Print<glm::ivec4>(string textColor, string bgColor, glm::ivec4 val) {
		std::cout << textColor + bgColor;
		std::cout << "[ivec4 x: " << val.x << ", y: " << val.y << ", z: " << val.z << ", w: " << val.w << "]";
		std::cout << TEXT_DEFAULT + BG_DEFAULT;
	}
	template<>
	void Print<glm::vec3>(string textColor, string bgColor, glm::vec3 val) {
		std::cout << textColor + bgColor;
		std::cout << "[vec3 x: " << val.x << ", y: " << val.y << ", z: " << val.z << "]";
		std::cout << TEXT_DEFAULT + BG_DEFAULT;
	}
	template<>
	void Print<glm::ivec3>(string textColor, string bgColor, glm::ivec3 val) {
		std::cout << textColor + bgColor;
		std::cout << "[ivec3 x: " << val.x << ", y: " << val.y << ", z: " << val.z << "]";
		std::cout << TEXT_DEFAULT + BG_DEFAULT;
	}
	template<>
	void Print<glm::vec2>(string textColor, string bgColor, glm::vec2 val) {
		std::cout << textColor + bgColor;
		std::cout << "[vec2 x: " << val.x << ", y: " << val.y << "]";
		std::cout << TEXT_DEFAULT + BG_DEFAULT;
	}
	template<>
	void Print<glm::ivec2>(string textColor, string bgColor, glm::ivec2 val) {
		std::cout << textColor + bgColor;
		std::cout << "[ivec2 x: " << val.x << ", y: " << val.y << "]";
		std::cout << TEXT_DEFAULT + BG_DEFAULT;
	}
	template<>
	void Print<glm::mat4>(string textColor, string bgColor, glm::mat4 val) {
		std::cout << std::endl;
		std::cout << textColor + bgColor;
		std::cout << std::fixed << std::setprecision(2) << "[ " << val[0][0] << " " << val[1][0] << " " << val[2][0] << " " << val[3][0] << " ]";
		std::cout << TEXT_DEFAULT + BG_DEFAULT << std::endl;
		std::cout << textColor + bgColor;
		std::cout << std::fixed << std::setprecision(2) << "[ " << val[0][1] << " " << val[1][1] << " " << val[2][1] << " " << val[3][1] << " ]";
		std::cout << TEXT_DEFAULT + BG_DEFAULT << std::endl;
		std::cout << textColor + bgColor;
		std::cout << std::fixed << std::setprecision(2) << "[ " << val[0][2] << " " << val[1][2] << " " << val[2][2] << " " << val[3][2] << " ]";
		std::cout << TEXT_DEFAULT + BG_DEFAULT << std::endl;
		std::cout << textColor + bgColor;
		std::cout << std::fixed << std::setprecision(2) << "[ " << val[0][3] << " " << val[1][3] << " " << val[2][3] << " " << val[3][3] << " ]";
		std::cout << TEXT_DEFAULT + BG_DEFAULT;
	}
	template<>
	void Print<aiMatrix4x4>(string textColor, string bgColor, aiMatrix4x4 val) {
		std::cout << std::endl;
		std::cout << textColor + bgColor;
		std::cout << std::fixed << std::setprecision(2) << "[ " << val[0][0] << " " << val[0][1] << " " << val[0][2] << " " << val[0][3] << " ]";
		std::cout << TEXT_DEFAULT + BG_DEFAULT << std::endl;
		std::cout << textColor + bgColor;
		std::cout << std::fixed << std::setprecision(2) << "[ " << val[1][0] << " " << val[1][1] << " " << val[1][2] << " " << val[1][3] << " ]";
		std::cout << TEXT_DEFAULT + BG_DEFAULT << std::endl;
		std::cout << textColor + bgColor;
		std::cout << std::fixed << std::setprecision(2) << "[ " << val[2][0] << " " << val[2][1] << " " << val[2][2] << " " << val[2][3] << " ]";
		std::cout << TEXT_DEFAULT + BG_DEFAULT << std::endl;
		std::cout << textColor + bgColor;
		std::cout << std::fixed << std::setprecision(2) << "[ " << val[3][0] << " " << val[3][1] << " " << val[3][2] << " " << val[3][3] << " ]";
		std::cout << TEXT_DEFAULT + BG_DEFAULT;
	}
}

class Log {
public:
	template <typename T>
	static void Basic(T t) {
		Print(TEXT_DEFAULT, BG_DEFAULT, t);
		std::cout << std::endl;
	};
	template <typename T, typename... Args>
	static void Basic(T t, Args... args) {
		Print(TEXT_DEFAULT, BG_DEFAULT, t);
		Basic(args...);
	};
	template <typename T>
	static void Error(T t) {
		Print(TEXT_RED, BG_DEFAULT, t);
		std::cout << std::endl;
	};
	template <typename T, typename... Args>
	static void Error(T t, Args... args) {
		Print(TEXT_RED, BG_DEFAULT, t);
		Error(args...);
	};
	template <typename T>
	static void Warning(T t) {
		Print(TEXT_YELLOW, BG_DEFAULT, t);
		std::cout << std::endl;
	};
	template <typename T, typename... Args>
	static void Warning(T t, Args... args) {
		Print(TEXT_YELLOW, BG_DEFAULT, t);
		Warning(args...);
	};
	template <typename T>
	static void Important(T t) {
		Print(TEXT_CYAN, BG_DEFAULT, t);
		std::cout << std::endl;
	};
	template <typename T, typename... Args>
	static void Important(T t, Args... args) {
		Print(TEXT_CYAN, BG_DEFAULT, t);
		Important(args...);
	};
	template <typename T>
	static void Success(T t) {
		Print(TEXT_GREEN, BG_DEFAULT, t);
		std::cout << std::endl;
	};
	template <typename T, typename... Args>
	static void Success(T t, Args... args) {
		Print(TEXT_GREEN, BG_DEFAULT, t);
		Success(args...);
	};
	template <typename T>
	static void ErrorBg(T t) {
		Print(TEXT_BLACK, BG_RED, t);
		std::cout << std::endl;
	};
	template <typename T, typename... Args>
	static void ErrorBg(T t, Args... args) {
		Print(TEXT_BLACK, BG_RED, t);
		ErrorBg(args...);
	};
	template <typename T>
	static void WarningBg(T t) {
		Print(TEXT_BLACK, BG_YELLOW, t);
		std::cout << std::endl;
	};
	template <typename T, typename... Args>
	static void WarningBg(T t, Args... args) {
		Print(TEXT_BLACK, BG_YELLOW, t);
		WarningBg(args...);
	};
	template <typename T>
	static void ImportantBg(T t) {
		Print(TEXT_BLACK, BG_CYAN, t);
		std::cout << std::endl;
	};
	template <typename T, typename... Args>
	static void ImportantBg(T t, Args... args) {
		Print(TEXT_BLACK, BG_CYAN, t);
		ImportantBg(args...);
	};
	template <typename T>
	static void SuccessBg(T t) {
		Print(TEXT_BLACK, BG_GREEN, t);
		std::cout << std::endl;
	};
	template <typename T, typename... Args>
	static void SuccessBg(T t, Args... args) {
		Print(TEXT_BLACK, BG_GREEN, t);
		SuccessBg(args...);
	};
private:
};
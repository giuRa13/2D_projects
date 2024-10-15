#pragma once

#include <string>
#include <string_view>
#include <source_location>
#include <vector>
#include <cassert>

#define ENGINE_LOG(x, ...) ENGINE_LOGGER::Logger::GetInstance().Log(x, __VA_ARGS__);
#define ENGINE_WARN(x, ...) ENGINE_LOGGER::Logger::GetInstance().Warn(x, __VA_ARGS__);
#define ENGINE_ERROR(x, ...) ENGINE_LOGGER::Logger::GetInstance().Error(std::source_location::current(), x, __VA_ARGS__);
#define ENGINE_INIT_LOGS(console, retain) ENGINE_LOGGER::Logger::GetInstance().Init(console, retain);

namespace ENGINE_LOGGER
{
	struct LogEntry
	{
		enum class LogType {INFO, WARN, ERR, NONE};
		LogType type{ LogType::INFO };
		std::string log{ "" };
	};

	class Logger
	{
	private:
		std::vector<LogEntry> m_LogEntries;
		bool m_bLogAdded{ false }, m_bInitialized{ false }, m_bConsoleLog{ true }, m_bRetainLogs{ true };

		Logger() = default;

		struct LogTime
		{
			std::string day, dayNumber, month, year, time;
			LogTime(const std::string& date);
		};

		std::string CurrentDateTime();


	public:
		static Logger& GetInstance();

		~Logger() = default;
		//make logger non-copiable
		Logger(const Logger&) = delete;
		Logger& operator=(const Logger&) = delete;

		void Init(bool consoleLog = true, bool retainLogs = true);

		template <typename... Args>
		void Log(const std::string& message, Args&&... args);

		template <typename... Args>
		void Warn(const std::string& message, Args&&... args);

		template <typename... Args>
		void Error(std::source_location location, const std::string& message, Args&&... args);

	};
}

#include "Logger.inl"
#pragma once

#include <string>
#include <string_view>
#include <source_location>
#include <vector>
#include <cassert>

#define ENGINE_LOG( x, ... ) ENGINE_LOGGER::Logger::GetInstance().Log( x __VA_OPT__(, ) __VA_ARGS__ )
#define ENGINE_WARN( x, ... ) ENGINE_LOGGER::Logger::GetInstance().Warn( x __VA_OPT__(, ) __VA_ARGS__ )
#define ENGINE_ERROR( x, ... ) ENGINE_LOGGER::Logger::GetInstance().Error( std::source_location::current(), x __VA_OPT__(, ) __VA_ARGS__ )
#define ENGINE_MESSAGE( x, ... ) ENGINE_LOGGER::Logger::GetInstance().Message( x __VA_OPT__(, ) __VA_ARGS__ )

#define ENGINE_ASSERT( x ) assert( x )
#define ENGINE_INIT_LOGS( console, retain ) ENGINE_LOGGER::Logger::GetInstance().Init( console, retain )
#define ENGINE_LOG_ADDED() ENGINE_LOGGER::Logger::GetInstance().LogAdded()
#define ENGINE_RESET_ADDED() ENGINE_LOGGER::Logger::GetInstance().ResetLogAdded()
#define ENGINE_GET_LOGS() ENGINE_LOGGER::Logger::GetInstance().GetLogs()
#define ENGINE_CLEAR_LOGS() ENGINE_LOGGER::Logger::GetInstance().ClearLogs()


namespace ENGINE_LOGGER
{

    struct LogEntry
	{
		enum class LogType {INFO, MESSAGE, WARN, ERR, NONE};
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
	    // Make the logger non-copyable
	    Logger( const Logger& ) = delete;
	    Logger& operator=( const Logger& ) = delete;

		void Init(bool consoleLog = true, bool retainLogs = true);

		template <typename... Args>
		void Log(const std::string_view message, Args&&... args);

		template <typename... Args>
		void Message(const std::string_view message, Args&&... args);

		template <typename... Args>
		void Warn(const std::string_view message, Args&&... args);

		template <typename... Args>
		void Error(std::source_location location, const std::string_view message, Args&&... args);
    
		void LuaLog( const std::string_view message );
		void LuaMessage( const std::string_view message );
		void LuaWarn( const std::string_view message );
		void LuaError( const std::string_view message );

		inline void ClearLogs() { m_LogEntries.clear(); }
		inline const std::vector<LogEntry>& GetLogs() { return m_LogEntries; }
		inline void ResetLogAdded() { m_bLogAdded = false; }
		inline const bool LogAdded() const { return m_bLogAdded; }
	};

}

#include "Logger.inl"
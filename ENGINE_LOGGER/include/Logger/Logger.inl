#pragma once

#include "Logger.hpp"
#include <fmt/format.h>
#include <iostream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <memory>

#ifdef _WIN32
#include <Windows.h>
constexpr WORD GREEN = 2;
constexpr WORD RED = 4;
constexpr WORD YELLOW = 6;
constexpr WORD WHITE = 7;
#else
static const char* GREEN = "\033[0;32m";
static const char* YELLOW = "\033[0;33m";
static const char* RED = "\033[0;31m";
static const char* WHITE = "\033[0;30m";
//static const char* CLOSE = "\022[0m";
static const char* RESET = "\033[0m";
#endif

namespace ENGINE_LOGGER
{
    template <typename... Args>
    void Logger::Log( const std::string_view message, Args&&... args )
    {
        assert( m_bInitialized && "The logger must be initialized before it is used!" );

        if ( !m_bInitialized )
        {
            std::cout << "The logger must be initialized before it is used!" << std::endl;
            return;
        }

        std::stringstream ss;
        ss << "ENGINE [INFO]: " << CurrentDateTime() << " - " << fmt::vformat( message, fmt::make_format_args( args... ) );

        if ( m_bConsoleLog )
        {
    #ifdef _WIN32
            HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE );
            SetConsoleTextAttribute( hConsole, GREEN );
            std::cout << ss.str() << "\n";
            SetConsoleTextAttribute( hConsole, WHITE );
    #else
            std::cout << GREEN << ss.str() << RESET << "\n";
    #endif
        }

        if ( m_bRetainLogs )
        {
            m_LogEntries.emplace_back( LogEntry::LogType::INFO, ss.str() );
            m_bLogAdded = true;
        }
    }


    template <typename... Args>
    void Logger::Warn( const std::string_view message, Args&&... args )
    {
        assert( m_bInitialized && "The logger must be initialized before it is used!" );

        if ( !m_bInitialized )
        {
            std::cout << "The logger must be initialized before it is used!" << std::endl;
            return;
        }

        std::stringstream ss;
        ss << "ENGINE [WARN]: " << CurrentDateTime() << " - " << fmt::vformat( message, fmt::make_format_args( args... ) );

        if ( m_bConsoleLog )
        {
    #ifdef _WIN32
            HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE );
            SetConsoleTextAttribute( hConsole, YELLOW );
            std::cout << ss.str() << "\n";
            SetConsoleTextAttribute( hConsole, WHITE );
    #else
            std::cout << YELLOW << ss.str() << RESET << "\n";
    #endif
        }

        if ( m_bRetainLogs )
        {
            m_LogEntries.emplace_back( LogEntry::LogType::WARN, ss.str() );
            m_bLogAdded = true;
        }
    }

    template <typename... Args>
    void Logger::Error( std::source_location location, const std::string_view message, Args&&... args )
    {
        assert( m_bInitialized && "The logger must be initialized before it is used!" );

        if ( !m_bInitialized )
        {
            std::cout << "The logger must be initialized before it is used!" << std::endl;
            return;
        }

        std::stringstream ss;
        ss << "ENGINE [ERROR]: " << CurrentDateTime() << " - " << fmt::vformat( message, fmt::make_format_args( args... ) )
        << "\nFUNC: " << location.function_name() << "\nLINE: " << location.line();

        if ( m_bConsoleLog )
        {
    #ifdef _WIN32
            HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE );
            SetConsoleTextAttribute( hConsole, RED );
            std::cout << ss.str();
            SetConsoleTextAttribute( hConsole, WHITE );
    #else
            std::cout << RED << ss.str() << RESET << "\n\n";
    #endif
        }

        if ( m_bRetainLogs )
        {
            m_LogEntries.emplace_back( LogEntry::LogType::ERR, ss.str() );
            m_bLogAdded = true;
        }
    }

}
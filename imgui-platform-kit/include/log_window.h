
#pragma once

/**
 * @file log_window.h
 * @brief Built-in log window with colour-coded entries, filtering and auto-scroll.
 */

#include <vector>
#include <string>
#include <cstdarg>
#include "imgui.h"
#include "colour_palette.h"
#include "user_interface_window.h"

namespace imgui_kit
{
    /// A single log line: message text and its display colour.
    struct LogEntry
	{
        std::string message; ///< Text of the log line.
        ImVec4 color;        ///< Colour used to render the line (see imgui_kit::colours).
    };

    /**
     * @brief Built-in window that displays colour-coded log messages.
     *
     * The log storage is static and shared: call addLog() from anywhere in
     * the application, before or after a LogWindow instance exists. The
     * window provides a text filter and an auto-scroll option.
     *
     * Register it with UserInterface::addWindow<LogWindow>(), or embed the
     * log UI inside your own window with renderContent().
     */
    class LogWindow final : public UserInterfaceWindow
	{
    private:
        inline static std::vector<LogEntry> logs;
        inline static ImGuiTextFilter filter;
        inline static bool autoScroll = true;
        inline static bool isWindowActive = false;

    public:
        LogWindow();
        /**
         * @brief Appends a printf-style formatted message to the log.
         * @param color Colour used to render the message (see imgui_kit::colours).
         * @param fmt printf-style format string.
         * @param ... Format arguments.
         */
        static void addLog(const ImVec4& color, const char* fmt, ...) IM_FMTARGS(2);
        /// Renders the log window. Called once per frame by the UserInterface.
        void render() override { draw(); }
        /// Returns true while the log window is open and being rendered.
        static bool isActive() { return isWindowActive; }
        /// Returns the number of stored log entries.
        static size_t getLogCount() { return logs.size(); }
        /// Removes all stored log entries.
        static void clearLogs() { logs.clear(); }
        /**
         * @brief Renders the log contents (toolbar, filter and entries) into
         * the current ImGui window.
         *
         * Use this to embed the log UI inside a custom window instead of
         * registering a standalone LogWindow.
         */
        static void renderContent();
        ~LogWindow() override = default;
    private:
        static void clean() { logs.clear(); }
        static void draw();
    };
}

#include <catch2/catch_test_macros.hpp>
#include <string>

#include "test_helpers.h"
#include "log_window.h"

using namespace imgui_kit;
using namespace imgui_kit::testing;

// NOTE: LogWindow::filter (an ImGuiTextFilter) is a private static member
// with no accessor, so its contents cannot be driven directly from a test —
// only ImGui::InputText-style interaction (unavailable headless) can change
// it. These tests exercise the filter/PassFilter render lines with whatever
// text is currently active (empty = matches everything after a clearLogs()
// baseline) rather than asserting on a specific active filter string.

static void resetLogWindowState()
{
    LogWindow::clearLogs();
}

// ─── renderContent(): empty / populated logs ──────────────────────────────────

TEST_CASE("LogWindow: renderContent with empty log renders without crashing", "[logwindow][render]")
{
    ImGuiFrameFixture fixture;
    resetLogWindowState();

    ImGuiFrameFixture::runFrame([] {
        ImGui::Begin("LogWindowTest");
        LogWindow::renderContent();
        ImGui::End();
    });

    CHECK(LogWindow::getLogCount() == 0);
    resetLogWindowState();
}

TEST_CASE("LogWindow: renderContent with one entry renders", "[logwindow][render]")
{
    ImGuiFrameFixture fixture;
    resetLogWindowState();

    LogWindow::addLog(colours::White, "[INFO] single entry");

    ImGuiFrameFixture::runFrame([] {
        ImGui::Begin("LogWindowTest");
        LogWindow::renderContent();
        ImGui::End();
    });

    CHECK(LogWindow::getLogCount() == 1);
    resetLogWindowState();
}

TEST_CASE("LogWindow: renderContent with many mixed-colour entries renders", "[logwindow][render]")
{
    ImGuiFrameFixture fixture;
    resetLogWindowState();

    for (int i = 0; i < 200; ++i)
    {
        const ImVec4& color = (i % 4 == 0) ? colours::Red
                              : (i % 4 == 1) ? colours::Green
                              : (i % 4 == 2) ? colours::Yellow
                                              : colours::White;
        LogWindow::addLog(color, "[LINE] entry number %d", i);
    }

    ImGuiFrameFixture::runFrame([] {
        ImGui::Begin("LogWindowTest");
        LogWindow::renderContent();
        ImGui::End();
    });

    CHECK(LogWindow::getLogCount() == 200);
    resetLogWindowState();
}

// ─── rendering never mutates the log ───────────────────────────────────────────

TEST_CASE("LogWindow: renderContent does not mutate the log", "[logwindow][render]")
{
    ImGuiFrameFixture fixture;
    resetLogWindowState();

    LogWindow::addLog(colours::White, "[INFO] a");
    LogWindow::addLog(colours::Red, "[ERROR] b");

    for (int frame = 0; frame < 3; ++frame)
    {
        ImGuiFrameFixture::runFrame([] {
            ImGui::Begin("LogWindowTest");
            LogWindow::renderContent();
            ImGui::End();
        });
    }

    CHECK(LogWindow::getLogCount() == 2);
    resetLogWindowState();
}

// ─── auto-scroll branch ───────────────────────────────────────────────────────

TEST_CASE("LogWindow: renderContent exercises the auto-scroll branch with many entries", "[logwindow][render]")
{
    // Enough entries to make the scrolling child region's content exceed its
    // view, exercising GetScrollY()/GetScrollMaxY()/SetScrollHereY().
    ImGuiFrameFixture fixture;
    resetLogWindowState();

    for (int i = 0; i < 500; ++i)
        LogWindow::addLog(colours::White, "[INFO] line %d of many", i);

    ImGuiFrameFixture::runFrame([] {
        ImGui::Begin("LogWindowTest");
        LogWindow::renderContent();
        ImGui::End();
    });

    CHECK(LogWindow::getLogCount() == 500);
    resetLogWindowState();
}

// ─── draw(): standalone window ─────────────────────────────────────────────────

TEST_CASE("LogWindow: draw() renders the standalone window via render()", "[logwindow][render]")
{
    ImGuiFrameFixture fixture;
    resetLogWindowState();

    LogWindow window;
    REQUIRE(LogWindow::isActive());
    LogWindow::addLog(colours::White, "[INFO] via draw()");

    ImGuiFrameFixture::runFrame([&] {
        window.render();
    });

    CHECK(LogWindow::isActive());
    CHECK(LogWindow::getLogCount() == 1);
    resetLogWindowState();
}

TEST_CASE("LogWindow: draw() early-returns cleanly when the window is collapsed", "[logwindow][render]")
{
    ImGuiFrameFixture fixture;
    resetLogWindowState();

    LogWindow window;
    LogWindow::addLog(colours::White, "[INFO] should not be visited by renderContent");

    ImGuiFrameFixture::runFrame([&] {
        ImGui::SetNextWindowCollapsed(true, ImGuiCond_Always);
        window.render();
    });

    // A clean, balanced frame (no ImGui assertion) is the proof the
    // early-return branch (Begin() returns false -> End(); return;) ran
    // without leaving Begin/End unbalanced.
    CHECK(LogWindow::getLogCount() == 1);
    resetLogWindowState();
}

// ─── construct-then-add-then-clear across frames ───────────────────────────────

TEST_CASE("LogWindow: interleaved addLog/clearLogs across multiple frames", "[logwindow][render]")
{
    ImGuiFrameFixture fixture;
    resetLogWindowState();

    LogWindow window;

    LogWindow::addLog(colours::White, "[INFO] frame 1 - a");
    ImGuiFrameFixture::runFrame([&] { window.render(); });
    CHECK(LogWindow::getLogCount() == 1);

    LogWindow::addLog(colours::Red, "[ERROR] frame 2 - b");
    ImGuiFrameFixture::runFrame([&] { window.render(); });
    CHECK(LogWindow::getLogCount() == 2);

    LogWindow::clearLogs();
    ImGuiFrameFixture::runFrame([&] { window.render(); });
    CHECK(LogWindow::getLogCount() == 0);

    LogWindow::addLog(colours::Green, "[INFO] frame 4 - c");
    ImGuiFrameFixture::runFrame([&] { window.render(); });
    CHECK(LogWindow::getLogCount() == 1);

    resetLogWindowState();
}

// ─── embedding renderContent() inside a caller-owned window ───────────────────

TEST_CASE("LogWindow: renderContent embeds cleanly inside a custom window", "[logwindow][render]")
{
    ImGuiFrameFixture fixture;
    resetLogWindowState();

    LogWindow::addLog(colours::White, "[INFO] embedded");

    ImGuiFrameFixture::runFrame([] {
        ImGui::Begin("MyCustomPanel");
        ImGui::Text("Custom content above the log.");
        LogWindow::renderContent();
        ImGui::End();
    });

    CHECK(LogWindow::getLogCount() == 1);
    resetLogWindowState();
}

// ─── very long entry renders ────────────────────────────────────────────────────

TEST_CASE("LogWindow: renders a maximum-length entry safely", "[logwindow][render]")
{
    ImGuiFrameFixture fixture;
    resetLogWindowState();

    const std::string longMessage(1000, 'x');
    LogWindow::addLog(colours::White, "%s", longMessage.c_str());

    ImGuiFrameFixture::runFrame([] {
        ImGui::Begin("LogWindowTest");
        LogWindow::renderContent();
        ImGui::End();
    });

    CHECK(LogWindow::getLogCount() == 1);
    resetLogWindowState();
}

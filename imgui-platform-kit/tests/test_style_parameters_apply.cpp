#include <catch2/catch_test_macros.hpp>
#include <vector>

#include "test_helpers.h"
#include "user_interface_parameters.h"
#include "themes.h"

using namespace imgui_kit;
using namespace imgui_kit::testing;

static const std::vector<Theme> ALL_THEMES = {
    Theme::Light, Theme::Dark, Theme::Classic, Theme::AdobeInspired, Theme::BlackDevil,
    Theme::BootstrapDark, Theme::Cherry, Theme::ClassicSteam, Theme::CleanDarkRed, Theme::Comfy,
    Theme::DuckRed, Theme::Darcula, Theme::DarkRuda, Theme::Darky, Theme::DeepDark,
    Theme::DiscordDark, Theme::EnemyMouse, Theme::EverForest, Theme::FutureDark, Theme::Gold,
    Theme::GreenFont, Theme::GreenLeaf, Theme::HazyDark, Theme::MaterialFlat, Theme::Microsoft,
    Theme::Modern, Theme::Moonlight, Theme::Photoshoop, Theme::PurpleComfy, Theme::QuickMinimal,
    Theme::RedFont, Theme::RedOni, Theme::RoundedVisualStudio, Theme::SoftCherry,
    Theme::SonicRiders, Theme::Unreal, Theme::VisualStudio, Theme::Windark,
    Theme::LedSynthmaster,
};

static bool colorsEqual(const ImVec4& a, const ImVec4& b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

// ─── apply() dispatches to the matching themes[] entry ───────────────────────

TEST_CASE("StyleParameters: apply() applies the selected theme for every Theme value", "[parameters][apply]")
{
    ImGuiContextFixture fixture;

    for (const Theme theme : ALL_THEMES)
    {
        INFO("Theme enum value: " << static_cast<int>(theme));

        const StyleParameters style(theme);
        ImGui::GetStyle() = ImGuiStyle{};
        style.apply();
        const ImVec4 viaApply_WindowBg = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];
        const ImVec4 viaApply_Text = ImGui::GetStyle().Colors[ImGuiCol_Text];

        ImGui::GetStyle() = ImGuiStyle{};
        themes.at(theme)();
        const ImVec4 direct_WindowBg = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];
        const ImVec4 direct_Text = ImGui::GetStyle().Colors[ImGuiCol_Text];

        CHECK(colorsEqual(viaApply_WindowBg, direct_WindowBg));
        CHECK(colorsEqual(viaApply_Text, direct_Text));
    }
}

TEST_CASE("StyleParameters: default-constructed params apply the Dark theme", "[parameters][apply]")
{
    ImGuiContextFixture fixture;

    const StyleParameters style;
    REQUIRE(style.theme == Theme::Dark);

    ImGui::GetStyle() = ImGuiStyle{};
    style.apply();
    const ImVec4 appliedWindowBg = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];
    const ImVec4 appliedText = ImGui::GetStyle().Colors[ImGuiCol_Text];

    ImGuiStyle expected{};
    ImGui::StyleColorsDark(&expected);

    CHECK(colorsEqual(appliedWindowBg, expected.Colors[ImGuiCol_WindowBg]));
    CHECK(colorsEqual(appliedText, expected.Colors[ImGuiCol_Text]));
}

// ─── documented limitation: non-theme fields are not forwarded ───────────────

TEST_CASE("StyleParameters: apply() does NOT forward non-theme fields (known limitation)", "[parameters][apply]")
{
    // Pins the documented limitation (see the @warning on StyleParameters in
    // user_interface_parameters.h and wiki/Configuration.md): apply() only
    // applies `theme`; the remaining fields are stored but never forwarded to
    // ImGui::GetStyle(). When apply() is fixed to forward these fields, this
    // test MUST be inverted deliberately, not silently broken.
    ImGuiContextFixture fixture;

    StyleParameters style(Theme::Dark);
    style.windowBgColor = ImVec4(0.123f, 0.456f, 0.789f, 1.0f);
    style.windowPadding = ImVec2(99.0f, 77.0f);
    style.windowRounding = 42.0f;
    style.transparency = 0.5f;
    style.textColor = ImVec4(0.9f, 0.1f, 0.2f, 1.0f);

    ImGui::GetStyle() = ImGuiStyle{};
    style.apply();
    const ImGuiStyle& applied = ImGui::GetStyle();

    CHECK_FALSE(applied.WindowPadding.x == 99.0f);
    CHECK_FALSE(applied.WindowPadding.y == 77.0f);
    CHECK_FALSE(applied.WindowRounding == 42.0f);
    CHECK_FALSE(applied.Alpha == 0.5f);
    CHECK_FALSE(colorsEqual(applied.Colors[ImGuiCol_Text], style.textColor));

    // Instead, the theme's own values won — Dark theme colours, not the sentinels.
    ImGuiStyle expectedDark{};
    ImGui::StyleColorsDark(&expectedDark);
    CHECK(colorsEqual(applied.Colors[ImGuiCol_WindowBg], expectedDark.Colors[ImGuiCol_WindowBg]));
    CHECK(colorsEqual(applied.Colors[ImGuiCol_Text], expectedDark.Colors[ImGuiCol_Text]));
}

// ─── const, repeatable ─────────────────────────────────────────────────────────

TEST_CASE("StyleParameters: apply() is const and repeatable", "[parameters][apply]")
{
    ImGuiContextFixture fixture;

    const StyleParameters style(Theme::Cherry);

    ImGui::GetStyle() = ImGuiStyle{};
    style.apply();
    const ImVec4 first = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];

    style.apply();
    const ImVec4 second = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];

    CHECK(colorsEqual(first, second));
}

// ─── stored fields survive apply() ─────────────────────────────────────────────

TEST_CASE("StyleParameters: stored fields are unchanged by apply()", "[parameters][apply]")
{
    ImGuiContextFixture fixture;

    StyleParameters style(Theme::Gold, ImVec4(0.1f, 0.2f, 0.3f, 1.0f));
    style.windowPadding = ImVec2(11.0f, 22.0f);
    style.windowRounding = 3.5f;
    style.transparency = 0.8f;
    style.textColor = ImVec4(0.4f, 0.5f, 0.6f, 1.0f);

    style.apply();

    CHECK(style.theme == Theme::Gold);
    CHECK(colorsEqual(style.windowBgColor, ImVec4(0.1f, 0.2f, 0.3f, 1.0f)));
    CHECK(style.windowPadding.x == 11.0f);
    CHECK(style.windowPadding.y == 22.0f);
    CHECK(style.windowRounding == 3.5f);
    CHECK(style.transparency == 0.8f);
    CHECK(colorsEqual(style.textColor, ImVec4(0.4f, 0.5f, 0.6f, 1.0f)));
}

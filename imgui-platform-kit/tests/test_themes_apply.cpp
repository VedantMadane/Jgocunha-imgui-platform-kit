#include <catch2/catch_test_macros.hpp>
#include <cstring>
#include <string>
#include <vector>

#include "test_helpers.h"
#include "themes.h"

using namespace imgui_kit;
using namespace imgui_kit::testing;

// Complete list of the 36 theme applier functions (mirrors ALL_THEMES in
// test_themes.cpp, minus Light/Dark/Classic which have no applyXxxTheme —
// they use the ImGui built-ins ImGui::StyleColorsLight/Dark/Classic instead).
struct NamedApplier
{
    const char* name;
    void (*fn)();
};

static const std::vector<NamedApplier> ALL_APPLIERS = {
    {"AdobeInspired", applyAdobeInspiredTheme},
    {"BlackDevil", applyBlackDevilTheme},
    {"BootstrapDark", applyBootstrapDarkTheme},
    {"Cherry", applyCherryTheme},
    {"ClassicSteam", applyClassicSteamTheme},
    {"CleanDarkRed", applyCleanDarkRedTheme},
    {"Comfy", applyComfyTheme},
    {"DuckRed", applyDuckRedTheme},
    {"Darcula", applyDarculaTheme},
    {"DarkRuda", applyDarkRudaTheme},
    {"Darky", applyDarkyTheme},
    {"DeepDark", applyDeepDarkTheme},
    {"DiscordDark", applyDiscordDarkTheme},
    {"EnemyMouse", applyEnemyMouseTheme},
    {"EverForest", applyEverForestTheme},
    {"FutureDark", applyFutureDarkTheme},
    {"Gold", applyGoldTheme},
    {"GreenFont", applyGreenFontTheme},
    {"GreenLeaf", applyGreenLeafTheme},
    {"HazyDark", applyHazyDarkTheme},
    {"MaterialFlat", applyMaterialFlatTheme},
    {"Microsoft", applyMicrosoftTheme},
    {"Modern", applyModernTheme},
    {"Moonlight", applyMoonlightTheme},
    {"Photoshoop", applyPhotoshoopTheme},
    {"PurpleComfy", applyPurpleComfyTheme},
    {"QuickMinimal", applyQuickMinimalTheme},
    {"RedFont", applyRedFontTheme},
    {"RoundedVisualStudio", applyRoundedVisualStudioTheme},
    {"SoftCherry", applySoftCherryTheme},
    {"SonicRiders", applySonicRidersTheme},
    {"Unreal", applyUnrealTheme},
    {"VisualStudio", applyVisualStudioTheme},
    {"Windark", applyWindarkTheme},
    {"LedSynthmaster", applyLedSynthmasterTheme},
    {"RedOni", applyRedOniTheme},
};

static constexpr int EXPECTED_APPLIER_COUNT = 36;

static bool colorsEqual(const ImVec4& a, const ImVec4& b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

static bool styleEqual(const ImGuiStyle& a, const ImGuiStyle& b)
{
    for (int i = 0; i < ImGuiCol_COUNT; ++i)
        if (!colorsEqual(a.Colors[i], b.Colors[i]))
            return false;
    return a.WindowRounding == b.WindowRounding &&
           a.WindowPadding.x == b.WindowPadding.x && a.WindowPadding.y == b.WindowPadding.y &&
           a.FrameRounding == b.FrameRounding;
}

// ─── every applier runs and mutates the default style ────────────────────────

TEST_CASE("Themes: every applier runs and mutates the default style", "[themes][apply]")
{
    ImGuiContextFixture fixture;

    REQUIRE(static_cast<int>(ALL_APPLIERS.size()) == EXPECTED_APPLIER_COUNT);

    for (const auto& applier : ALL_APPLIERS)
    {
        INFO("Theme: " << applier.name);
        ImGui::GetStyle() = ImGuiStyle{};
        const ImGuiStyle defaultStyle = ImGui::GetStyle();

        applier.fn();

        const ImGuiStyle& applied = ImGui::GetStyle();
        const bool mutated =
            !colorsEqual(applied.Colors[ImGuiCol_WindowBg], defaultStyle.Colors[ImGuiCol_WindowBg]) ||
            !colorsEqual(applied.Colors[ImGuiCol_Text], defaultStyle.Colors[ImGuiCol_Text]) ||
            applied.WindowRounding != defaultStyle.WindowRounding;
        REQUIRE(mutated);
    }
}

// ─── exact colour spot-checks (literals verified against themes.cpp) ─────────

TEST_CASE("Themes: exact colour spot-checks for representative themes", "[themes][apply]")
{
    ImGuiContextFixture fixture;

    {
        applyAdobeInspiredTheme();
        const ImGuiStyle& s = ImGui::GetStyle();
        CHECK(s.WindowRounding == 4.0f);
        CHECK(colorsEqual(s.Colors[ImGuiCol_Text], ImVec4(1.0f, 1.0f, 1.0f, 1.0f)));
        CHECK(colorsEqual(s.Colors[ImGuiCol_WindowBg], ImVec4(0.1137254908680916f, 0.1137254908680916f, 0.1137254908680916f, 1.0f)));
    }
    {
        applyCherryTheme();
        const ImGuiStyle& s = ImGui::GetStyle();
        CHECK(s.WindowRounding == 0.0f);
        CHECK(colorsEqual(s.Colors[ImGuiCol_Text], ImVec4(0.8588235378265381f, 0.929411768913269f, 0.886274516582489f, 0.8799999952316284f)));
        CHECK(colorsEqual(s.Colors[ImGuiCol_WindowBg], ImVec4(0.1294117718935013f, 0.1372549086809158f, 0.168627455830574f, 1.0f)));
    }
    {
        applyDarculaTheme();
        const ImGuiStyle& s = ImGui::GetStyle();
        CHECK(s.WindowRounding == 5.300000190734863f);
        CHECK(colorsEqual(s.Colors[ImGuiCol_Text], ImVec4(0.7333333492279053f, 0.7333333492279053f, 0.7333333492279053f, 1.0f)));
        CHECK(colorsEqual(s.Colors[ImGuiCol_WindowBg], ImVec4(0.2352941185235977f, 0.2470588237047195f, 0.2549019753932953f, 0.9399999976158142f)));
    }
    {
        applyDiscordDarkTheme();
        const ImGuiStyle& s = ImGui::GetStyle();
        CHECK(s.WindowRounding == 0.0f);
        CHECK(colorsEqual(s.Colors[ImGuiCol_Text], ImVec4(1.0f, 1.0f, 1.0f, 1.0f)));
        CHECK(colorsEqual(s.Colors[ImGuiCol_WindowBg], ImVec4(0.2117647081613541f, 0.2235294133424759f, 0.2470588237047195f, 1.0f)));
    }
    {
        applyGoldTheme();
        const ImGuiStyle& s = ImGui::GetStyle();
        CHECK(s.WindowRounding == 4.0f);
        CHECK(colorsEqual(s.Colors[ImGuiCol_Text], ImVec4(0.9176470637321472f, 0.9176470637321472f, 0.9176470637321472f, 1.0f)));
        CHECK(colorsEqual(s.Colors[ImGuiCol_WindowBg], ImVec4(0.05882352963089943f, 0.05882352963089943f, 0.05882352963089943f, 1.0f)));
    }
    {
        applyMaterialFlatTheme();
        const ImGuiStyle& s = ImGui::GetStyle();
        CHECK(s.WindowRounding == 0.0f);
        CHECK(colorsEqual(s.Colors[ImGuiCol_Text], ImVec4(0.8313725590705872f, 0.8470588326454163f, 0.8784313797950745f, 1.0f)));
        CHECK(colorsEqual(s.Colors[ImGuiCol_WindowBg], ImVec4(0.1725490242242813f, 0.1921568661928177f, 0.2352941185235977f, 1.0f)));
    }
    {
        applyVisualStudioTheme();
        const ImGuiStyle& s = ImGui::GetStyle();
        CHECK(s.WindowRounding == 0.0f);
        CHECK(colorsEqual(s.Colors[ImGuiCol_Text], ImVec4(1.0f, 1.0f, 1.0f, 1.0f)));
        CHECK(colorsEqual(s.Colors[ImGuiCol_WindowBg], ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f)));
    }
    {
        applyRedOniTheme();
        const ImGuiStyle& s = ImGui::GetStyle();
        CHECK(s.WindowRounding == 6.0f);
        CHECK(colorsEqual(s.Colors[ImGuiCol_Text], ImVec4(0.95f, 0.90f, 0.88f, 1.00f)));
        CHECK(colorsEqual(s.Colors[ImGuiCol_WindowBg], ImVec4(0.08f, 0.06f, 0.06f, 1.00f)));
    }
}

// ─── themes map — all 39 entries callable ─────────────────────────────────────

TEST_CASE("Themes: themes map - all 39 entries are callable and mutate or match a built-in style", "[themes][apply][map]")
{
    ImGuiContextFixture fixture;

    for (const auto& [theme, fn] : themes)
    {
        INFO("Theme enum value: " << static_cast<int>(theme));
        ImGui::GetStyle() = ImGuiStyle{};

        REQUIRE_NOTHROW(fn());
        const ImGuiStyle applied = ImGui::GetStyle();

        if (theme == Theme::Light)
        {
            ImGuiStyle expected{};
            ImGui::StyleColorsLight(&expected);
            CHECK(styleEqual(applied, expected));
        }
        else if (theme == Theme::Dark)
        {
            ImGuiStyle expected{};
            ImGui::StyleColorsDark(&expected);
            CHECK(styleEqual(applied, expected));
        }
        else if (theme == Theme::Classic)
        {
            ImGuiStyle expected{};
            ImGui::StyleColorsClassic(&expected);
            CHECK(styleEqual(applied, expected));
        }
        else
        {
            const ImGuiStyle defaultStyle{};
            const bool mutated = !styleEqual(applied, defaultStyle);
            CHECK(mutated);
        }
    }
}

// ─── idempotence ───────────────────────────────────────────────────────────────

TEST_CASE("Themes: applying a theme is idempotent", "[themes][apply]")
{
    ImGuiContextFixture fixture;

    for (const auto& applier : {std::pair{"Cherry", applyCherryTheme}, std::pair{"Darcula", applyDarculaTheme}, std::pair{"Gold", applyGoldTheme}})
    {
        INFO("Theme: " << applier.first);
        applier.second();
        const ImGuiStyle first = ImGui::GetStyle();
        applier.second();
        const ImGuiStyle second = ImGui::GetStyle();
        CHECK(styleEqual(first, second));
    }
}

// ─── no state leaks between themes ─────────────────────────────────────────────

TEST_CASE("Themes: no state leaks between sequential theme applications", "[themes][apply]")
{
    ImGuiContextFixture fixture;

    struct Pair { const char* aName; void (*a)(); const char* bName; void (*b)(); };
    const std::vector<Pair> pairs = {
        {"Cherry", applyCherryTheme, "Darcula", applyDarculaTheme},
        {"Gold", applyGoldTheme, "DiscordDark", applyDiscordDarkTheme},
        {"MaterialFlat", applyMaterialFlatTheme, "VisualStudio", applyVisualStudioTheme},
    };

    for (const auto& p : pairs)
    {
        INFO(p.aName << " -> " << p.bName);

        ImGui::GetStyle() = ImGuiStyle{};
        p.a();
        p.b();
        const ImGuiStyle afterAThenB = ImGui::GetStyle();

        ImGui::GetStyle() = ImGuiStyle{};
        p.b();
        const ImGuiStyle freshB = ImGui::GetStyle();

        CHECK(styleEqual(afterAThenB, freshB));
    }
}

// ─── appliers do not require a frame ───────────────────────────────────────────

TEST_CASE("Themes: appliers run immediately after context creation, no frame required", "[themes][apply]")
{
    ImGuiContextFixture fixture;
    // No ImGui::NewFrame() has ever been called in this context.
    REQUIRE_NOTHROW(applyCherryTheme());
    REQUIRE_NOTHROW(applyDarculaTheme());
}

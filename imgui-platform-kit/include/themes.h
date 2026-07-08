#pragma once

/**
 * @file themes.h
 * @brief Colour themes for the user interface and a runtime theme selector.
 *
 * Select a theme via imgui_kit::StyleParameters::theme, or let the user switch
 * at runtime with showImGuiKitThemeSelector().
 */

#include "imgui.h"
#include <functional>
#include <string>
#include <unordered_map>

namespace imgui_kit
{
    /**
     * @brief Shows a window that lets the user switch themes at runtime.
     * @param p_open Optional ImGui open flag; when given, the window shows a
     * close button that clears it.
     */
    void showImGuiKitThemeSelector(bool* p_open = nullptr);

    /**
     * @brief Available colour themes.
     *
     * Light, Dark and Classic map to the ImGui built-in styles; the remaining
     * themes are community styles from
     * <a href="https://github.com/Patitotective/ImThemes">ImThemes</a>.
     */
    // theme list from https://github.com/Patitotective/ImThemes
    enum class Theme
    {
        Light,               ///< ImGui built-in light style.
        Dark,                ///< ImGui built-in dark style (default).
        Classic,             ///< ImGui built-in classic style.
        AdobeInspired,       ///< Dark grey theme inspired by Adobe applications.
        BlackDevil,          ///< Black theme with red accents.
        BootstrapDark,       ///< Dark theme with Bootstrap-like blue accents.
        Cherry,              ///< Dark theme with cherry-red accents.
        ClassicSteam,        ///< Olive-green theme reminiscent of classic Steam.
        CleanDarkRed,        ///< Clean dark theme with red highlights.
        Comfy,               ///< Soft, rounded dark theme.
        DuckRed,             ///< Dark theme with warm red accents.
        Darcula,             ///< Theme based on the JetBrains Darcula palette.
        DarkRuda,            ///< Dark blue-grey theme.
        Darky,               ///< Plain dark theme.
        DeepDark,            ///< Very dark, high-contrast theme.
        DiscordDark,         ///< Theme based on Discord's dark palette.
        EnemyMouse,          ///< Dark theme with cyan accents.
        EverForest,          ///< Muted green theme based on the Everforest palette.
        FutureDark,          ///< Futuristic dark theme.
        Gold,                ///< Dark theme with gold accents.
        GreenFont,           ///< Dark theme with green text.
        GreenLeaf,           ///< Green-tinted theme.
        HazyDark,            ///< Hazy, low-contrast dark theme.
        MaterialFlat,        ///< Flat theme following Material Design colours.
        Microsoft,           ///< Light theme inspired by Microsoft's design language.
        Modern,              ///< Modern-looking dark theme.
        Moonlight,           ///< Dark blue-purple night theme.
        Photoshoop,          ///< Dark grey theme inspired by Photoshop.
        PurpleComfy,         ///< Soft dark theme with purple accents.
        QuickMinimal,        ///< Minimal dark theme.
        RedFont,             ///< Dark theme with red text.
        RedOni,              ///< Dark theme with strong red accents.
        RoundedVisualStudio, ///< Visual Studio-like theme with rounded corners.
        SoftCherry,          ///< Softer variant of the Cherry theme.
        SonicRiders,         ///< Colourful theme inspired by Sonic Riders.
        Unreal,              ///< Grey theme inspired by Unreal Engine.
        VisualStudio,        ///< Theme based on the Visual Studio dark palette.
        Windark,             ///< Windows-like dark theme.
        LedSynthmaster       ///< Dark theme inspired by the LED Synthmaster plugin.
	};

    /// Human-readable display name for each Theme value.
    inline const std::unordered_map<Theme, std::string> themeNames = {
	    {Theme::Light, "Light"},
	    {Theme::Dark, "Dark"},
	    {Theme::Classic, "Classic"},
	    {Theme::AdobeInspired, "Adobe Inspired"},
	    {Theme::BlackDevil, "Black Devil"},
	    {Theme::BootstrapDark, "Bootstrap Dark"},
	    {Theme::Cherry, "Cherry"},
	    {Theme::ClassicSteam, "Classic Steam"},
	    {Theme::CleanDarkRed, "Clean Dark Red"},
	    {Theme::Comfy, "Comfy"},
	    {Theme::DuckRed, "Duck Red"},
	    {Theme::Darcula, "Darcula"},
	    {Theme::DarkRuda, "Dark Ruda"},
	    {Theme::Darky, "Darky"},
	    {Theme::DeepDark, "Deep Dark"},
	    {Theme::DiscordDark, "Discord Dark"},
	    {Theme::EnemyMouse, "Enemy Mouse"},
	    {Theme::EverForest, "Ever Forest"},
	    {Theme::FutureDark, "Future Dark"},
	    {Theme::Gold, "Gold"},
	    {Theme::GreenFont, "Green Font"},
	    {Theme::GreenLeaf, "Green Leaf"},
	    {Theme::HazyDark, "Hazy Dark"},
	    {Theme::MaterialFlat, "Material Flat"},
	    {Theme::Microsoft, "Microsoft"},
	    {Theme::Modern, "Modern"},
	    {Theme::Moonlight, "Moonlight"},
	    {Theme::Photoshoop, "Photoshoop"},
	    {Theme::PurpleComfy, "Purple Comfy"},
	    {Theme::QuickMinimal, "Quick Minimal"},
	    {Theme::RedFont, "Red Font"},
	    {Theme::RedOni, "Red Oni"},
	    {Theme::RoundedVisualStudio, "Rounded Visual Studio"},
	    {Theme::SoftCherry, "Soft Cherry"},
	    {Theme::SonicRiders, "Sonic Riders"},
	    {Theme::Unreal, "Unreal"},
	    {Theme::VisualStudio, "Visual Studio"},
	    {Theme::Windark, "Windark"},
	    {Theme::LedSynthmaster, "Led Synthmaster"}
    };

    /**
     * @name Theme appliers
     * Apply the corresponding theme to the current ImGui context by setting
     * ImGui::GetStyle(). Usually called indirectly through the #themes map or
     * StyleParameters::apply() rather than directly. Light, Dark and Classic
     * have no applier here; they use the ImGui built-ins
     * (ImGui::StyleColorsLight/Dark/Classic).
     * @{
     */
    // Stub functions for theme application
    void applyAdobeInspiredTheme();
    void applyBlackDevilTheme();
    void applyBootstrapDarkTheme();
    void applyCherryTheme();
    void applyClassicSteamTheme();
    void applyCleanDarkRedTheme();
    void applyComfyTheme();
    void applyDuckRedTheme();
    void applyDarculaTheme();
    void applyDarkRudaTheme();
    void applyDarkyTheme();
    void applyDeepDarkTheme();
    void applyDiscordDarkTheme();
    void applyEnemyMouseTheme();
    void applyEverForestTheme();
    void applyFutureDarkTheme();
    void applyGoldTheme();
    void applyGreenFontTheme();
    void applyGreenLeafTheme();
    void applyHazyDarkTheme();
    void applyMaterialFlatTheme();
    void applyMicrosoftTheme();
    void applyModernTheme();
    void applyMoonlightTheme();
    void applyPhotoshoopTheme();
    void applyPurpleComfyTheme();
    void applyQuickMinimalTheme();
    void applyRedFontTheme();
    void applyRedOniTheme();
    void applyRoundedVisualStudioTheme();
    void applySoftCherryTheme();
    void applySonicRidersTheme();
    void applyUnrealTheme();
    void applyVisualStudioTheme();
    void applyWindarkTheme();
    void applyLedSynthmasterTheme();
    /** @} */

    /// Maps each Theme value to the function that applies it.
    inline std::unordered_map<Theme, std::function<void()>> themes = {
        {Theme::Light, []() { ImGui::StyleColorsLight(); }},
        {Theme::Dark, []() { ImGui::StyleColorsDark(); }},
        {Theme::Classic, []() { ImGui::StyleColorsClassic(); }},
        {Theme::AdobeInspired, []() { applyAdobeInspiredTheme(); }},
        {Theme::BlackDevil, []() { applyBlackDevilTheme(); }},
        {Theme::BootstrapDark, []() { applyBootstrapDarkTheme(); }},
        {Theme::Cherry, []() { applyCherryTheme(); }},
        {Theme::ClassicSteam, []() { applyClassicSteamTheme(); }},
        {Theme::CleanDarkRed, []() { applyCleanDarkRedTheme(); }},
        {Theme::Comfy, []() { applyComfyTheme(); }},
        {Theme::DuckRed, []() { applyDuckRedTheme(); }},
        {Theme::Darcula, []() { applyDarculaTheme(); }},
        {Theme::DarkRuda, []() { applyDarkRudaTheme(); }},
        {Theme::Darky, []() { applyDarkyTheme(); }},
        {Theme::DeepDark, []() { applyDeepDarkTheme(); }},
        {Theme::DiscordDark, []() { applyDiscordDarkTheme(); }},
        {Theme::EnemyMouse, []() { applyEnemyMouseTheme(); }},
        {Theme::EverForest, []() { applyEverForestTheme(); }},
        {Theme::FutureDark, []() { applyFutureDarkTheme(); }},
        {Theme::Gold, []() { applyGoldTheme(); }},
        {Theme::GreenFont, []() { applyGreenFontTheme(); }},
        {Theme::GreenLeaf, []() { applyGreenLeafTheme(); }},
        {Theme::HazyDark, []() { applyHazyDarkTheme(); }},
        {Theme::MaterialFlat, []() { applyMaterialFlatTheme(); }},
        {Theme::Microsoft, []() { applyMicrosoftTheme(); }},
        {Theme::Modern, []() { applyModernTheme(); }},
        {Theme::Moonlight, []() { applyMoonlightTheme(); }},
        {Theme::Photoshoop, []() { applyPhotoshoopTheme(); }},
        {Theme::PurpleComfy, []() { applyPurpleComfyTheme(); }},
        {Theme::QuickMinimal, []() { applyQuickMinimalTheme(); }},
        {Theme::RedFont, []() { applyRedFontTheme(); }},
        {Theme::RedOni, []() { applyRedOniTheme(); }},
        {Theme::RoundedVisualStudio, []() { applyRoundedVisualStudioTheme(); }},
        {Theme::SoftCherry, []() { applySoftCherryTheme(); }},
        {Theme::SonicRiders, []() { applySonicRidersTheme(); }},
        {Theme::Unreal, []() { applyUnrealTheme(); }},
        {Theme::VisualStudio, []() { applyVisualStudioTheme(); }},
        {Theme::Windark, []() { applyWindarkTheme(); }},
        {Theme::LedSynthmaster, []() { applyLedSynthmasterTheme(); }}
    };
}

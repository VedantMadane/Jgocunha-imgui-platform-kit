#pragma once

/**
 * @file template_window.h
 * @brief Minimal example window; a starting point for your own windows.
 */

#include "user_interface_window.h"
#include "log_window.h"

namespace imgui_kit
{
	/**
	 * @brief Minimal reference example of a UserInterfaceWindow subclass.
	 *
	 * Demonstrates the smallest possible custom window: derive from
	 * UserInterfaceWindow and override render(). Register it with
	 * UserInterface::addWindow<TemplateWindow>(). Copy this class as a
	 * starting point for your own windows.
	 */
	class TemplateWindow : public UserInterfaceWindow
	{
	private:
		/// Renders the template window contents. Called once per frame.
		void render() override;
	};
}

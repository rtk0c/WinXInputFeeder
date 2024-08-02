#include "pch.hpp"

#include "userconfig.hpp"

#include "gamepad.hpp"
#include "utils.hpp"

#include <algorithm>
#include <fstream>

using namespace std::literals;

ConfigGamepad::ConfigGamepad() {
	memset(buttons, 0xFF, kX360ButtonCount * sizeof(KeyCode));
}

std::pair<ConfigGamepad&, size_t> ConfigProfile::AddX360() {
	if (x360Count >= 4) {
		// Return a dummy reference, the SIZE_MAX should already indicate failure
		return { gamepads.front(), SIZE_MAX };
	}

	gamepads.insert(gamepads.begin() + x360Count, ConfigGamepad());
	size_t idx = x360Count++;
	return { gamepads[idx], idx };
}

std::pair<ConfigGamepad&, size_t> ConfigProfile::AddDS4() {
	// TODO
	return { gamepads.front(), 0 };
}

static void WriteJoystick(toml::table& profile, const ConfigJoystick& js) {
	profile.emplace("Type", js.useMouse ? "mouse"s : "keyboard"s);
	profile.emplace("Speed", js.speed);
	profile.emplace("Sensitivity", js.sensitivity);
	profile.emplace("NonLinearSensitivity", js.nonLinear);
	profile.emplace("Deadzone", js.deadzone);
	profile.emplace("InvertXAxis", js.invertXAxis);
	profile.emplace("InvertYAxis", js.invertYAxis);
}

toml::table SaveConfig(const Config& vConfig) {
	toml::table res;

	toml::table general;
	general.emplace("MouseCheckFrequency", vConfig.mouseCheckFrequency);
	res.emplace("General", std::move(general));

	toml::table hotkeys;
	hotkeys.emplace("ShowUI", KeyCodeToString(vConfig.hotkeyShowUI));
	hotkeys.emplace("CaptureCursor", KeyCodeToString(vConfig.hotkeyCaptureCursor));
	res.emplace("HotKeys", std::move(hotkeys));

	toml::table profiles;
	for (auto&& [vName, vProfile] : vConfig.profiles) {
		toml::table profile;

		profile.emplace("XboxCount", vProfile.x360Count);

		toml::array gamepads;
		for (auto& vGamepad : vProfile.gamepads) {
			toml::table gamepad;

			for (int vBtn = 0; vBtn < kX360ButtonCount; ++vBtn) {
				KeyCode vKey = vGamepad.buttons[vBtn];
				if (vBtn != 0xFF)
					gamepad.emplace(X360ButtonToString(static_cast<X360Button>(vBtn)), KeyCodeToString(vKey));
			}

			toml::table lstick;
			WriteJoystick(gamepad, vGamepad.lstick);
			gamepad.emplace("LStick", std::move(lstick));

			toml::table rstick;
			WriteJoystick(gamepad, vGamepad.rstick);
			gamepad.emplace("RStick", std::move(rstick));

			gamepads.push_back(std::move(gamepad));
		}
		profile.emplace("Gamepads", std::move(gamepads));

		profiles.emplace(vName, std::move(profile));
	}
	res.emplace("Profiles", std::move(profiles));

	return res;
}

static KeyCode ReadKeyCode(toml::node_view<const toml::node> t) {
	auto str = t.value<std::string_view>();
	if (str)
		return KeyCodeFromString(str.value()).value_or(0xFF);
	else
		return 0xFF;
}

static void ReadJoystick(toml::node_view<const toml::node> t, ConfigJoystick& js) {
	if (const auto& v = t["Type"];
		v == "keyboard")
		js.useMouse = false;
	else if (v == "mouse")
		js.useMouse = true;

	js.speed = std::clamp(t["Speed"].value_or<float>(1.0f), 0.0f, 1.0f);
	js.sensitivity = t["Sensitivity"].value_or<float>(50.0f);
	js.nonLinear = t["NonLinearSensitivity"].value_or<float>(0.8f);
	js.deadzone = t["Deadzone"].value_or<float>(0.02f);
	js.invertXAxis = t["InvertXAxis"].value_or<bool>(false);
	js.invertYAxis = t["InvertYAxis"].value_or<bool>(false);
}

Config LoadConfig(const toml::table& fConfig) {
	Config config;

	auto fGeneral = fConfig["General"];
	config.mouseCheckFrequency = fGeneral["MouseCheckFrequency"].value_or<int>(75);

	auto fHotkey = fConfig["HotKeys"];
	config.hotkeyShowUI = ReadKeyCode(fHotkey["ShowUI"]);
	config.hotkeyCaptureCursor = ReadKeyCode(fHotkey["CaptureCursor"]);

	auto fProfiles = fConfig["Profiles"].as_table();
	if (fProfiles) for (auto&& [key, val] : *fProfiles) {
		auto e1 = val.as_table();
		if (!e1) continue;
		auto& fProfile = *e1;
		auto fName = key.str();

		ConfigProfile profile;

		profile.x360Count = fProfile["XboxCount"].value_or(0);

		auto fGamepads = fProfile["Gamepads"].as_array();
		if (fGamepads) for (auto& val : *fGamepads) {
			auto e1 = val.as_table();
			if (!e1) continue;
			auto& fGamepad = *e1;

			ConfigGamepad gamepad;

			for (unsigned char i = 0; i < kX360ButtonCount; ++i) {
				gamepad.buttons[i] = ReadKeyCode(fGamepad[X360ButtonToString(static_cast<X360Button>(i))]);
			}
			ReadJoystick(fGamepad["LStick"], gamepad.lstick);
			ReadJoystick(fGamepad["RStick"], gamepad.rstick);

			profile.gamepads.push_back(std::move(gamepad));
		}

		config.profiles.try_emplace(std::string(fName), std::move(profile));
	}

	return config;
}

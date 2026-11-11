#pragma once

#include <array>

namespace optics_ui {

enum class Command { None, ScopePosition, ScopeTilt, Speed, ScopeHelp, Save, Reset, Cancel, ScopeBack };
using ButtonRows = std::array<std::array<Command, 4>, 2>;

inline constexpr ButtonRows scopeRows{{
    {Command::ScopePosition, Command::ScopeTilt, Command::Speed, Command::ScopeHelp},
    {Command::Save, Command::Reset, Command::Cancel, Command::None},
}};
inline constexpr ButtonRows helpRows{{
    {Command::None, Command::None, Command::None, Command::None},
    {Command::ScopeBack, Command::Cancel, Command::None, Command::None},
}};

inline constexpr const char* asset(Command command)
{
    switch (command) {
    case Command::ScopePosition: return "layout\\btn-scopeposition.nif";
    case Command::ScopeTilt: return "layout\\btn-scopetilt.nif";
    case Command::Speed: return "layout\\btn-speed.nif";
    case Command::ScopeHelp: return "layout\\btn-scopehelp.nif";
    case Command::Save: return "ui-common\\btn-save.nif";
    case Command::Reset: return "ui-common\\btn-reset.nif";
    case Command::Cancel: return "layout\\btn-cancel.nif";
    case Command::ScopeBack: return "layout\\btn-scopeback.nif";
    case Command::None: return "";
    }
    return "";
}

inline constexpr float scale = 1.6f;
inline constexpr float buttonScale = 1.4f;
inline constexpr float rowGap = 1.7f;
inline constexpr float verticalGap = 1.0f;

}

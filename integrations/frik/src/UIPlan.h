#pragma once
#include <array>
#include <string_view>
namespace optics_ui {
enum class Page { Home, Dot, Appearance, Presets, Window, WindowAdjust, Geometry, Target, WindowOptions, Options, Help, Scope, ScopeHelp };
enum class Command { None, Dot, Window, FitWindow, Appearance, Presets, Options, Home, Zero, Brightness, Size, Speed, Save, Reset, Cancel, Color, Circle, Day, Night, StoreDay, StoreNight, Position, Depth, Scale, Geometry, Tilt, Roll, Shape, Target, Automatic, Manual, Next, Original, Hand, TargetMode, Help, ScopePosition, ScopeTilt, ScopeHelp, ScopeBack, WindowOptions, WindowReset };
struct Plan { Page page; std::array<std::array<Command,4>,2> rows; };
inline constexpr std::array<Plan,13> plans{{
 {Page::Home,{{{{Command::Dot,Command::Window,Command::Appearance,Command::Presets}},{{Command::Options,Command::Save,Command::Reset,Command::Cancel}}}}},
 {Page::Dot,{{{{Command::Zero,Command::Brightness,Command::Size,Command::None}},{{Command::Speed,Command::Save,Command::Home,Command::Cancel}}}}},
 {Page::Appearance,{{{{Command::Color,Command::Circle,Command::Original,Command::None}},{{Command::Save,Command::Home,Command::Cancel,Command::None}}}}},
 {Page::Presets,{{{{Command::Day,Command::Night,Command::StoreDay,Command::StoreNight}},{{Command::Save,Command::Home,Command::Cancel,Command::None}}}}},
 {Page::Window,{{{{Command::Automatic,Command::Manual,Command::Target,Command::Original}},{{Command::FitWindow,Command::Save,Command::Home,Command::Cancel}}}}},
 {Page::WindowAdjust,{{{{Command::Position,Command::Depth,Command::Scale,Command::Geometry}},{{Command::Speed,Command::Save,Command::Window,Command::Cancel}}}}},
 {Page::Geometry,{{{{Command::Tilt,Command::Roll,Command::Shape,Command::WindowOptions}},{{Command::Speed,Command::Save,Command::FitWindow,Command::Cancel}}}}},
 {Page::Target,{{{{Command::Automatic,Command::Manual,Command::Next,Command::None}},{{Command::Speed,Command::Save,Command::Window,Command::Cancel}}}}},
 {Page::WindowOptions,{{{{Command::Target,Command::Original,Command::WindowReset,Command::Hand}},{{Command::Save,Command::Geometry,Command::Cancel,Command::None}}}}},
 {Page::Options,{{{{Command::Hand,Command::TargetMode,Command::Help,Command::None}},{{Command::Reset,Command::Save,Command::Home,Command::Cancel}}}}},
 {Page::Help,{{{{Command::None,Command::None,Command::None,Command::None}},{{Command::Home,Command::Cancel,Command::None,Command::None}}}}},
 {Page::Scope,{{{{Command::ScopePosition,Command::ScopeTilt,Command::Speed,Command::ScopeHelp}},{{Command::Save,Command::Reset,Command::Cancel,Command::None}}}}},
 {Page::ScopeHelp,{{{{Command::None,Command::None,Command::None,Command::None}},{{Command::ScopeBack,Command::Cancel,Command::None,Command::None}}}}},
}};
inline constexpr const Plan& plan(Page p){for(const auto& q:plans)if(q.page==p)return q;return plans[0];}
inline constexpr const char* asset(Command c){switch(c){
case Command::Dot:return "layout\\btn-dot.nif";
case Command::Window:return "layout\\btn-window.nif";
case Command::FitWindow:return "layout\\btn-fitwindow.nif";
case Command::Appearance:return "layout\\btn-appearance.nif";
case Command::Presets:return "layout\\btn-presets.nif";
case Command::Options:return "layout\\btn-options.nif";
case Command::Home:return "layout\\btn-home.nif";
case Command::Zero:return "layout\\btn-zero.nif";
case Command::Brightness:return "layout\\btn-brightness.nif";
case Command::Size:return "layout\\btn-size.nif";
case Command::Speed:return "layout\\btn-speed.nif";
case Command::Save:return "ui-common\\btn-save.nif";
case Command::Reset:return "ui-common\\btn-reset.nif";
case Command::Cancel:return "layout\\btn-cancel.nif";
case Command::Color:return "layout\\btn-color.nif";
case Command::Circle:return "layout\\btn-circle.nif";
case Command::Day:return "layout\\btn-day.nif";
case Command::Night:return "layout\\btn-night.nif";
case Command::StoreDay:return "layout\\btn-storeday.nif";
case Command::StoreNight:return "layout\\btn-storenight.nif";
case Command::Position:return "layout\\btn-position.nif";
case Command::Depth:return "layout\\btn-depth.nif";
case Command::Scale:return "layout\\btn-scale.nif";
case Command::Geometry:return "layout\\btn-geometry.nif";
case Command::Tilt:return "layout\\btn-tilt.nif";
case Command::Roll:return "layout\\btn-roll.nif";
case Command::Shape:return "layout\\btn-shape.nif";
case Command::Target:return "layout\\btn-target.nif";
case Command::Automatic:return "layout\\btn-automatic.nif";
case Command::Manual:return "layout\\btn-manual.nif";
case Command::Next:return "layout\\btn-next.nif";
case Command::Original:return "layout\\btn-original.nif";
case Command::Hand:return "layout\\btn-hand.nif";
case Command::TargetMode:return "layout\\btn-targetmode.nif";
case Command::Help:return "layout\\btn-help.nif";
case Command::ScopePosition:return "layout\\btn-scopeposition.nif";
case Command::ScopeTilt:return "layout\\btn-scopetilt.nif";
case Command::ScopeHelp:return "layout\\btn-scopehelp.nif";
case Command::ScopeBack:return "layout\\btn-scopeback.nif";
case Command::WindowOptions:return "layout\\btn-windowoptions.nif";
case Command::WindowReset:return "layout\\btn-windowreset.nif";
default:return "";}}
inline constexpr bool windowPage(Page p){return p==Page::Window||p==Page::WindowAdjust||p==Page::Geometry||p==Page::Target||p==Page::WindowOptions;}
inline constexpr float scale=1.6f,buttonScale=1.4f,rowGap=1.7f,verticalGap=1.f,anchorX=-16,anchorY=4,anchorZ=-5;
}

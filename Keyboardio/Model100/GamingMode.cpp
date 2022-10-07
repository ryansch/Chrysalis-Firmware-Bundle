#include "GamingMode.h"
#include "Kaleidoscope-SpaceCadet.h"

#include <Arduino.h>                            // for F, PSTR, __FlashStringHelper
#include <Kaleidoscope-FocusSerial.h>           // for Focus, FocusSerial
#include "kaleidoscope/KeyEvent.h"              // for KeyEvent
#include "kaleidoscope/KeyAddr.h"               // for KeyAddr
#include "kaleidoscope/Runtime.h"               // for Runtime, Runtime_
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult, EventHandlerResult::OK
#include "kaleidoscope/layers.h"                // for Layer, Layer_
#include "kaleidoscope/plugin/LEDControl.h"     // for LEDControl
#include <Kaleidoscope-LED-Palette-Theme.h>     // for LEDPaletteTheme

namespace kaleidoscope {
namespace plugin {

uint8_t GamingMode::gaming_layer;
uint8_t GamingMode::last_led_mode = 255;
bool GamingMode::active = false;

uint16_t GamingMode::map_base_;

void GamingMode::setGamingLayer(uint8_t layer_id) {
  gaming_layer = layer_id;
}

EventHandlerResult GamingMode::onSetup() {
  map_base_ = ::LEDPaletteTheme.reserveThemes(1);

  return EventHandlerResult::OK;
}

EventHandlerResult GamingMode::onLayerChange() {
  // if (::LEDControl.get_mode_index() == led_mode_id_)
  //   ::LEDControl.get_mode<TransientLEDMode>()->onActivate();

  if (!active && Layer.isActive(gaming_layer)) {
    active = true;

    last_led_mode = ::LEDControl.get_mode_index();
    ::LEDControl.set_mode(led_mode_id_);
    ::SpaceCadet.disable();
  } else if (active) {
    active = false;

    if (last_led_mode != 255)
      ::LEDControl.set_mode(last_led_mode);
    ::SpaceCadet.enable();
  }

  return EventHandlerResult::OK;
}

EventHandlerResult GamingMode::onNameQuery() {
  return ::Focus.sendName(F("GamingMode"));
}

EventHandlerResult GamingMode::onFocusEvent(const char *input) {
  return ::LEDPaletteTheme.themeFocusEvent(input, PSTR("gamingmode.map"), map_base_, 0);
}

bool GamingMode::isUninitialized() {
  return ::LEDPaletteTheme.isThemeUninitialized(map_base_, 0);
}

// void GamingMode::updateColorIndexAtPosition(uint8_t layer, uint16_t position, uint8_t palette_index) {
//   if (layer >= max_layers_) return;
//
//   uint16_t index = Runtime.device().led_count * layer + position;
//   ::LEDPaletteTheme.updateColorIndexAtPosition(map_base_, index, palette_index);
// }

void GamingMode::TransientLEDMode::onActivate() {
  if (!Runtime.has_leds)
    return;

  ::LEDPaletteTheme.updateHandler(parent_->map_base_, 0);

  // ::LEDControl.set_all_leds_to(CRGB(0, 0, 170));
  //
  // ::LEDControl.setCrgbAt(KeyAddr(1, 2), CRGB(170, 0, 0)); // w
  // ::LEDControl.setCrgbAt(KeyAddr(2, 1), CRGB(170, 0, 0)); // a
  // ::LEDControl.setCrgbAt(KeyAddr(2, 2), CRGB(170, 0, 0)); // s
  // ::LEDControl.setCrgbAt(KeyAddr(2, 3), CRGB(170, 0, 0)); // d
  //
  // ::LEDControl.setCrgbAt(KeyAddr(3, 7), CRGB(203, 101, 0)); // shift
  // ::LEDControl.setCrgbAt(KeyAddr(2, 0), CRGB(203, 101, 0)); // shift (pgup)
  //
  // ::LEDControl.setCrgbAt(KeyAddr(0, 7), CRGB(0, 160, 0)); // control
  // ::LEDControl.setCrgbAt(KeyAddr(3, 0), CRGB(0, 160, 0)); // control (pgdn)
  //
  // ::LEDControl.setCrgbAt(KeyAddr(1, 7), CRGB(0, 160, 160)); // space (backspace)
  // ::LEDControl.setCrgbAt(KeyAddr(2, 7), CRGB(0, 160, 160)); // alt (cmd)
}

void GamingMode::TransientLEDMode::refreshAt(KeyAddr key_addr) {
  ::LEDPaletteTheme.refreshAt(parent_->map_base_, 0, key_addr);
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::GamingMode GamingMode;

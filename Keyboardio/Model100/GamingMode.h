#pragma once

#include "kaleidoscope/KeyEvent.h"                       // for KeyEvent
#include "kaleidoscope/KeyAddr.h"                        // for KeyAddr
#include "kaleidoscope/plugin.h"                         // for Plugin
#include "kaleidoscope/event_handler_result.h"           // for EventHandlerResult
#include "kaleidoscope/plugin/AccessTransientLEDMode.h"  // for AccessTransientLEDMode
#include "kaleidoscope/plugin/LEDMode.h"                 // for LEDMode
#include "kaleidoscope/plugin/LEDModeInterface.h"        // for LEDModeInterface

namespace kaleidoscope {
namespace plugin {

class GamingMode : public Plugin,
                   public LEDModeInterface,
                   public AccessTransientLEDMode {
  public:
    GamingMode(void) {}

    void setGamingLayer(uint8_t layer_id);

    EventHandlerResult onSetup();
    EventHandlerResult onLayerChange();

    EventHandlerResult onNameQuery();
    EventHandlerResult onFocusEvent(const char *input);

    static bool isUninitialized();
    // static void updateColorIndexAtPosition(uint8_t layer, uint16_t position, uint8_t palette_index);

  class TransientLEDMode : public LEDMode {
    public:
      // Please note that storing the parent ptr is only required
      // for those LED modes that require access to
      // members of their parent class. Most LED modes can do without.
      //
      explicit TransientLEDMode(const GamingMode *parent)
        : parent_(parent) {}

    protected:
      friend class GamingMode;

      void onActivate() final;
      void refreshAt(KeyAddr key_addr) final;

    private:
      const GamingMode *parent_;
  };

  private:
    static uint8_t gaming_layer;
    static uint8_t last_led_mode;
    static bool active;

    static uint16_t map_base_;
};

}  // namespace plugin
}  // namespace kaleidoscope

extern kaleidoscope::plugin::GamingMode GamingMode;

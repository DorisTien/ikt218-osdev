#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <cstdint>

namespace UiAOS
{
    namespace IO
    {
        using keyboard_callback = void (*)(uint8_t, void *);

        class Keyboard
        {
        public:
            static void hook_keyboard(keyboard_callback cb, void *ctx);
            static char scancode_to_ascii(uint8_t scancode);

        private:
        };
    }
}

#endif // KEYBOARD_H

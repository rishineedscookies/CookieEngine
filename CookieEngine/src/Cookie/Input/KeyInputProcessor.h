#pragma once
#include "Cookie/Input/InputProcessor.h"

namespace Cookie {
    
    class COOKIE_API KeyInputProcessor : public InputProcessor
    {
    public:
        KeyInputProcessor(int keycode)
            : m_Keycode(keycode) {}

        virtual bool GetBoolValue() override;

    private:
        int m_Keycode;
    };

}
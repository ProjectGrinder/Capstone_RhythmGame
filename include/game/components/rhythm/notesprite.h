#pragma once

namespace Game::Rhythm
{
    struct NoteSprite
    {
        const AssetsRecord *normal;
        const AssetsRecord *accent;
        const AssetsRecord *rain;
        const AssetsRecord *disabled;
        explicit NoteSprite() :
        normal(load_sprite("img/rhythm/base_normal.dds", "normal", 200, 40)),
        accent(load_sprite("img/rhythm/base_accent.dds", "accent", 200, 40)),
        rain(load_sprite("img/rhythm/base_rain.dds", "rain", 200, 20)),
        disabled(load_sprite("img/rhythm/base_disabled.dds", "disabled", 200, 40))
        {}
    };
}
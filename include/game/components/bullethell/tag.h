#pragma once

namespace Game::BulletHell
{
    enum TagType
    {
        NO_TAG,
    };

    struct Tag
    {
        TagType type;
        Tag() : type(NO_TAG)
        {}
        explicit Tag(const TagType type) : type(type)
        {}
    };
}
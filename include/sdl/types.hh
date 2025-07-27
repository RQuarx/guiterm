#pragma once
#include <expected>
#include <cstdint>
#include <utility>
#include <format>
#include <string>
#include <tuple>
#include <SDL3/SDL_pixels.h>


namespace sdl
{
    using size_pair = std::pair<int32_t, int32_t>;
    using void_or_msg = std::expected<void, std::string>;

    class Window;

    struct alignas(alignof(int32_t)) Color
    {
        using tuple_color = std::tuple<int8_t, int8_t, int8_t, int8_t>;

        uint8_t r = 0x00;
        uint8_t g = 0x00;
        uint8_t b = 0x00;
        uint8_t a = 0xFF;

        Color( void ) = default;
        Color( const SDL_FColor &p_color ) :
            r(p_color.r),
            g(p_color.g),
            b(p_color.b),
            a(p_color.a)
        {}
        Color( const uint8_t &p_r, const uint8_t &p_g, const uint8_t &p_b ) :
            r(p_r),
            g(p_g),
            b(p_b)
        {}


        [[nodiscard]]
        auto to_sdl( void ) const -> SDL_FColor
        {
            return {
                static_cast<float>(r),
                static_cast<float>(g),
                static_cast<float>(b),
                static_cast<float>(a)
            };
        }


    };
}

constexpr inline auto
operator""_rgb( unsigned long long val ) -> sdl::Color
{
    return sdl::Color {
        static_cast<uint8_t>((val >> 16) & 0xFF),
        static_cast<uint8_t>((val >> 8) & 0xFF),
        static_cast<uint8_t>(val & 0xFF)
    };
}


constexpr inline auto
operator""_rgba( unsigned long long val ) -> sdl::Color
{
    return sdl::Color {{
        static_cast<float>((val >> 24) & 0xFF),
        static_cast<float>((val >> 16) & 0xFF),
        static_cast<float>((val >> 8) & 0xFF),
        static_cast<float>(val & 0xFF)
    }};
}


#define COL_TO_SDL( p_color ) \
    p_color.r, p_color.g, p_color.b, p_color.a


template<>
struct std::formatter<sdl::Color> : std::formatter<std::string>
{
    constexpr auto
    parse( format_parse_context &p_ctx ) const
    { return p_ctx.begin(); }


    auto
    format( const sdl::Color &p_color, format_context &p_ctx ) const
    {
        char buff[10] { 0 };
        std::snprintf(buff, 10, "#%02X%02X%02X%02X",
                      p_color.r, p_color.g, p_color.b, p_color.a);
        return std::format_to(p_ctx.out(), "{}", buff);
    }
};
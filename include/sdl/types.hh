#pragma once
#include <expected>
#include <cstdint>
#include <utility>
#include <format>
#include <string>
#include <tuple>
#include <SDL3/SDL_pixels.h>
#include <json/value.h>


namespace sdl
{
    using size_pair = std::pair<int32_t, int32_t>;
    using void_or_msg = std::expected<void, std::string>;

    class Window;

    struct alignas(alignof(int32_t)) Color
    {
        using tuple_color = std::tuple<int8_t, int8_t, int8_t, int8_t>;

        uint8_t r { 0x00 };
        uint8_t g { 0x00 };
        uint8_t b { 0x00 };
        uint8_t a { 0xFF };

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
        Color( const uint8_t &p_r, const uint8_t &p_g,
               const uint8_t &p_b, const uint8_t &p_a ) :
            r(p_r),
            g(p_g),
            b(p_b),
            a(p_a)
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


        [[nodiscard]]
        static auto from_string( const std::string &p_hex ) -> Color
        {
            if (p_hex.empty()) return {};

            static constexpr auto parse_hex_component {
            [](const std::string &p_hex )
            {
                try {
                    return static_cast<uint8_t>(std::stoi(p_hex, nullptr, 16));
                } catch (...) {
                    return static_cast<uint8_t>(255);
                }
            } };

            const std::string hex { p_hex.starts_with('#')
                                  ? p_hex.substr(1) : p_hex };
            const size_t len { hex.length() };

            if (len == 3 || len == 4) {
                return {
                    parse_hex_component({ 2, hex[0] }),
                    parse_hex_component({ 2, hex[1] }),
                    parse_hex_component({ 2, hex[2] }),
                    parse_hex_component(len == 4
                                       ? std::string( 2, hex[3] ) : "ff")
                };
            } else if (len == 6 || len == 8) {
                return {
                    parse_hex_component(hex.substr(0, 2)),
                    parse_hex_component(hex.substr(2, 2)),
                    parse_hex_component(hex.substr(4, 2)),
                    parse_hex_component(len == 8
                                        ? hex.substr(6, 2) : "ff"),
                };
            }

            return {};
        }


        [[nodiscard]]
        static auto from_json( const Json::Value &p_json ) -> Color
        { return from_string(p_json.asString()); }
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
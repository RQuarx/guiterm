#pragma once
#include <SDL3/SDL_render.h>
#include "sdl/types.hh"

#define NONNULL( p_char_str ) \
    p_char_str == nullptr ? "nullptr" : std::string(p_char_str)


namespace sdl
{
    /**
     * @brief Retrieve the last error message on the current thread.
     *
     * This function will return the string "No error message present"
     * if the error message from SDL_GetError is a nullptr or empty.
    */
    [[nodiscard]]
    auto get_error( void ) -> std::string;


    void quit( void );


    auto init( const std::string &p_app_name,
               const std::string &p_app_version,
               const std::string &p_app_id ) -> void_or_msg;
}
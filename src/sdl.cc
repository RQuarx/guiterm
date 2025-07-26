#include <SDL3/SDL_init.h>
#include "sdl.hh"


namespace sdl
{
    auto
    get_error( void ) -> std::string
    {
        std::string err { NONNULL(SDL_GetError()) };
        if (err == "nullptr" || err.empty())
            return "No error message present";

        SDL_ClearError();
        return err;
    }


    void
    quit( void )
    { SDL_Quit(); }


    auto
    init( const std::string &p_app_name,
          const std::string &p_app_version,
          const std::string &p_app_id ) -> void_or_msg
    {
        if (!SDL_Init(SDL_INIT_VIDEO))
            return std::unexpected(get_error());
        atexit(quit);

        if (!SDL_SetAppMetadata(p_app_name.c_str(),
                                p_app_version.c_str(),
                                p_app_id.c_str()))
            return std::unexpected(get_error());

        return {};
    }

}
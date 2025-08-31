#pragma once
#include <memory>
#include <string>
#include <SDL3/SDL_render.h>
#include "types.hh"

class Logger;


namespace sdl
{
    class Window
    {
    public:
        enum FuncRetval : uint8_t
        {
            RETURN_SKIP,
            RETURN_FAILURE,
            RETURN_SUCCESS,
            RETURN_CONTINUE,
            RETURN_FORCE,
        };

        Window( const std::shared_ptr<Logger> &p_logger,
                const std::string             &p_title,
                const size_pair               &p_size );
        ~Window( void );

        auto operator=( const Window &p_copy ) -> Window & = delete;
        Window        ( const Window &p_copy )             = delete;

        Window( Window &&p_other ) noexcept;
        auto operator=( Window &&p_other ) noexcept -> Window &;


        void set_color( const std::pair<Color, Color> &p_colors );


        auto get_window_px_size( void ) -> std::pair<int32_t, int32_t>;


        auto run( void ) -> int32_t;

    private:
        std::shared_ptr<Logger> m_logger;

        SDL_Window   *m_window;
        SDL_Renderer *m_render;

        Color m_bg;
        Color m_fg;


        auto event_stream( const SDL_Event &p_event ) -> FuncRetval;
        auto render_stream( void ) -> FuncRetval;
    };
}
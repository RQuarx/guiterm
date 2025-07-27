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
        };

        Window( const std::shared_ptr<Logger> &p_logger,
                const std::string             &p_title,
                const size_pair               &p_size );
        ~Window( void );

        auto operator=( const Window &p_copy ) -> Window & = delete;
        Window        ( const Window &p_copy )             = delete;

        Window( Window &&p_other ) noexcept;
        auto operator=( Window &&p_other ) noexcept -> Window &;


        /**
         * @brief Sets the window background to @p p_color .
         */
        void set_bg( Color p_color );


        auto run( void ) -> int32_t;

    private:
        std::shared_ptr<Logger> m_logger;

        SDL_Window   *m_window;
        SDL_Renderer *m_render;

        Color m_bg_color;


        auto event_stream( const SDL_Event &p_event ) -> FuncRetval;
        auto render_stream( void ) -> FuncRetval;
    };
}
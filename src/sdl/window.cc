#include <SDL3/SDL_timer.h>
#include "sdl/window.hh"
#include "log.hh"
#include "sdl.hh"

using sdl::Window;


Window::Window( const std::shared_ptr<Logger> &p_logger,
                const std::string             &p_title,
                const size_pair               &p_size ) :
    m_logger(p_logger)
{
    m_logger->log(info, "Creating a new sdl::Window");
    if (!SDL_CreateWindowAndRenderer(p_title.c_str(),
                                     p_size.first, p_size.second,
                                     SDL_WINDOW_RESIZABLE | SDL_WINDOW_TRANSPARENT,
                                     &m_window, &m_render)) {
        m_logger->log(error, "Failed to create window and renderer: {}",
                      sdl::get_error());
        return;
    }

    // if (!SDL_SetRenderVSync(m_render, 1))
    //     m_logger->log(error, "Failed to set render vysnc: {}",
    //                   sdl::get_error());
}


Window::~Window( void )
{
    m_logger->log(info, "Destroying an sdl::Window instance");
    if (m_render != nullptr) {
        SDL_DestroyRenderer(m_render);
        m_render = nullptr;
    }
    if (m_window != nullptr) {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }
}


Window::Window( Window &&p_other ) noexcept :
    m_logger(std::move(p_other.m_logger)),
    m_window(p_other.m_window),
    m_render(p_other.m_render)
{
    p_other.m_window = nullptr;
    p_other.m_render = nullptr;
}


auto
Window::operator=( Window &&p_other ) noexcept -> Window &
{
    if (this != &p_other) {
        if (m_render) SDL_DestroyRenderer(m_render);
        if (m_window) SDL_DestroyWindow(m_window);

        m_logger = std::move(p_other.m_logger);
        m_window = p_other.m_window;
        m_render = p_other.m_render;

        p_other.m_window = nullptr;
        p_other.m_render = nullptr;
    }

    return *this;
}


void
Window::set_color( const std::pair<Color, Color> &p_colors )
{
    m_bg = p_colors.first;
    m_fg = p_colors.second;

    m_logger->log(debug, "Setting the window colours to fg: {}, bg: {}",
                  m_bg, m_fg);
}


auto
Window::run( void ) -> int32_t
{
    constexpr uint32_t PERIODIC_RENDER = 250;

    bool running      { true  };
    bool fail         { false };
    bool needs_render { true  };
    SDL_Event event;
    uint64_t last_render_event { SDL_GetTicks() };

    while (running) {
        while (SDL_PollEvent(&event)) {
            FuncRetval retval { event_stream(event) };

            switch (retval) {
            case RETURN_CONTINUE:
                needs_render = true;
                continue;
            case RETURN_SKIP: break;
            case RETURN_FAILURE:
                running = false;
                fail    = true;
                break;
            case RETURN_SUCCESS:
                running = false;
                break;
            case RETURN_FORCE:
                needs_render = true;
                break;
            }
            break;
        }

        if (!running) break;
        uint64_t now { SDL_GetTicks() };
        if (now - last_render_event >= PERIODIC_RENDER) {
            last_render_event = now;
            needs_render = true;
        }

        if (!needs_render) continue;

        FuncRetval retval { render_stream() };
        m_logger->log(debug, "{}", get_window_px_size());
        needs_render = false;

        switch (retval) {
        case RETURN_FORCE: [[fallthrough]];
        case RETURN_CONTINUE: [[fallthrough]];
        case RETURN_SKIP: continue;

        case RETURN_FAILURE:
            running = false;
            fail    = true;
            break;
        case RETURN_SUCCESS:
            running = false;
            break;
        }
    }

    return fail;
}


auto
Window::event_stream( const SDL_Event &p_event ) -> FuncRetval
{
    switch (p_event.type) {
    case SDL_EVENT_QUIT:
        return RETURN_SUCCESS;
    case SDL_EVENT_WINDOW_RESIZED:
        return render_stream();
    default:
        return RETURN_SKIP;
    }
}


auto
Window::render_stream( void ) -> FuncRetval
{
    SDL_SetRenderDrawColor(m_render, COL_TO_SDL(m_bg));
    SDL_RenderClear(m_render);

    SDL_RenderPresent(m_render);
    m_logger->log(debug, "Render");
    return RETURN_CONTINUE;
}


auto
Window::get_window_px_size( void ) -> std::pair<int32_t, int32_t>
{
    int32_t w;
    int32_t h;

    SDL_GetWindowSizeInPixels(m_window, &w, &h);
    return { w, h };
}
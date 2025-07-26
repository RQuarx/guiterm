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
                                     SDL_WINDOW_RESIZABLE,
                                     &m_window, &m_render)) {
        m_logger->log(error, "Failed to create window and renderer: {}",
                      sdl::get_error());
        return;
    }

    if (!SDL_SetRenderVSync(m_render, 1))
        m_logger->log(error, "Failed to set render vysnc: {}",
                      sdl::get_error());
}


Window::~Window( void )
{
    m_logger->log(info, "Destorying an sdl::Window instance");
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
Window::set_bg( sdl::Color p_color )
{
    m_bg_color = std::move(p_color);
    m_logger->log(debug, "Setting sdl::Window's background color to {}",
                  m_bg_color);
}
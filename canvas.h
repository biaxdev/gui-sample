#pragma once

#include <gtkmm/drawingarea.h>
#include <glibmm/main.h>
#include <string>
#include <vector>

class CCanvas : public Gtk::DrawingArea
{
public:
    CCanvas();
    virtual ~CCanvas();

protected:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

private:
    int fifo_fd_;
    sigc::connection io_watch_conn_;
    uint16_t raw_distance_mm_ = 0;

    bool on_fifo_read(Glib::IOCondition condition);
    std::vector<std::string> format_all_units() const;
};
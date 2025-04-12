#pragma once

#include <gtkmm/drawingarea.h>
#include <gtkmm/button.h>
#include <gtkmm/box.h>
#include <glibmm/main.h>

class CCanvas : public Gtk::DrawingArea
{
public:
    CCanvas();
    virtual ~CCanvas();

    void set_unit_index(int index) { unit_index_ = index; queue_draw(); }

protected:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

private:
    int fifo_fd_;
    sigc::connection io_watch_conn_;
    uint16_t raw_distance_mm_ = 0;
    int unit_index_ = 0;

    bool on_fifo_read(Glib::IOCondition condition);
    std::string format_distance() const;
};
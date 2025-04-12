#include "canvas.h"
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <iomanip>

CCanvas::CCanvas()
{
    fifo_fd_ = open("/tmp/fifo", O_RDONLY | O_NONBLOCK);
    if (fifo_fd_ < 0) {
        perror("open fifo");
        return;
    }

    io_watch_conn_ = Glib::signal_io().connect(
        sigc::mem_fun(*this, &CCanvas::on_fifo_read),
        fifo_fd_,
        Glib::IO_IN
    );
}

CCanvas::~CCanvas()
{
    if (io_watch_conn_.connected())
        io_watch_conn_.disconnect();
    if (fifo_fd_ >= 0)
        close(fifo_fd_);
}

bool CCanvas::on_fifo_read(Glib::IOCondition condition)
{
    if (condition & Glib::IO_IN) {
        uint16_t value = 0;
        ssize_t len = read(fifo_fd_, &value, sizeof(value));
        if (len == sizeof(value)) {
            raw_distance_mm_ = value;
            queue_draw();
        }
    }
    return true;
}

std::string CCanvas::format_distance() const
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);

    switch (unit_index_) {
        case 0: // mm
            oss << raw_distance_mm_ << " mm";
            break;
        case 1: // cm
            oss << (raw_distance_mm_ / 10.0) << " cm";
            break;
        case 2: // m
            oss << (raw_distance_mm_ / 1000.0) << " m";
            break;
        case 3: // inch
            oss << (raw_distance_mm_ / 25.4) << " in";
            break;
        default:
            oss << raw_distance_mm_ << " mm";
    }

    return oss.str();
}

bool CCanvas::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
    Gtk::Allocation allocation = get_allocation();
    double width = allocation.get_width();
    double height = allocation.get_height();

    cr->set_source_rgb(1.0, 1.0, 1.0); cr->paint();

    cr->set_source_rgb(0.1, 0.1, 0.1);
    cr->select_font_face("Sans", Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_BOLD);
    cr->set_font_size(30.0);

    std::string text = format_distance();
    cr->move_to(20, height / 2);
    cr->show_text(text);

    return true;
}
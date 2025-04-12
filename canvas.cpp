#include "canvas.h"
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>

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

std::vector<std::string> CCanvas::format_all_units() const
{
    std::ostringstream oss;
    std::vector<std::string> units;

    oss << std::fixed << std::setprecision(2);
    oss.str(""); oss.clear();
    oss << (raw_distance_mm_ / 25.4); units.push_back(oss.str() + " in");

    oss.str(""); oss.clear();
    oss << (raw_distance_mm_ / 10.0); units.push_back(oss.str() + " cm");

    oss.str(""); oss.clear();
    oss << (raw_distance_mm_ / 1000.0); units.push_back(oss.str() + " m");

    oss.str(""); oss.clear();
    oss << raw_distance_mm_; units.push_back(oss.str() + " mm");

    return units;
}

bool CCanvas::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
    Gtk::Allocation allocation = get_allocation();
    double width = allocation.get_width();
    double height = allocation.get_height();

    cr->set_source_rgb(1.0, 1.0, 1.0);
    cr->paint();

    std::vector<std::string> labels = format_all_units();

    double box_width = width - 40;
    double box_height = 60;
    double x = 20;
    double y = 20;
    double spacing = 20;
    double radius = 12;

    for (const auto& label : labels)
    {
        // Draw rounded rectangle
        cr->set_source_rgb(0.2, 0.7, 0.9);
        cr->begin_new_path();
        cr->arc(x + box_width - radius, y + radius, radius, -M_PI_2, 0);
        cr->arc(x + box_width - radius, y + box_height - radius, radius, 0, M_PI_2);
        cr->arc(x + radius, y + box_height - radius, radius, M_PI_2, M_PI);
        cr->arc(x + radius, y + radius, radius, M_PI, 3 * M_PI_2);
        cr->close_path();
        cr->fill();

        // Draw label
        cr->set_source_rgb(1, 1, 1);
        cr->select_font_face("Sans", Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_BOLD);
        cr->set_font_size(20);

        cr->move_to(x + 20, y + box_height / 2 + 7);
        cr->show_text(label);

        y += box_height + spacing;
    }

    return true;
}
#include "dashboard_window.h"

DashboardWindow::DashboardWindow()
{
    set_title("Sensor Dashboard");
    set_default_size(300, 500);

    title_.set_markup("<b><big>Distance Sensor</big></b>");
    title_.set_halign(Gtk::ALIGN_CENTER);
    title_.set_margin_bottom(10);

    box_.set_orientation(Gtk::ORIENTATION_VERTICAL);
    box_.set_spacing(10);
    box_.set_margin_top(20);
    box_.set_margin_bottom(20);
    box_.set_margin_start(20);
    box_.set_margin_end(20);

    box_.pack_start(title_, Gtk::PACK_SHRINK);
    box_.pack_start(canvas_);

    add(box_);
    show_all_children();
}
#pragma once

#include <gtkmm/window.h>
#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include "canvas.h"

class DashboardWindow : public Gtk::Window
{
public:
    DashboardWindow();

private:
    Gtk::Box box_;
    Gtk::Label title_;
    CCanvas canvas_;
};
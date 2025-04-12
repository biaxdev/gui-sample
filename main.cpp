#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include <gtkmm/button.h>
#include <gtkmm/box.h>
#include "canvas.h"

class DashboardWindow : public Gtk::Window
{
public:
    DashboardWindow()
    {
        set_title("VL53L1X Distance Viewer");
        set_default_size(400, 200);

        button_.set_label("Change Unit");
        button_.signal_clicked().connect(sigc::mem_fun(*this, &DashboardWindow::on_button_clicked));

        box_.set_orientation(Gtk::ORIENTATION_VERTICAL);
        box_.pack_start(canvas_);
        box_.pack_start(button_, Gtk::PACK_SHRINK);

        add(box_);
        show_all_children();
    }

private:
    Gtk::Box box_;
    CCanvas canvas_;
    Gtk::Button button_;
    int unit_index_ = 0;

    void on_button_clicked()
    {
        unit_index_ = (unit_index_ + 1) % 4;
        canvas_.set_unit_index(unit_index_);
    }
};

int main(int argc, char** argv)
{
    auto app = Gtk::Application::create(argc, argv, "org.example.vl53l1x");

    DashboardWindow window;
    return app->run(window);
}
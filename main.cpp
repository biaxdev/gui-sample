#include <gtkmm/application.h>
#include "dashboard_window.h"

int main(int argc, char** argv)
{
    auto app = Gtk::Application::create(argc, argv, "org.sensor.dashboard");
    DashboardWindow window;
    return app->run(window);
}
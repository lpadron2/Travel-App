#include <Application.h>
#include <FL/Enumerations.H>
#include <FL/Fl_Scroll.H>
#include <bobcat_ui/bobcat_ui.h>
#include <bobcat_ui/button.h>
#include <bobcat_ui/dropdown.h>
#include <bobcat_ui/textbox.h>
#include <bobcat_ui/window.h>
#include <string>
#include <sstream>
#include <fstream>

using namespace bobcat;
using namespace std;

Application::Application() {
    initData();
    initInterface();
}

void Application::handleClick(bobcat::Widget *sender) {}

void Application::initData() {
    ifstream inputFile;
    string line;

    inputFile.clear();

    inputFile.open("./assets/vertices.csv");
    
    while(!inputFile.eof())
    {
        while(inputFile.peek() == ' ')
        {
            inputFile.get();
        }
        getline(inputFile, line);

        cities.append(new Vertex(line));
    }

    inputFile.close();
}

void Application::initInterface() {
    window = new Window(100, 100, 400, 400, "Flight Planner");

    start = new Dropdown(20, 40, 360, 25, "Starting Point");
    dest = new Dropdown(20, 100, 360, 25, "Destination");

    for(int i = 0; i < cities.size(); i++)
    {
        start ->add(cities[i] ->data);
        dest ->add(cities[i]->data);
    }

    search = new Button(20, 150, 360, 25, "Search");
    ON_CLICK(search, Application::handleClick);

    results = new Fl_Scroll(20, 170, 360, 180, "Results");
    results->align(FL_ALIGN_BOTTOM_LEFT);
    results->box(FL_THIN_DOWN_BOX);

    window->show();
}
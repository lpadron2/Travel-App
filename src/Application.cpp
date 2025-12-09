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

void Application::handleClick(bobcat::Widget *sender) {

    results->clear();
    window->redraw();
    
    int startIndex = start->value();
    int destIndex = dest->value();

    Waypoint *path;

    if (choice->text() == "Least number of stops") {
        path = g.bfs(cities[startIndex], cities[destIndex]);

        system("clear");

        if (path) {
            cout << "We found a path" << endl;
            Waypoint *temp = path;
            int y = results->y() + 10;
            while (temp != nullptr) {
                results->add(new TextBox(40, y, 300, 25, temp->vertex->data));
                y += 40;
                if (temp->parent != nullptr) {
                    results->add(new TextBox(
                        40, y, 300, 25,
                        "    Flight time: " + to_string(temp->weight) + " hours")); 
                    y += 40;
                }
                cout << temp->vertex->data << " " << temp->partialCost << endl;
                temp = temp->parent;

                window->redraw();
            }
        } else {
            cout << "There is no path" << endl;
        }
    }
    
    else if (choice->text() == "Cheapest price"){
        path = g.ucsMoney(cities[startIndex], cities[destIndex]);

        system("clear");

        if (path) {
            cout << "We found a path" << endl;
            Waypoint *temp = path;
            int y = results->y() + 10;
            while (temp != nullptr) {
                results->add(new TextBox(40, y, 300, 25, temp->vertex->data));
                y += 40;
                if (temp->parent != nullptr) {
                    results->add(new TextBox(40, y, 300, 25, "    Flight cost: $ " + to_string(temp->price))); 
                    y += 40;
                }
                cout << temp->vertex->data << " " << temp->partialCostMoney << endl;
                temp = temp->parent;

                window->redraw();
            }
        } else {
            cout << "There is no path" << endl;
        }
    }
    else if(choice->text() == "Shortest travel time")
    {
        path = g.ucs(cities[startIndex], cities[destIndex]);

        system("clear");

        if (path) {
            cout << "We found a path" << endl;
            Waypoint *temp = path;
            int y = results->y() + 10;
            while (temp != nullptr) {
                results->add(new TextBox(40, y, 300, 25, temp->vertex->data));
                y += 40;
                if (temp->parent != nullptr) {
                    results->add(new TextBox(
                        40, y, 300, 25,
                        "    Flight time: " + to_string(temp->weight) + " hours")); 
                    y += 40;
                }
                cout << temp->vertex->data << " " << temp->partialCost << endl;
                temp = temp->parent;

                window->redraw();
            }
        } else {
            cout << "There is no path" << endl;
        }
    }

}

void Application::initData() {
    ifstream inputFile;
    istringstream stringData;
    string line;

    int v1, v2, dest, price;

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

    for(int i = 0; i < cities.size(); i++)
    {
        g.addVertex(cities[i]);
    }

    inputFile.clear();

    inputFile.open("./assets/edges.csv");

    while(!inputFile.eof())
    {
        while(inputFile.peek() == ' ')
        {
            inputFile.get();
        }
        getline(inputFile, line);

        if(inputFile.good())
        {
            stringData.clear();
            stringData.str(line);

            while(stringData.good())
            {
                string token;

                if(getline(stringData, token, ','))
                {
                    v1 = stoi(token);
                }
                if(getline(stringData, token, ','))
                {
                    v2 = stoi(token);
                }
                if(getline(stringData, token, ','))
                {
                    dest = stoi(token);
                }
                if(getline(stringData, token, ','))
                {
                    price = stoi(token);
                }

                g.addEdge(cities[v1], cities[v2], dest, price);
            }
            
        }
    }

    inputFile.close();
}

void Application::initInterface() {
    window = new Window(100, 100, 400, 600, "Flight Planner");

    start = new Dropdown(20, 40, 360, 25, "Starting Point");
    dest = new Dropdown(20, 100, 360, 25, "Destination");
    choice = new Dropdown(20,160, 360, 25, "Preference" );

    for(int i = 0; i < cities.size(); i++)
    {
        start ->add(cities[i] ->data);
        dest ->add(cities[i]->data);
    }


    choice = new Dropdown(20,160,360,25,"Preference");
    choice->add("Cheapest price");
    choice->add("Shortest travel time");
    choice->add("Least number of stops");


    search = new Button(20, 220, 360, 25, "Search");
    ON_CLICK(search, Application::handleClick);


    results = new Fl_Scroll(20, 300, 360, 180, "Results");

    results->align(FL_ALIGN_TOP_LEFT);
    results->box(FL_THIN_UP_BOX);

    window->show();
}
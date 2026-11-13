#include <gtk/gtk.h>
#include <stdlib.h>
#include "domains.h"
#include "methods.h"
#include "stopping_criterion.h"
#include <random>
#include <thread>
#include <chrono>


using vec = std::vector<double>;

vec points[2026];
int count = 0;

vec p1 = vec{-6.0, -5.0};
vec p2 = vec{5.0, 5.0};

void *area_pointer = nullptr;

double f1(const vec& x) 
{
    if (x.size() != 2) 
	{
        throw std::invalid_argument("f1 expects 2 dimensions");
    }

    return x[0] * x[0] + x[1] * x[1];
}

double f2(const vec& x)
{
    if (x.size() != 2) 
	{
        throw std::invalid_argument("f2 expects 2 dimensions");
    }

    return 100.0 * std::pow(x[1] - x[0] * x[0], 2)
         + std::pow(1.0 - x[0], 2);
}

double f3(const vec& x) 
{
    if (x.size() != 2) 
	{
        throw std::invalid_argument("f3 expects 2 dimensions");
    }

    return std::pow(x[0] - 2.0, 2)
         + 2.0 * std::pow(x[1] + 1.0, 2);
}

double f4(const vec& x) 
{
    if (x.size() != 3) 
	{
        throw std::invalid_argument("f4 expects 3 dimensions");
    }

    return std::pow(x[0] - 1.0, 2)
         + std::pow(x[1] + 2.0, 2)
         + std::pow(x[2] - 0.5, 2);
}



gboolean draw
(
    GtkWidget *widget,
    cairo_t *cr,
    gpointer data
)
{
    if(count <= 1)
        return FALSE;

    int w = gtk_widget_get_allocated_width(widget);
    int h = gtk_widget_get_allocated_height(widget);
    int x;
    int y;

    for (int i = 0; i < count - 1; i++) {
        x = ( points[i][0] - p1[0] ) * ( w ) / ( p2[0] - p1[0] );
        y = ( points[i][1] - p1[1] ) * ( h ) / ( p2[1] - p1[1] );
        cairo_move_to(cr, x, y);

        x = ( points[i + 1][0] - p1[0] ) * ( w ) / ( p2[0] - p1[0] ); 
        y = ( points[i + 1][1] - p1[1] ) * ( h ) / ( p2[1] - p1[1] );
        cairo_line_to(cr, x, y);
    }

    cairo_stroke(cr);
    return FALSE;
}


gboolean update(gpointer data)
{
    GtkWidget *area = GTK_WIDGET(data);

    gtk_widget_queue_draw(area);

    return G_SOURCE_CONTINUE;
}


gpointer activate
(
    GtkApplication *app,
    gpointer user_data
)
{
    GtkWidget *window;
    window = gtk_application_window_new(app);

    GtkWidget *area = gtk_drawing_area_new();    
    area_pointer = area;
    g_signal_connect(
        area,
        "draw",
        G_CALLBACK(draw),
        nullptr
    );

    gtk_container_add(
        GTK_CONTAINER(window),
        area
    );    

    //g_timeout_add(500, update, area);

    gtk_widget_show_all(window);
    
    return area;
}


void ex1()
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
	// std::mt19937 gen(std::random_device{}());
	std::mt19937 gen(67);

    RandomSearch method(2000, gen);

    
	std::vector<std::shared_ptr<StoppingCriterion>> stopping_criteria;
	stopping_criteria.emplace_back(std::make_shared<FuncVal>(0.001));

    OptimizationContext opt_ctxt;
    opt_ctxt.domain = std::make_shared<Rect>(p1, p2);
    opt_ctxt.function = [](const vec& x) {
                            return f2(x);
                        };
    opt_ctxt.stopping_criteria = stopping_criteria;

    method.setup_optimization(opt_ctxt);

    while( !method.is_done() )
    {
        method.optimization_step();
        vec point = method.get_search_res().point;
        points[count++] = point;
        update(area_pointer);
    }    

    std::cout << '\n' << method.get_search_res() << '\n';
}


int main
(
    int argc,
    char **argv
)
{
    GtkApplication *app;

    app = gtk_application_new(
            nullptr,
            G_APPLICATION_DEFAULT_FLAGS
    );

    
    g_signal_connect(
        app,
        "activate",
        G_CALLBACK (activate), 
        nullptr
    );

    std::thread calculator(ex1);

    g_application_run(
        G_APPLICATION (app),
        argc,
        argv
    );
    
    g_object_unref(app);
    
    calculator.join();

    return 0;
}

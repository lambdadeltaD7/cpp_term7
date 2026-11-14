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
std::vector<vec> poly;
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


void draw_rect
(
    cairo_t *cr,
    int w, int h,
    double offset_frac
)
{
    int x1 = w * offset_frac;
    int y1 = h * ( 1 - offset_frac );
    int x2 = w * ( 1 - offset_frac );
    int y2 = h * offset_frac;

    cairo_move_to(cr, x1, y1);
    cairo_line_to(cr, x1, y2);

    cairo_move_to(cr, x1, y2);
    cairo_line_to(cr, x2, y2);

    cairo_move_to(cr, x2, y2);
    cairo_line_to(cr, x2, y1);

    cairo_move_to(cr, x2, y1);
    cairo_line_to(cr, x1, y1);
}


gboolean draw_rand
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

    double offset_frac = 0.15;
    draw_rect(
        cr,
        w, h,
        offset_frac
    );

    int x;
    int y;
    int radius = 5;
    
    for (int i = 0; i < count - 1; i++) {
        x = ( points[i][0] - p1[0] ) * ( w * (1 - 2 * offset_frac) )
                / ( p2[0] - p1[0] ) + w * offset_frac;

        y = ( points[i][1] - p1[1] ) * ( h * (1 - 2 * offset_frac) )
                / ( p2[1] - p1[1] ) + h * offset_frac;

        cairo_move_to(cr, x, y);


        x = ( points[ i + 1 ][0] - p1[0] ) * ( w * (1 - 2 * offset_frac) ) 
                / ( p2[0] - p1[0] ) + w * offset_frac; 

        y = ( points[ i + 1 ][1] - p1[1] ) * ( h * (1 - 2 * offset_frac) ) 
                / ( p2[1] - p1[1] ) + h * offset_frac;

        cairo_line_to(cr, x, y);
    }
    
    cairo_stroke(cr);


    for (int i = 0; i < count; ++i)
    {
        x = ( points[i][0] - p1[0] ) * ( w * (1 - 2 * offset_frac) )
                / ( p2[0] - p1[0] ) + w * offset_frac;

        y = ( points[i][1] - p1[1] ) * ( h * (1 - 2 * offset_frac) )
                / ( p2[1] - p1[1] ) + h * offset_frac;

        cairo_move_to(cr, x, y);

        cairo_arc(cr, x, y, radius, 0, 2 * M_PI);
        cairo_set_source_rgb(cr, 1.0, 0.0, 0.0);
        cairo_fill(cr);
    }

    cairo_stroke(cr);
    return FALSE;
}




gboolean draw_deform
(
    GtkWidget *widget,
    cairo_t *cr,
    gpointer data
)
{

    int x;
    int y;
    
    int n = poly.size();

    int w = gtk_widget_get_allocated_width(widget);
    int h = gtk_widget_get_allocated_height(widget);

    double offset_frac = 0.15;
    draw_rect(
        cr,
        w, h,
        offset_frac
    );

    for (int i = 0; i < n; i++) {
        x = ( poly[i][0] - p1[0] ) * ( w * (1 - 2 * offset_frac) ) / ( p2[0] - p1[0] ) + w * offset_frac;
        y = ( poly[i][1] - p1[1] ) * ( h * (1 - 2 * offset_frac) ) / ( p2[1] - p1[1] ) + h * offset_frac;
        cairo_move_to(cr, x, y);

        x = ( poly[ ( i + 1 ) % n ][0] - p1[0] ) * ( w * (1 - 2 * offset_frac) ) / ( p2[0] - p1[0] ) + w * offset_frac;
        y = ( poly[ ( i + 1 ) % n ][1] - p1[1] ) * ( h * (1 - 2 * offset_frac) ) / ( p2[1] - p1[1] ) + h * offset_frac;
        cairo_line_to(cr, x, y);
    }

    cairo_stroke(cr);

    int radius = 5;
    for (int i = 0; i < n; ++i)
    {
        x = ( poly[i][0] - p1[0] ) * ( w * (1 - 2 * offset_frac) ) / ( p2[0] - p1[0] ) + w * offset_frac;
        y = ( poly[i][1] - p1[1] ) * ( h * (1 - 2 * offset_frac) ) / ( p2[1] - p1[1] ) + h * offset_frac;

        cairo_move_to(cr, x, y);

        cairo_arc(cr, x, y, radius, 0, 2 * M_PI);
        cairo_set_source_rgb(cr, 1.0, 0.0, 0.0);
        cairo_fill(cr);
    }

    cairo_stroke(cr);
    return FALSE;
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

    gtk_container_add(
        GTK_CONTAINER(window),
        area
    );    

    gtk_widget_show_all(window);
    
    return area;
}


void ex1()
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    g_signal_connect(
        area_pointer,
        "draw",
        G_CALLBACK(draw_rand),
        nullptr
    );

    std::mt19937 gen(std::random_device{}());
	//std::mt19937 gen(7777777777);

    RandomSearch method(200000, gen);

    
	std::vector<std::shared_ptr<StoppingCriterion>> stopping_criteria;
	stopping_criteria.emplace_back(std::make_shared<FuncVal>(0.00000001));

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
        if( method.is_updated() )
        {
            vec point = method.get_search_res().point;
            points[count++] = point;
            gtk_widget_queue_draw(GTK_WIDGET(area_pointer));
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
        }
    }    

    std::cout << '\n' << method.get_search_res() << '\n';
}


void ex2()
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    g_signal_connect(
        area_pointer,
        "draw",
        G_CALLBACK(draw_deform),
        nullptr
    );

	double alpha = 1.0;
	double beta = 0.5;
	double gamma = 2;
	double vertex_dist = 1;

    DeformPoly method(30, alpha, beta, gamma, vertex_dist); 

	std::vector<std::shared_ptr<StoppingCriterion>> stopping_criteria;
	stopping_criteria.emplace_back(std::make_shared<FuncVal>(0.0000000000001));

    OptimizationContext opt_ctxt;
    opt_ctxt.domain = std::make_shared<Rect>(p1,p2);
    opt_ctxt.function = [](const vec& x) {
                            return f3(x);
                        };
    opt_ctxt.stopping_criteria = stopping_criteria;

    method.setup_optimization(opt_ctxt);

    while( !method.is_done() )
    {
        method.optimization_step();
        poly.clear();
        poly = method.get_poly();
        gtk_widget_queue_draw(GTK_WIDGET(area_pointer));
        std::this_thread::sleep_for(std::chrono::seconds(1));
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

    std::thread calculator(ex2);

    g_application_run(
        G_APPLICATION (app),
        argc,
        argv
    );
    
    g_object_unref(app);
    
    calculator.join();

    return 0;
}

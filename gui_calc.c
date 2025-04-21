#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "calc.h"

GtkWidget *entry;
GtkWidget *label;

// Función para agregar texto al entry
void append_text(GtkWidget *widget, gpointer text) {
    const char *current = gtk_entry_get_text(GTK_ENTRY(entry));
    char new_text[256];
    snprintf(new_text, sizeof(new_text), "%s%s", current, (char *)text);
    gtk_entry_set_text(GTK_ENTRY(entry), new_text);
}

// Limpiar el entry
void clear_entry(GtkWidget *widget, gpointer data) {
    gtk_entry_set_text(GTK_ENTRY(entry), "");
}

// Calcular el resultado
void calculate(GtkWidget *widget, gpointer data) {
    const char *input = gtk_entry_get_text(GTK_ENTRY(entry));

    struct ast *result = parse_expression(input);
    if (result) {
        gtk_label_set_text(GTK_LABEL(label), result_buffer);
    } else {
        gtk_label_set_text(GTK_LABEL(label), "Error en la expresión.");
    }
}

// Mostrar la ayuda
void show_help(GtkWidget *widget, gpointer data) {
    print_help();
    gtk_label_set_text(GTK_LABEL(label), "Consulta la ayuda en la terminal.");
}

// Función para cargar estilos CSS
void apply_css(GtkWidget *widget, const char *class_name) {
    GtkStyleContext *context = gtk_widget_get_style_context(widget);
    gtk_style_context_add_class(context, class_name);
}

// Main
int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Cargar CSS
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider,

	".ventana-principal {"
    "  background-color: #818181;"  // Fondo oscuro
    "}"

    ".contenedor-principal {"
    "  background-color: #818181;"  // Fondo oscuro del VBox
    "}"

    ".boton-numerico {"
    "  background-color: #FFC38D;"
    "  background-image: linear-gradient(to bottom, #FFC38D, #FFC795);"
    "  color: white;"
    "  font-size: 18px;"
    "  border: 2px solid white;"
    "  border-radius: 5px;"
	"  margin: 2px;"
    "}"

    ".boton-operador {"
    "  background-color: #F59F3F;"
    "  background-image: linear-gradient(to bottom, #F59F3F, #F6A348);"
    "  color: white;"
    "  font-size: 18px;"
    "  border: 2px solid white;"
    "  border-radius: 5px;"
	"  margin: 2px;"
    "}"

    ".boton-funcion {"
    "  background-color: #F59F3F;"
    "  background-image: linear-gradient(to bottom, #F59F3F, #F6A348);"
    "  color: white;"
    "  font-size: 18px;"
    "  border: 2px solid white;"
    "  border-radius: 5px;"
	"  margin: 2px;"
    "}"

    ".boton-comparativo {"
    "  background-color: #F59F3F;"
    "  background-image: linear-gradient(to bottom, #F59F3F, #F6A348);"
    "  color: white;"
    "  font-size: 18px;"
    "  border: 2px solid white;"
    "  border-radius: 5px;"
	"  margin: 2px;"
    "}"

    ".boton-especial {"
    "  background-color: #F59F3F;"
    "  background-image: linear-gradient(to bottom, #F59F3F, #F6A348);"
    "  font-size: 20px;"
	"  font-weight: bold;"
    "  border: 2px solid white;"
    "  border-radius: 10px;"
	"  margin: 2px;"
    "}"

	".entry-estilizado {"
    "  font-size: 24px;"  // Aumenta la fuente del Entry
    "  padding: 10px;"
    "  border: 2px solid #2980b9;"
    "  border-radius: 8px;"
    "}"

    ".label-estilizado {"
    "  font-size: 20px;"  // Aumenta la fuente del Label
    "  font-weight: bold;"
    "  color: #FFFFFF;"
    "}", -1, NULL);

    // Agregar el proveedor de estilos al contexto de la pantalla
    GdkDisplay *display = gdk_display_get_default();
    GdkScreen *screen = gdk_display_get_default_screen(display);
    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    // Ventana principal
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Chris Calculadora");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 600);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    apply_css(window, "ventana-principal");
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Caja principal
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    apply_css(vbox, "contenedor-principal");

    // Entrada de texto y etiqueta
    entry = gtk_entry_new();
	gtk_widget_set_size_request(entry, 350, 50);  // Aumenta el tamaño del entry
	apply_css(entry, "entry-estilizado");  // Aplica estilo con CSS
	gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 5);
    label = gtk_label_new("Resultado: ");
	gtk_widget_set_size_request(label, 350, 50);  // Aumenta el tamaño del label
	apply_css(label, "label-estilizado");  // Aplica estilo con CSS
	gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 5);

    // Crear una cuadrícula para los botones
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);

    // Botones numéricos y operadores básicos
    char *buttons[4][4] = {
        {"7", "8", "9", "/"},
        {"4", "5", "6", "*"},
        {"1", "2", "3", "-"},
        {"(", "0", ")", "+"}
    };

    // Crear e insertar los botones en la cuadrícula
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            GtkWidget *btn = gtk_button_new_with_label(buttons[i][j]);
            g_signal_connect(btn, "clicked", G_CALLBACK(append_text), buttons[i][j]);
            gtk_grid_attach(GTK_GRID(grid), btn, j, i, 1, 1);
            // Aplicar estilos dependiendo del tipo de botón
        	if (isdigit(buttons[i][j][0])) {  // Si es un número (0-9)
        	    apply_css(btn, "boton-numerico");
        	} else {  // Si es un operador (+, -, *, /, (, ))
        	    apply_css(btn, "boton-operador");
        	}
        }
    }

    // Fila para funciones especiales
    GtkWidget *btn_sin = gtk_button_new_with_label("sin");
    GtkWidget *btn_cos = gtk_button_new_with_label("cos");
    GtkWidget *btn_tan = gtk_button_new_with_label("tan");
    GtkWidget *btn_log = gtk_button_new_with_label("log");
    GtkWidget *btn_exp = gtk_button_new_with_label("exp");
    GtkWidget *btn_sqrt = gtk_button_new_with_label("sqrt");
    GtkWidget *btn_abs = gtk_button_new_with_label("|");

    // Conectar funciones matemáticas
    g_signal_connect(btn_sin, "clicked", G_CALLBACK(append_text), "sin(");
    g_signal_connect(btn_cos, "clicked", G_CALLBACK(append_text), "cos(");
    g_signal_connect(btn_tan, "clicked", G_CALLBACK(append_text), "tan(");
    g_signal_connect(btn_log, "clicked", G_CALLBACK(append_text), "log(");
    g_signal_connect(btn_exp, "clicked", G_CALLBACK(append_text), "exp(");
    g_signal_connect(btn_sqrt, "clicked", G_CALLBACK(append_text), "sqrt(");
    g_signal_connect(btn_abs, "clicked", G_CALLBACK(append_text), "|");
    apply_css(btn_sin, "boton-funcion");
    apply_css(btn_cos, "boton-funcion");
    apply_css(btn_tan, "boton-funcion");
    apply_css(btn_log, "boton-funcion");
    apply_css(btn_exp, "boton-funcion");
    apply_css(btn_sqrt, "boton-funcion");
    apply_css(btn_abs, "boton-funcion");

    // Operadores lógicos
    GtkWidget *btn_gt = gtk_button_new_with_label(">");
    GtkWidget *btn_lt = gtk_button_new_with_label("<");
    GtkWidget *btn_eq = gtk_button_new_with_label("==");
    GtkWidget *btn_ne = gtk_button_new_with_label("<>");
    GtkWidget *btn_ge = gtk_button_new_with_label(">=");
    GtkWidget *btn_le = gtk_button_new_with_label("<=");

	// Funciones especiales
    g_signal_connect(btn_gt, "clicked", G_CALLBACK(append_text), ">");
    g_signal_connect(btn_lt, "clicked", G_CALLBACK(append_text), "<");
    g_signal_connect(btn_eq, "clicked", G_CALLBACK(append_text), "==");
    g_signal_connect(btn_ne, "clicked", G_CALLBACK(append_text), "<>");
    g_signal_connect(btn_ge, "clicked", G_CALLBACK(append_text), ">=");
    g_signal_connect(btn_le, "clicked", G_CALLBACK(append_text), "<=");
    apply_css(btn_gt, "boton-comparativo");
    apply_css(btn_lt, "boton-comparativo");
    apply_css(btn_eq, "boton-comparativo");
    apply_css(btn_ne, "boton-comparativo");
    apply_css(btn_ge, "boton-comparativo");
    apply_css(btn_le, "boton-comparativo");

    // Botones de ayuda, limpiar y calcular
    GtkWidget *btn_help = gtk_button_new_with_label("help");
    GtkWidget *btn_clear = gtk_button_new_with_label("C");
    GtkWidget *btn_equals = gtk_button_new_with_label("=");

    // Conectar los botones de ayuda, limpiar y calcular
    g_signal_connect(btn_help, "clicked", G_CALLBACK(show_help), NULL);
    g_signal_connect(btn_clear, "clicked", G_CALLBACK(clear_entry), NULL);
    g_signal_connect(btn_equals, "clicked", G_CALLBACK(calculate), NULL);
    apply_css(btn_help, "boton-especial");
    apply_css(btn_clear, "boton-especial");
    apply_css(btn_equals, "boton-especial");

    // Agregar las funciones matemáticas a la cuadrícula
    gtk_grid_attach(GTK_GRID(grid), btn_sin, 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_cos, 1, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_tan, 2, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_log, 3, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_exp, 0, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_sqrt, 1, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_abs, 2, 5, 1, 1);

    // Agregar los operadores lógicos a la cuadrícula
    gtk_grid_attach(GTK_GRID(grid), btn_gt, 0, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_lt, 1, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_eq, 2, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_ne, 3, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_ge, 0, 7, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_le, 1, 7, 1, 1);

    // Agregar los botones especiales
    gtk_grid_attach(GTK_GRID(grid), btn_help, 3, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_clear, 2, 7, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_equals, 3, 7, 1, 1);

    // Agregar la cuadrícula al contenedor principal
    gtk_box_pack_start(GTK_BOX(vbox), grid, TRUE, TRUE, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
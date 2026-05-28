#include "Engine2D.h"
#include "line.h"
#include "canvas.h"
#include <iostream>

class proyecto1 : public Engine2D {
private:
    Canvas canvas;
    Color colorFondo = Color(0.1f, 0.1f, 0.15f);
    Color colorPincel = Color(1.0f, 0.0f, 0.0f);
    bool dibujando = false;
    enum BrushTypes {
        LINE,
        RECTANGLE,
        TRIANGLE,
        CIRCLE,
        NONE
      };
    int brush = LINE;
    int currentResize = LINE;

public:
    proyecto1(): Engine2D(1024, 600, "Proyecto #1 - Gestion y Despliegue de Primitivas"),
        canvas([this](int x, int y, const Color& color) { putPixel(x, y, color); }) {}
    void setup() override {
        clear(colorFondo);
        std::cout << "Motor inicializado exitosamente." << std::endl;



    }
    // Eventos
    void onkeyDown(int key) override {
        if (key == GLFW_KEY_SPACE) {
            clear(colorFondo);
            std::cout << brush << std::endl;
        }
    }

    void onMouseButtonDown(const int button, const double x, const double y) override {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            if (!dibujando) {
            dibujando = true;
                switch (brush) {
                    case LINE:
                        currentResize = LINE;
                        canvas.addLine(Point(static_cast<int>(x), static_cast<int>(y)), colorPincel);
                        break;
                    case RECTANGLE:
                        currentResize = RECTANGLE;
                        break;
                    case TRIANGLE:
                        currentResize = TRIANGLE;
                        break;
                    case CIRCLE:
                        currentResize = CIRCLE;
                        break;

                }
            }
        }
        //putPixel(x, y, colorPincel);
    }
    void onMouseButtonUp(int button, double x, double y) override {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            dibujando = false;
            currentResize = NONE;
        }
    }
    // Evento de movimiento continuo
    void onMouseMove(double x, double y) override {
        if (dibujando) {
            int ix = static_cast<int>(x);
            int iy = static_cast<int>(y);
            //putPixel(ix, iy, colorPincel);
            switch (currentResize) {
                case NONE:
                    break;
                case LINE:
                    if (Shape* latestShape = canvas.getLastShape(); latestShape != nullptr) {
                        Canvas::resizeLine(Point(ix, iy), latestShape);
                    }
                    break;

            }
        }

    }
    void update(float deltaTime) override {
        clear(colorFondo);
        canvas.draw();
    }
    void drawUI() override {
        ImGui::Begin("Herramientas");
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::Separator();
        float col[3] = { colorPincel.r, colorPincel.g, colorPincel.b };
        if (ImGui::ColorEdit3("Color Pincel", col)) {
            colorPincel.r = col[0];
            colorPincel.g = col[1];
            colorPincel.b = col[2];
        }
        ImGui::Text("Manten click izquierdo para dibujar.");
        ImGui::Text("Presiona ESPACIO para limpiar.");
        ImGui::RadioButton("Line", &brush, LINE); ImGui::SameLine();
        ImGui::RadioButton("Rectangle", &brush, RECTANGLE); ImGui::SameLine();
        ImGui::RadioButton("Triangle", &brush, TRIANGLE);ImGui::SameLine();
        ImGui::RadioButton("Circle", &brush, CIRCLE);
        ImGui::End();
    }
};

int main() {
    proyecto1 app;
    app.run();
    return 0;
}